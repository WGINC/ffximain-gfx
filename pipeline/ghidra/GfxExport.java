// GfxExport.java -- Ghidra headless script: decompile listed functions into the pipeline's JSONL
// export format (the same shape as ida_gfx.export_step), so hx2cpp/gate can use Ghidra when IDA is
// not available.
//
//   analyzeHeadless PROJDIR proj -import FFXiMain_unpacked.dll -postScript GfxExport.java ARGS
//   (or -process FFXiMain_unpacked.dll -noanalysis on later runs)
// ARGS: addresses.txt names.json out.jsonl [conv.txt]
//   conv.txt       conv_facts.py output: conventions, purges and float returns set before decompiling
//   addresses.txt  one hex address per line
//   names.json     {"0xADDR": "name"} applied as function/label names before decompiling
//
// Record: {"ea", "name", "type", "text", "objs": {printed: {ea, func, type, printed, size, is_str}},
//          "args": [[name, type]], "ret", "cc", "purged", "source": "ghidra"}
// @category FFXI

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.decompiler.component.DecompilerUtils;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import ghidra.program.model.pcode.*;
import ghidra.program.model.lang.Register;
import ghidra.program.model.lang.PrototypeModel;
import ghidra.program.database.SpecExtension;
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.framework.Application;
import generic.jar.ResourceFile;
import java.io.*;
import java.nio.file.*;
import java.util.*;

public class GfxExport extends GhidraScript {

    private static String esc(String s) {
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            switch (c) {
                case '"': b.append("\\\""); break;
                case '\\': b.append("\\\\"); break;
                case '\n': b.append("\\n"); break;
                case '\r': break;
                case '\t': b.append("\\t"); break;
                default:
                    if (c < 0x20) b.append(String.format("\\u%04x", (int) c)); else b.append(c);
            }
        }
        return b.append('"').toString();
    }

    private Address addr(String hex) {
        return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(Long.parseLong(hex.replace("0x", ""), 16));
    }

    private void applyNames(String path) throws Exception {
        String s = new String(Files.readAllBytes(Paths.get(path)));
        SymbolTable st = currentProgram.getSymbolTable();
        int n = 0;
        java.util.regex.Matcher m = java.util.regex.Pattern.compile("\"(0x[0-9A-Fa-f]+)\"\\s*:\\s*\"([^\"]+)\"").matcher(s);
        while (m.find()) {
            String name = m.group(2);
            if (name.startsWith("sub_") || !name.matches("[A-Za-z_][A-Za-z0-9_]*")) continue;
            Address a = addr(m.group(1));
            Function f = getFunctionAt(a);
            try {
                if (f != null) f.setName(name, SourceType.USER_DEFINED);
                else st.createLabel(a, name, SourceType.USER_DEFINED);
                n++;
            } catch (Exception e) { /* duplicate names: keep Ghidra's */ }
        }
        println("names applied: " + n);
    }

    // conv_facts.py: `0xADDR CC PURGE RET` per function, from the game's code (ecx/edx use, ret N, ST0 on return)
    private Map<Long, String[]> conv = new HashMap<>();

    // and per indirect call instruction: `0xCALL icall CC NSTACK RET`
    private Map<Long, String[]> icall = new HashMap<>();

    // `0xADDR regin EAX,EDX`: registers the function's code reads on entry beyond its convention's (entry_regs.json,
    // edx_use.json); `0xADDR keeps ecx,edx`: registers it leaves untouched, its callees included (reg_keeps.json)
    private Map<Long, String[]> regin = new HashMap<>(), keeps = new HashMap<>();
    // `0xADDR varargs N`: the IDB declares it variadic with N fixed parameters (cdecl: it pops 0)
    private Map<Long, Integer> varargs = new HashMap<>();

    // a variadic callee: its N fixed parameters (the committed ones, else undefined4) then `...`, so the
    // decompiler gives each call the arguments its own pushes hold
    private int applyVarargs() {
        int n = 0;
        for (Map.Entry<Long, Integer> e : varargs.entrySet()) {
            Function f = getFunctionAt(currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(e.getKey()));
            if (f == null) continue;
            try {
                List<Variable> ps = new ArrayList<>();
                Parameter[] have = f.getParameters();
                for (int i = 0; i < e.getValue(); i++)
                    ps.add(i < have.length && have[i].isStackVariable() ? new ParameterImpl(have[i].getName(), have[i].getDataType(), currentProgram)
                                                                         : new ParameterImpl("xi_s" + (i + 1), Undefined4DataType.dataType, currentProgram));
                f.setCustomVariableStorage(false);
                f.setCallingConvention("__cdecl");
                f.replaceParameters(ps, Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.USER_DEFINED);
                f.setVarArgs(true);
                n++;
            } catch (Exception x) { println("varargs not set at " + f.getEntryPoint() + ": " + x); }
        }
        return n;
    }

    private void loadConv(String path) throws Exception {
        for (String l : Files.readAllLines(Paths.get(path))) {
            String[] p = l.trim().split("\\s+");
            if (p.length == 3 && p[1].equals("regin")) regin.put(Long.parseLong(p[0].replace("0x", ""), 16), p[2].split(","));
            else if (p.length == 3 && p[1].equals("varargs")) varargs.put(Long.parseLong(p[0].replace("0x", ""), 16), Integer.parseInt(p[2]));
            else if (p.length == 3 && p[1].equals("keeps")) keeps.put(Long.parseLong(p[0].replace("0x", ""), 16), p[2].split(","));
            else if (p.length == 5 && p[1].equals("icall")) icall.put(Long.parseLong(p[0].replace("0x", ""), 16), p);
            else if (p.length == 4 && !p[1].equals("-")) conv.put(Long.parseLong(p[0].replace("0x", ""), 16), p);
        }
        println("conventions from the code: " + conv.size() + " functions, " + icall.size() + " indirect calls");
    }

    // an indirect call's prototype at the call site (Ghidra's signature override): the convention and the
    // stack arguments its pop count fixes, so the decompiler passes the object in ECX and the right pushes
    private int overrideIcalls(Function f) {
        int n = 0;
        for (Instruction in : currentProgram.getListing().getInstructions(f.getBody(), true)) {
            if (!in.getFlowType().isCall() || !in.getFlowType().isComputed()) continue;
            String[] p = icall.get(in.getAddress().getOffset());
            if (p == null) continue;
            try {
                FunctionDefinitionDataType fd = new FunctionDefinitionDataType("xi_icall_" + in.getAddress());
                fd.setReturnType(p[4].equals("float") ? Float10DataType.dataType : Undefined4DataType.dataType);
                List<ParameterDefinition> ps = new ArrayList<>();
                if (p[2].equals("__thiscall")) ps.add(new ParameterDefinitionImpl("this", new PointerDataType(VoidDataType.dataType), null));
                for (int i = 0; i < Integer.parseInt(p[3]); i++) ps.add(new ParameterDefinitionImpl("a" + (i + 1), Undefined4DataType.dataType, null));
                fd.setArguments(ps.toArray(new ParameterDefinition[0]));
                fd.setCallingConvention(p[2]);
                HighFunctionDBUtil.writeOverride(f, in.getAddress(), fd);
                n++;
            } catch (Exception e) { println("override not set at " + in.getAddress() + ": " + e); }
        }
        return n;
    }

    // a caller that reads EAX (or EDX) as a direct callee left it -- Ghidra's `extraout_` names, a register output
    // of the call that the caller uses -- reads the callee's register at its `ret`, whatever Ghidra committed as
    // the callee's return. The callee's return is widened to that register (EAX: undefined4, EDX: EDX:EAX,
    // undefined8) so the C++ call reads it too; a float return (fpret.json) is never widened. Callers that
    // read EAX through a callee that never sets it (a bare `ret`) leave that callee untranslatable (`in_EAX`).
    private int widenReturns(DecompInterface d, List<String> addrs) {
        Register eax = currentProgram.getRegister("EAX"), edx = currentProgram.getRegister("EDX");
        Map<Function, Integer> need = new LinkedHashMap<>();
        for (String line : addrs) {
            line = line.trim(); if (line.isEmpty()) continue;
            Function f = getFunctionAt(addr(line));
            if (f == null) continue;
            DecompileResults r = d.decompileFunction(f, 60, monitor);
            if (r == null || !r.decompileCompleted() || r.getHighFunction() == null) continue;
            Iterator<HighSymbol> it = r.getHighFunction().getLocalSymbolMap().getSymbols();
            List<HighVariable> hv = new ArrayList<>();
            while (it.hasNext()) { HighSymbol s = it.next(); if (s.getName().startsWith("extraout_") && s.getHighVariable() != null) hv.add(s.getHighVariable()); }
            Iterator<PcodeOpAST> ops = r.getHighFunction().getPcodeOps();
            Set<HighVariable> hs = new HashSet<>(hv);
            while (ops.hasNext()) {
                PcodeOpAST op = ops.next();
                Varnode o = op.getOutput();
                if (o == null || !o.isRegister() || !hs.contains(o.getHigh())) continue;
                if (op.getOpcode() != PcodeOp.INDIRECT && op.getOpcode() != PcodeOp.CALL) continue;
                long off = o.getOffset();
                int w;
                if (off >= eax.getOffset() && off < eax.getOffset() + 4) w = 4;
                else if (off >= edx.getOffset() && off < edx.getOffset() + 4) w = 8;
                else continue;
                Instruction in = currentProgram.getListing().getInstructionAt(op.getSeqnum().getTarget());
                if (in == null || !in.getFlowType().isCall() || in.getFlowType().isComputed() || in.getFlows().length != 1) continue;
                Function c = getFunctionAt(in.getFlows()[0]);
                if (c == null || c.isExternal() || c.isThunk()) continue;
                // a callee that leaves the register untouched does not return in it: the caller reads its own
                // value from before the call (the callee's prototype model says so, keepModels)
                if (kept(c).contains(w == 8 ? "edx" : "eax")) continue;
                need.merge(c, w, Math::max);
            }
        }
        int n = 0;
        for (Map.Entry<Function, Integer> e : need.entrySet()) {
            Function c = e.getKey();
            DataType rt = c.getReturnType();
            if (rt instanceof Float10DataType || rt instanceof AbstractFloatDataType) continue;
            if (rt.getLength() >= e.getValue() && !(rt instanceof VoidDataType)) continue;
            DataType t = e.getValue() == 8 ? Undefined8DataType.dataType : Undefined4DataType.dataType;
            try {
                if (c.hasCustomVariableStorage()) {
                    VariableStorage vs = e.getValue() == 8 ? new VariableStorage(currentProgram, edx, eax) : new VariableStorage(currentProgram, eax);
                    List<Parameter> ps = Arrays.asList(c.getParameters());
                    c.updateFunction(c.getCallingConventionName(), new ReturnParameterImpl(t, vs, true, currentProgram), ps,
                        Function.FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.USER_DEFINED);
                } else c.setReturnType(t, SourceType.USER_DEFINED);
                n++;
            } catch (Exception x) { println("return not widened at " + c.getEntryPoint() + ": " + x); }
        }
        return n;
    }

    // the decompiled text, line by line as Ghidra prints it, with `/*XI_ICALL:0xCALL*/` in front of the callee
    // expression of each indirect call that has an override, so hx2cpp calls it with that convention. The callee
    // starts at the call's first token after its `=` (its output); a `(` directly before it opens the callee.
    private String markedText(ClangTokenGroup root, Map<String, String> marked) {
        List<ClangLine> lines = DecompilerUtils.toLines(root);
        List<ClangToken> all = new ArrayList<>();
        for (ClangLine ln : lines) all.addAll(ln.getAllTokens());
        Map<PcodeOp, List<Integer>> byOp = new LinkedHashMap<>();
        for (int i = 0; i < all.size(); i++) {
            PcodeOp op = all.get(i).getPcodeOp();
            if (op != null && op.getOpcode() == PcodeOp.CALLIND) byOp.computeIfAbsent(op, k -> new ArrayList<>()).add(i);
        }
        Map<Integer, String> at = new HashMap<>();
        for (Map.Entry<PcodeOp, List<Integer>> e : byOp.entrySet()) {
            long ca = e.getKey().getSeqnum().getTarget().getOffset();
            String[] p = icall.get(ca);
            if (p == null) continue;
            List<Integer> ix = e.getValue();
            int start = ix.get(0);
            for (int j = 0; j < ix.size(); j++)
                if (all.get(ix.get(j)).getText().equals("=") && j + 1 < ix.size()) { start = ix.get(j + 1); break; }
            int k = start - 1;
            while (k >= 0 && all.get(k).getText().isEmpty()) k--;
            if (k >= 0 && all.get(k).getText().equals("(") && all.get(k).getPcodeOp() == null) start = k;
            String key = String.format("0x%08X", ca);
            at.put(start, "/*XI_ICALL:" + key + "*/");
            marked.put(key, "[\"" + p[2] + "\", " + p[3] + ", \"" + p[4] + "\"]");
        }
        // a floating constant: Ghidra prints its value without its width (`0.2` for the float 0x3E4CCCCD, which C++
        // reads as a double); `/*XI_K:size:bits*/` in front gives the constant's own encoding from its varnode
        for (int j = 0; j < all.size(); j++) {
            ClangToken t = all.get(j);
            if (!(t instanceof ClangVariableToken)) continue;
            Varnode v = ((ClangVariableToken) t).getVarnode();
            HighVariable hv = ((ClangVariableToken) t).getHighVariable();
            if (v == null || !v.isConstant() || hv == null || !(hv.getDataType() instanceof AbstractFloatDataType)) continue;
            if (v.getSize() != 4 && v.getSize() != 8) continue;
            String m = String.format("/*XI_K:%d:%X*/", v.getSize(), v.getSize() == 4 ? v.getOffset() & 0xFFFFFFFFL : v.getOffset());
            at.merge(j, m, (x, y) -> x + y);
            marked.putIfAbsent("", "");
        }
        // a piece `x._O_S_`: its sign from the piece varnode's own type (the varnode of size S the `.` token's op
        // reads or writes), `/*XI_PS:1*/` signed or `/*XI_PS:0*/` unsigned; none when the op's varnodes disagree
        for (int j = 0; j < all.size(); j++) {
            if (!all.get(j).getText().matches("_\\d+_\\d+_")) continue;
            int sz = Integer.parseInt(all.get(j).getText().split("_")[2]);
            int k = j - 1;
            while (k >= 0 && all.get(k).getText().isEmpty()) k--;
            if (k < 0 || !all.get(k).getText().equals(".") || all.get(k).getPcodeOp() == null) continue;
            PcodeOp op = all.get(k).getPcodeOp();
            Set<Boolean> sg = new HashSet<>();
            List<Varnode> vs = new ArrayList<>(Arrays.asList(op.getInputs()));
            if (op.getOutput() != null) vs.add(op.getOutput());
            for (Varnode v : vs) {
                if (v.isConstant() || v.getSize() != sz || v.getHigh() == null) continue;
                DataType dt = v.getHigh().getDataType();
                if (dt instanceof TypeDef) dt = ((TypeDef) dt).getBaseDataType();
                sg.add(dt instanceof AbstractIntegerDataType && ((AbstractIntegerDataType) dt).isSigned());
            }
            if (sg.size() != 1) continue;
            at.merge(j, sg.contains(true) ? "/*XI_PS:1*/" : "/*XI_PS:0*/", (x, y) -> x + y);
            marked.putIfAbsent("", "");
        }
        StringBuilder b = new StringBuilder();
        int i = 0;
        for (ClangLine ln : lines) {
            b.append(ln.getIndentString());
            for (ClangToken t : ln.getAllTokens()) {
                String m = at.get(i++);
                if (m != null) b.append(m);
                b.append(t.getText());
            }
            b.append('\n');
        }
        return b.toString();
    }

    // set what the code establishes: the convention, the return in ST0, and for callee-cleanup the stack
    // parameters that `ret N` pops (N/4 dwords) when the decompiler's committed list pops a different amount
    private boolean applyConv(Function f, boolean params) {
        String[] p = conv.get(f.getEntryPoint().getOffset());
        if (p == null) return false;
        try {
            if (!p[1].equals(f.getCallingConventionName())) f.setCallingConvention(p[1]);
            if (params && p[3].equals("st0in")) {
                // fpret.json `consumes`: the code's first x87 instruction pops a value the caller left in ST0
                // (__ftol). The return keeps the storage Ghidra committed; the one parameter is ST0.
                Register st0 = currentProgram.getRegister("ST0");
                if (f.getParameterCount() != 1 || !st0.equals(f.getParameter(0).getRegister())) {
                    Parameter ret = f.getReturn();
                    ReturnParameterImpl r = new ReturnParameterImpl(ret.getDataType(), ret.getVariableStorage(), true, currentProgram);
                    ParameterImpl x = new ParameterImpl("st0", Float10DataType.dataType, new VariableStorage(currentProgram, st0), currentProgram);
                    f.updateFunction(p[1], r, Function.FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.USER_DEFINED, x);
                }
                return true;
            }
            int purge = Integer.parseInt(p[2]);
            DataType rt0 = f.getReturnType();
            if (params && (p[3].equals("float") || rt0 instanceof Float10DataType || rt0 instanceof AbstractFloatDataType)) {
                // fpret.json `float` (or a float return Ghidra's decompiler inferred where fpret.json has no
                // verdict): MSVC returns it in ST0. Ghidra's standard storage returns a 10-byte float10
                // through a hidden pointer, so the storage is custom: the return in ST0, the convention's
                // registers (this: ECX; fastcall: ECX, EDX) and the stack parameters at their offsets, N/4 of
                // them for a callee that pops N (purges.json), the decompiler's own for a __cdecl one
                Register st0 = currentProgram.getRegister("ST0");
                List<Variable> ps = new ArrayList<>();
                if (p[1].equals("__thiscall") || p[1].equals("__fastcall"))
                    ps.add(new ParameterImpl(p[1].equals("__thiscall") ? "this" : "xi_ecx", new PointerDataType(VoidDataType.dataType),
                        new VariableStorage(currentProgram, currentProgram.getRegister("ECX")), currentProgram));
                if (p[1].equals("__fastcall"))
                    ps.add(new ParameterImpl("xi_edx", Undefined4DataType.dataType, new VariableStorage(currentProgram, currentProgram.getRegister("EDX")), currentProgram));
                List<Parameter> stack = new ArrayList<>();
                int have = 0;
                for (Parameter q : f.getParameters()) if (q.isStackVariable() && !q.isAutoParameter() && !q.getName().equals("__return_storage_ptr__")) { stack.add(q); have += Math.max(4, (q.getLength() + 3) & ~3); }
                if (p[1].equals("__cdecl") || have == purge)
                    for (Parameter q : stack) ps.add(new ParameterImpl(q.getName(), q.getDataType(), q.getStackOffset(), currentProgram));
                else
                    for (int i = 0; i < purge / 4; i++) ps.add(new ParameterImpl("xi_s" + (i + 1), Undefined4DataType.dataType, 4 + 4 * i, currentProgram));
                DataType rt = p[3].equals("float") || rt0 instanceof Float10DataType ? Float10DataType.dataType : rt0;
                f.updateFunction(p[1], new ReturnParameterImpl(rt, new VariableStorage(currentProgram, st0), currentProgram),
                    ps, Function.FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.USER_DEFINED);
                return true;
            }
            if (params && f.hasCustomVariableStorage()) {
                // a standard convention (conv.txt) places the parameters itself; custom storage left from
                // committing the decompiler's view can drop the `this` a __thiscall callee reads in ECX
                f.setCustomVariableStorage(false);
            }
            if (params && !p[1].equals("__cdecl")) {
                int have = 0;
                for (Parameter q : f.getParameters()) if (q.isStackVariable()) have += Math.max(4, (q.getLength() + 3) & ~3);
                if (have != purge) {
                    List<Variable> keep = new ArrayList<>();
                    for (Parameter q : f.getParameters()) if (!q.isStackVariable() && !q.isAutoParameter()) keep.add(q);
                    for (int i = 0; i < purge / 4; i++)
                        keep.add(new ParameterImpl("xi_s" + (i + 1), Undefined4DataType.dataType, currentProgram));
                    f.replaceParameters(keep, Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.USER_DEFINED);
                }
            }
            return true;
        } catch (Exception e) { println("convention not set at " + f.getEntryPoint() + ": " + e); return false; }
    }

    // the Windows SDK's declared prototypes, as Ghidra ships them compiled from the SDK headers, applied by
    // name to the imported functions (externals); the names that got one
    private Set<String> sdkApplied = new HashSet<>();

    private void applySdkPrototypes() throws Exception {
        ResourceFile rf = Application.getModuleDataFile("Base", "typeinfo/win32/windows_vs12_32.gdt");
        FileDataTypeManager dtm = FileDataTypeManager.openFileArchive(rf, false);
        Map<String, FunctionDefinition> defs = new HashMap<>();
        Iterator<DataType> it = dtm.getAllDataTypes();
        while (it.hasNext()) { DataType t = it.next(); if (t instanceof FunctionDefinition) defs.putIfAbsent(t.getName(), (FunctionDefinition) t); }
        for (Function ext : currentProgram.getFunctionManager().getExternalFunctions()) {
            FunctionDefinition fd = defs.get(ext.getName());
            if (fd == null) continue;
            if (new ApplyFunctionSignatureCmd(ext.getEntryPoint(), fd, SourceType.IMPORTED).applyTo(currentProgram)) sdkApplied.add(ext.getName());
        }
        dtm.close();
        println("SDK prototypes applied to imports: " + sdkApplied.size());
    }

    // locals that hold an x87 register value: Ghidra narrows an x87 accumulator to the `float` it is finally stored
    // as, but the original keeps it in ST (rounded by the control word, not to float) until a `fstp dword`. In a
    // function with no SSE instruction a float value outside memory is in an x87 register; a local whose every
    // definition is a float p-code op, a load, a phi or a copy of a constant or of such a value is one of them
    // (at least one float arithmetic op among them), declared float10 by hx2cpp.
    private static final Set<Integer> FOPS = new HashSet<>(Arrays.asList(PcodeOp.FLOAT_ADD, PcodeOp.FLOAT_SUB,
        PcodeOp.FLOAT_MULT, PcodeOp.FLOAT_DIV, PcodeOp.FLOAT_NEG, PcodeOp.FLOAT_ABS, PcodeOp.FLOAT_SQRT,
        PcodeOp.FLOAT_INT2FLOAT, PcodeOp.FLOAT_FLOAT2FLOAT, PcodeOp.FLOAT_CEIL, PcodeOp.FLOAT_FLOOR, PcodeOp.FLOAT_ROUND));

    // the decompiler's locals in the stack frame: [name, offset from the entry ESP, size], from each symbol's storage
    private String stackLocals(HighFunction h) {
        StringBuilder b = new StringBuilder("[");
        Iterator<HighSymbol> it = h.getLocalSymbolMap().getSymbols();
        while (it.hasNext()) {
            HighSymbol sy = it.next();
            if (sy.isParameter()) continue;
            VariableStorage st = sy.getStorage();
            if (st == null || !st.isStackStorage()) continue;
            if (b.length() > 1) b.append(", ");
            b.append("[").append(esc(sy.getName())).append(", ").append(st.getStackOffset()).append(", ").append(sy.getSize()).append("]");
        }
        return b.append("]").toString();
    }

    private List<String> x87Locals(Function f, HighFunction h) {
        List<String> out = new ArrayList<>();
        for (Instruction in : currentProgram.getListing().getInstructions(f.getBody(), true))
            for (int i = 0; i < in.getNumOperands(); i++)
                for (Object o : in.getOpObjects(i))
                    if (o instanceof Register && ((Register) o).getName().startsWith("XMM")) return out;
        Iterator<HighSymbol> it = h.getLocalSymbolMap().getSymbols();
        while (it.hasNext()) {
            HighSymbol sy = it.next(); HighVariable v = sy.getHighVariable();
            if (v == null || !(v instanceof HighLocal) || sy.isParameter()) continue;
            DataType dt = sy.getDataType();
            if (!(dt instanceof FloatDataType || dt instanceof DoubleDataType)) continue;
            VariableStorage st = sy.getStorage();
            if (st == null || st.isStackStorage() || st.isMemoryStorage()) continue;
            boolean arith = false, ok = true;
            for (Varnode vn : v.getInstances()) {
                PcodeOp d = vn.getDef();
                if (d == null) { ok = false; break; }
                int c = d.getOpcode();
                if (FOPS.contains(c)) arith = true;
                else if (c == PcodeOp.MULTIEQUAL || c == PcodeOp.LOAD || c == PcodeOp.INDIRECT) continue;
                else if (c == PcodeOp.COPY) {
                    Varnode src = d.getInput(0); PcodeOp sd = src.getDef();
                    if (!(src.isConstant() || (sd != null && (FOPS.contains(sd.getOpcode()) || sd.getOpcode() == PcodeOp.MULTIEQUAL || sd.getOpcode() == PcodeOp.LOAD)))) { ok = false; break; }
                }
                else { ok = false; break; }
            }
            if (ok && arith) out.add(sy.getName());
        }
        return out;
    }

    // A function that reads EAX, ECX or EDX as it found it at entry (`lea eax,[ecx+0x44]; ret` at 0x1002B5A0
    // returns the caller's EDX in EDX; a bare `ret` at 0x100279A0 returns the caller's EAX) shows the entry value
    // as in_EAX / in_ECX / in_EDX in its own decompile. That value is an input, so it becomes a parameter in
    // that register and each caller passes what it holds there (source: the function's own dataflow).
    // the registers a function's code reads on entry that its convention does not pass (conv.txt `regin`, from
    // entry_regs.json and edx_use.json), added as parameters in those registers so its callers in the set pass them
    // (hx2cpp calls it through an exact asm call and refuses such a function as an entry itself)
    private int registerInputs(Collection<Function> fs) {
        int n = 0;
        for (Function c : fs) {
            String[] rs = regin.get(c.getEntryPoint().getOffset());
            if (rs == null || c.isExternal() || c.isThunk()) continue;
            List<Register> add = new ArrayList<>();
            for (String g : rs) {
                Register reg = currentProgram.getRegister(g);
                boolean has = false;
                for (Parameter p : c.getParameters()) if (p.isRegisterVariable() && p.getRegister().getBaseRegister().equals(reg.getBaseRegister())) has = true;
                if (!has) add.add(reg);
            }
            if (add.isEmpty()) continue;
            try {
                List<Parameter> ps = new ArrayList<>();
                for (Parameter p : c.getParameters()) if (!p.getName().equals("__return_storage_ptr__"))
                    ps.add(new ParameterImpl(p.getName(), p.getDataType(), p.getVariableStorage(), currentProgram));
                for (Register g : add)
                    ps.add(new ParameterImpl("xi_" + g.getName().toLowerCase(), Undefined4DataType.dataType, new VariableStorage(currentProgram, g), currentProgram));
                c.updateFunction(c.getCallingConventionName(), c.getReturn(), ps,
                    Function.FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.USER_DEFINED);
                n++;
            } catch (Exception x) { println("register input not added at " + c.getEntryPoint() + ": " + x); }
        }
        return n;
    }

    // the registers conv.txt says the function leaves untouched; EAX only while it returns nothing (a function
    // that returns a value in EAX and never writes EAX returns its caller's EAX: Ghidra's return already says that)
    private Set<String> kept(Function c) {
        String[] k = keeps.get(c.getEntryPoint().getOffset());
        Set<String> r = new TreeSet<>();
        if (k == null) return r;
        r.addAll(Arrays.asList(k));
        if (!(c.getReturnType() instanceof VoidDataType)) r.remove("eax");
        return r;
    }

    static final String KEEP = "_xikeep";

    // Ghidra's x86 conventions say a call kills ECX and EDX (and EAX by the output); the game's compiler kept values
    // in them across calls to functions it knew leave them alone (reg_preserve.py). Each such callee gets a prototype
    // model that is its convention with those registers unaffected (a compiler-spec extension), so the caller's text
    // reads the value it had before the call instead of an `extraout_` the callee never produced. The suffix is
    // removed from everything written out.
    private int keepModels(Collection<Function> fs) throws Exception {
        Set<String> base = new HashSet<>(Arrays.asList("__cdecl", "__stdcall", "__thiscall", "__fastcall"));
        Map<String, Integer> off = new HashMap<>();
        for (String g : new String[] { "eax", "ecx", "edx" }) off.put(g, currentProgram.getRegister(g.toUpperCase()).getOffset());
        Set<String> made = new HashSet<>();
        int n = 0;
        for (Function c : fs) {
            if (c.isExternal() || c.isThunk()) continue;
            String cc = c.getCallingConventionName();
            Set<String> k = kept(c);
            if (k.isEmpty() || !base.contains(cc)) continue;
            String name = cc + KEEP + "_" + String.join("_", k);
            if (made.add(name) && currentProgram.getCompilerSpec().getCallingConvention(name) == null) {
                PrototypeModel m = currentProgram.getCompilerSpec().getCallingConvention(cc);
                XmlEncode e = new XmlEncode(true);
                m.encode(e, currentProgram.getCompilerSpec().getPcodeInjectLibrary());
                String x = e.toString().replaceFirst("name=\"" + java.util.regex.Pattern.quote(cc) + "\"", "name=\"" + name + "\"");
                StringBuilder un = new StringBuilder();
                for (String g : k) {
                    String vn = String.format("<varnode space=\"register\" offset=\"0x%x\" size=\"4\"/>", off.get(g));
                    x = x.replace(vn, "");      // register varnodes appear only in killedbycall, likelytrash, unaffected
                    un.append(vn);
                    if (g.equals("eax")) x = x.replaceFirst("<output killedbycall=\"true\"", "<output killedbycall=\"false\"");
                }
                x = x.replaceFirst("<unaffected>", "<unaffected>" + un);
                x = x.replaceAll("<(killedbycall|likelytrash)>\\s*</\\1>", "");     // a list left empty is not allowed
                new SpecExtension(currentProgram).addReplaceCompilerSpecExtension(x, monitor);
            }
            try { c.setCallingConvention(name); n++; }
            catch (Exception x) { println("kept-register model not set at " + c.getEntryPoint() + ": " + x); }
        }
        return n;
    }

    // where each parameter lives, in the prototype's order: a register name or "stack" (hx2cpp calls a callee
    // through registers its convention does not cover with an exact asm call, and refuses such an entry itself)
    private String storage(Function g) {
        StringBuilder b = new StringBuilder("[");
        for (Parameter p : g.getParameters()) {
            if (b.length() > 1) b.append(", ");
            Register r = p.isRegisterVariable() ? p.getRegister() : null;
            b.append(esc(r != null ? r.getBaseRegister().getName() : p.isStackVariable() ? "stack" : "other"));
        }
        return b.append("]").toString();
    }

    // The decompiler's "floatprecision" rules (SubfloatFlow) shrink x87 80-bit operations to the width of the
    // value finally stored, which erases where the instructions round: an fstp to a float slot that is loaded
    // back, or an fmul of an 80-bit product. With the group off, every FLOAT2FLOAT the instructions perform is
    // printed as a cast and every 80-bit operation is typed float10, so the C++ rounds exactly where the game
    // does (source: the p-code of the instructions). The option is the decompiler's own <currentaction> toggle,
    // appended to the options list the Java side encodes.
    static class ExactFloatOptions extends DecompileOptions {
        @Override public void encode(Encoder enc, DecompInterface iface) throws IOException {
            Encoder px = (Encoder) java.lang.reflect.Proxy.newProxyInstance(Encoder.class.getClassLoader(),
                new Class<?>[] { Encoder.class }, (p, m, args) -> {
                    if (m.getName().equals("closeElement") && args[0] == ElementId.ELEM_OPTIONSLIST) {
                        enc.openElement(ElementId.ELEM_CURRENTACTION);
                        String[] v = { iface.getSimplificationStyle(), "floatprecision", "off" };
                        ElementId[] e = { ElementId.ELEM_PARAM1, ElementId.ELEM_PARAM2, ElementId.ELEM_PARAM3 };
                        for (int i = 0; i < 3; i++) { enc.openElement(e[i]); enc.writeString(AttributeId.ATTRIB_CONTENT, v[i]); enc.closeElement(e[i]); }
                        enc.closeElement(ElementId.ELEM_CURRENTACTION);
                    }
                    try { return m.invoke(enc, args); } catch (java.lang.reflect.InvocationTargetException x) { throw x.getCause(); }
                });
            super.encode(px, iface);
        }
    }

    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        List<String> addrs = Files.readAllLines(Paths.get(a[0]));
        if (a.length > 1 && !a[1].equals("-")) applyNames(a[1]);
        if (a.length > 3) loadConv(a[3]);
        applySdkPrototypes();
        DecompInterface d = new DecompInterface();
        DecompileOptions opt = new ExactFloatOptions();
        d.setOptions(opt);
        d.toggleCCode(true); d.toggleSyntaxTree(true);
        d.openProgram(currentProgram);
        // Pass 1: commit the signatures the decompiler infers for every target and every function it calls,
        // so calls in the final text carry real prototypes (otherwise callees read `undefined f(void)`).
        Set<Function> fixSig = new LinkedHashSet<>();
        for (String line : addrs) {
            line = line.trim(); if (line.isEmpty()) continue;
            Function f = getFunctionAt(addr(line));
            if (f == null) f = createFunction(addr(line), null);
            if (f == null) continue;
            fixSig.add(f);
            for (Function c : f.getCalledFunctions(monitor)) if (!c.isThunk() && !c.isExternal()) fixSig.add(c);
        }
        int set = 0;
        for (Function f : fixSig) if (applyConv(f, false)) set++;
        println("conventions set before decompiling: " + set + " of " + fixSig.size());
        int committed = 0;
        for (Function f : fixSig) {
            if (f.getSignatureSource() == SourceType.USER_DEFINED || f.getSignatureSource() == SourceType.IMPORTED) continue;
            DecompileResults r = d.decompileFunction(f, 60, monitor);
            if (r == null || !r.decompileCompleted() || r.getHighFunction() == null) continue;
            try {
                HighFunctionDBUtil.commitParamsToDatabase(r.getHighFunction(), true, HighFunctionDBUtil.ReturnCommitOption.COMMIT, SourceType.ANALYSIS);
                committed++;
            } catch (Exception e) { /* keep Ghidra's */ }
        }
        println("signatures committed: " + committed + " of " + fixSig.size());
        for (Function f : fixSig) applyConv(f, true);     // the commit may have replaced them
        // A stack argument is a 4-byte slot the caller fills with a 4-byte push; a narrower inferred parameter
        // (the decompiler typed it from the callee reading only its low bytes) would make the C++ caller pass a
        // zero- or sign-extended value where the game pushes the whole register. Parameter types here are all
        // inferred (conv.txt carries convention and pop count only); SDK-declared prototypes keep theirs.
        int widened = 0;
        for (Function f : fixSig) {
            if (f.getSignatureSource() == SourceType.IMPORTED) continue;   // SDK-declared
            for (Parameter p : f.getParameters()) {
                if (p.isAutoParameter() || !p.isStackVariable() || p.getLength() >= 4) continue;
                try {
                    if (f.hasCustomVariableStorage())
                        p.setDataType(Undefined4DataType.dataType, new VariableStorage(currentProgram, p.getStackOffset(), 4), false, SourceType.ANALYSIS);
                    else p.setDataType(Undefined4DataType.dataType, SourceType.ANALYSIS);
                    widened++;
                } catch (Exception e) { println("parameter not widened at " + f.getEntryPoint() + ": " + e.getMessage()); }
            }
        }
        println("narrow stack parameters widened to their 4-byte slot: " + widened);
        int ov = 0;
        for (String line : addrs) {
            line = line.trim(); if (line.isEmpty()) continue;
            Function f = getFunctionAt(addr(line));
            if (f != null) ov += overrideIcalls(f);
        }
        println("indirect call prototypes set: " + ov);
        // repeated: a widened function returns what its own callee left in EAX, so that callee is widened next
        println("variadic prototypes set: " + applyVarargs());
        println("register inputs added: " + registerInputs(fixSig));
        println("kept-register prototype models set: " + keepModels(fixSig));
        d.dispose(); d = new DecompInterface(); d.setOptions(opt); d.toggleCCode(true); d.toggleSyntaxTree(true); d.openProgram(currentProgram);
        for (int round = 0, w = 1; round < 8 && w > 0; round++) {
            w = widenReturns(d, addrs);
            println("round " + round + ": callee returns widened to the register a caller reads: " + w);
        }
        d.dispose(); d = new DecompInterface(); d.setOptions(opt); d.toggleCCode(true); d.toggleSyntaxTree(true); d.openProgram(currentProgram);
        PrintWriter out = new PrintWriter(new FileWriter(a[2]));
        int ok = 0, bad = 0;
        for (String line : addrs) {
            line = line.trim(); if (line.isEmpty()) continue;
            Address ea = addr(line);
            Function f = getFunctionAt(ea);
            if (f == null) { f = createFunction(ea, null); }
            if (f == null) { bad++; continue; }
            DecompileResults r = d.decompileFunction(f, 120, monitor);
            boolean unmarked = false;
            if (r == null || !r.decompileCompleted()) { bad++; continue; }
            String text = r.getDecompiledFunction().getC();
            Map<String, String> marked = new LinkedHashMap<>();
            String mtext = markedText(r.getCCodeMarkup(), marked);
            if (!marked.isEmpty()) {
                // the printed text sanitises symbol names character for character (`s_a.b` -> `s_a_b`), so each
                // marker goes into Ghidra's own text after the same number of non-blank characters
                String plain = mtext.replaceAll("/\\*XI_[A-Z]+:[0-9A-Fx:]+\\*/", "");
                if (plain.replaceAll("\\s+", "").length() != text.replaceAll("\\s+", "").length()) {
                    println("marked text does not align with Ghidra's at " + ea + "; indirect calls left unmarked");
                    marked.clear(); unmarked = true;
                } else {
                    java.util.regex.Matcher mk = java.util.regex.Pattern.compile("/\\*XI_[A-Z]+:[0-9A-Fx:]+\\*/").matcher(mtext);
                    StringBuilder nb = new StringBuilder();
                    int nonblank = 0, mpos = 0, tpos = 0;
                    while (mk.find()) {
                        for (int q = mpos; q < mk.start(); q++) if (!Character.isWhitespace(mtext.charAt(q))) nonblank++;
                        mpos = mk.end();
                        int seen = 0, q = 0;
                        for (q = 0; q < text.length() && seen < nonblank; q++) if (!Character.isWhitespace(text.charAt(q))) seen++;
                        while (q < text.length() && Character.isWhitespace(text.charAt(q))) q++;
                        nb.append(text, tpos, q).append(mk.group()); tpos = q;
                    }
                    text = nb.append(text.substring(tpos)).toString();
                }
            }
            // objects referenced by the body: called functions and globals, by their printed name
            Map<String, String> objs = new LinkedHashMap<>();
            ClangTokenGroup root = r.getCCodeMarkup();
            List<ClangNode> toks = new ArrayList<>();
            root.flatten(toks);
            for (ClangNode cn : toks) {
                if (!(cn instanceof ClangToken)) continue;
                ClangToken t = (ClangToken) cn;
                String nm = t.getText();
                if (objs.containsKey(nm)) continue;
                if (t instanceof ClangFuncNameToken) {
                    PcodeOp op = ((ClangFuncNameToken) t).getPcodeOp();
                    Address ta = null;
                    if (op != null && op.getOpcode() == PcodeOp.CALL) ta = op.getInput(0).getAddress();
                    if (ta == null) { Function g = getGlobalFunctions(nm).isEmpty() ? null : getGlobalFunctions(nm).get(0); if (g != null) ta = g.getEntryPoint(); }
                    if (ta == null || ta.equals(ea)) continue;
                    Function g = getFunctionAt(ta);
                    Function ext = g == null ? currentProgram.getFunctionManager().getReferencedFunction(ta) : null;
                    if (ext != null && ext.isExternal()) {
                        // a call through the import table slot `ta`: the prototype is the SDK's (windows_vs12_32.gdt)
                        // where one was found by name, else none ("import_proto": false; hx2cpp refuses the call)
                        boolean known = sdkApplied.contains(ext.getName());
                        objs.put(nm, String.format("{\"ea\": %d, \"func\": true, \"import\": true, \"import_proto\": %s, \"type\": %s, \"printed\": [%s], \"size\": 0, \"is_str\": false, \"purged\": -1}",
                                ta.getOffset(), known ? "true" : "false", esc(ext.getSignature(true).getPrototypeString(true).replaceFirst("\\s*" + java.util.regex.Pattern.quote(ext.getName()) + "\\s*\\(", "(")), esc(nm)));
                        continue;
                    }
                    String sig = g != null ? g.getSignature(true).getPrototypeString(true) : "int __cdecl(void)";
                    int purge = g != null ? g.getStackPurgeSize() : 0;
                    objs.put(nm, String.format("{\"ea\": %d, \"func\": true, \"type\": %s, \"printed\": [%s], \"size\": 0, \"is_str\": false, \"purged\": %d, \"storage\": %s}",
                            ta.getOffset(), esc(sig.replaceFirst("\\s*" + java.util.regex.Pattern.quote(nm) + "\\s*\\(", "(")), esc(nm), (purge == Function.UNKNOWN_STACK_DEPTH_CHANGE || purge < 0 ? -1 : purge), g != null ? storage(g) : "[]"));
                } else if (nm.matches("[A-Za-z_][A-Za-z0-9_]*") && (!(t instanceof ClangVariableToken)
                        || !(((ClangVariableToken) t).getHighVariable() instanceof HighGlobal))) {
                    // a variable token with no HighGlobal behind it is an address taken of a symbol (`&g_x`)
                    if (t instanceof ClangVariableToken && ((ClangVariableToken) t).getHighVariable() != null) continue;
                    // any other token that names a program symbol (PTR_ labels, functions used as values)
                    SymbolIterator it = currentProgram.getSymbolTable().getSymbols(nm);
                    Symbol sy = it.hasNext() ? it.next() : null;
                    if (sy == null || !sy.getAddress().isMemoryAddress() || f.getBody().contains(sy.getAddress())) continue;
                    Function g = getFunctionAt(sy.getAddress());
                    if (g != null)
                        objs.put(nm, String.format("{\"ea\": %d, \"func\": true, \"type\": %s, \"printed\": [%s], \"size\": 0, \"is_str\": false, \"purged\": %d, \"storage\": %s}",
                                sy.getAddress().getOffset(), esc(g.getSignature(true).getPrototypeString(true).replaceFirst("\\s*" + java.util.regex.Pattern.quote(g.getName()) + "\\s*\\(", "(")), esc(nm), (g.getStackPurgeSize() == Function.UNKNOWN_STACK_DEPTH_CHANGE || g.getStackPurgeSize() < 0 ? -1 : g.getStackPurgeSize()), storage(g)));
                    else {
                        // the data defined at the symbol, if any; otherwise its address is all that is used
                        Data dd = getDataAt(sy.getAddress());
                        DataType dt = dd != null ? dd.getDataType() : Undefined4DataType.dataType;
                        objs.put(nm, String.format("{\"ea\": %d, \"func\": false, \"type\": %s, \"printed\": [%s], \"size\": %d, \"is_str\": %s}",
                                sy.getAddress().getOffset(), esc(dt.getDisplayName()), esc(nm), Math.max(dt.getLength(), 1), dd != null && dd.hasStringValue() ? "true" : "false"));
                    }
                } else if (t instanceof ClangVariableToken) {
                    HighVariable hv = ((ClangVariableToken) t).getHighVariable();
                    if (!(hv instanceof HighGlobal)) continue;
                    HighSymbol hs = hv.getSymbol();
                    Address ga = hs != null && hs.getStorage() != null ? hs.getStorage().getMinAddress() : null;
                    if (ga == null || !ga.isMemoryAddress() || f.getBody().contains(ga)) continue;
                    DataType dt = hv.getDataType();
                    Data dd = getDataAt(ga);
                    boolean str = dd != null && dd.hasStringValue();
                    objs.put(nm, String.format("{\"ea\": %d, \"func\": false, \"type\": %s, \"printed\": [%s], \"size\": %d, \"is_str\": %s}",
                            ga.getOffset(), esc(dt.getDisplayName()), esc(nm), dt.getLength(), str ? "true" : "false"));
                }
            }
            StringBuilder ob = new StringBuilder("{");
            boolean first = true;
            for (Map.Entry<String, String> e : objs.entrySet()) {
                if (!first) ob.append(", "); first = false;
                ob.append(esc(e.getKey())).append(": ").append(e.getValue());
            }
            ob.append("}");
            StringBuilder ab = new StringBuilder("[");
            Parameter[] ps = f.getParameters();
            for (int i = 0; i < ps.length; i++) {
                if (i > 0) ab.append(", ");
                ab.append("[").append(esc(ps[i].getName())).append(", ").append(esc(ps[i].getDataType().getDisplayName())).append("]");
            }
            ab.append("]");
            String cc = f.getCallingConventionName();
            StringBuilder ib = new StringBuilder("{");
            for (Map.Entry<String, String> e : marked.entrySet()) { if (e.getKey().isEmpty()) continue; if (ib.length() > 1) ib.append(", "); ib.append(esc(e.getKey())).append(": ").append(e.getValue()); }
            ib.append("}");
            StringBuilder xb = new StringBuilder("[");
            for (String n : x87Locals(f, r.getHighFunction())) { if (xb.length() > 1) xb.append(", "); xb.append(esc(n)); }
            xb.append("]");
            out.println(String.format("{\"ea\": %d, \"name\": %s, \"type\": %s, \"text\": %s, \"objs\": %s, \"args\": %s, \"ret\": %s, \"cc\": %s, \"purged\": %d, \"icalls\": %s, \"icalls_unmarked\": %s, \"x87_locals\": %s, \"stack_locals\": %s, \"storage\": %s, \"source\": \"ghidra\"}",
                    ea.getOffset(), esc(f.getName()), esc(f.getSignature(true).getPrototypeString(true)), esc(text), ob, ab,
                    esc(f.getReturnType().getDisplayName()), esc(cc == null ? "__cdecl" : cc), Math.max(f.getStackPurgeSize(), 0), ib, unmarked ? "true" : "false", xb, stackLocals(r.getHighFunction()), storage(f)).replaceAll(KEEP + "(?:_(?:eax|ecx|edx))+", ""));
            ok++;
        }
        out.close();
        d.dispose();
        println("decompiled " + ok + ", failed " + bad);
    }
}
