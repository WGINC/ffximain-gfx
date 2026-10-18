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

    private void loadConv(String path) throws Exception {
        for (String l : Files.readAllLines(Paths.get(path))) {
            String[] p = l.trim().split("\\s+");
            if (p.length == 5 && p[1].equals("icall")) icall.put(Long.parseLong(p[0].replace("0x", ""), 16), p);
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
            if (params && p[3].equals("float") && !(f.getReturnType() instanceof Float10DataType))
                f.setReturnType(Float10DataType.dataType, SourceType.USER_DEFINED);
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

    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        List<String> addrs = Files.readAllLines(Paths.get(a[0]));
        if (a.length > 1 && !a[1].equals("-")) applyNames(a[1]);
        if (a.length > 3) loadConv(a[3]);
        applySdkPrototypes();
        DecompInterface d = new DecompInterface();
        DecompileOptions opt = new DecompileOptions();
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
        int ov = 0;
        for (String line : addrs) {
            line = line.trim(); if (line.isEmpty()) continue;
            Function f = getFunctionAt(addr(line));
            if (f != null) ov += overrideIcalls(f);
        }
        println("indirect call prototypes set: " + ov);
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
                String plain = mtext.replaceAll("/\\*XI_ICALL:0x[0-9A-F]+\\*/", "");
                if (plain.replaceAll("\\s+", "").length() != text.replaceAll("\\s+", "").length()) {
                    println("marked text does not align with Ghidra's at " + ea + "; indirect calls left unmarked");
                    marked.clear(); unmarked = true;
                } else {
                    java.util.regex.Matcher mk = java.util.regex.Pattern.compile("/\\*XI_ICALL:0x[0-9A-F]+\\*/").matcher(mtext);
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
                    objs.put(nm, String.format("{\"ea\": %d, \"func\": true, \"type\": %s, \"printed\": [%s], \"size\": 0, \"is_str\": false, \"purged\": %d}",
                            ta.getOffset(), esc(sig.replaceFirst("\\s*" + java.util.regex.Pattern.quote(nm) + "\\s*\\(", "(")), esc(nm), (purge == Function.UNKNOWN_STACK_DEPTH_CHANGE || purge < 0 ? -1 : purge)));
                } else if (!(t instanceof ClangVariableToken) && nm.matches("[A-Za-z_][A-Za-z0-9_]*")) {
                    // any other token that names a program symbol (PTR_ labels, functions used as values)
                    SymbolIterator it = currentProgram.getSymbolTable().getSymbols(nm);
                    Symbol sy = it.hasNext() ? it.next() : null;
                    if (sy == null || !sy.getAddress().isMemoryAddress() || f.getBody().contains(sy.getAddress())) continue;
                    Function g = getFunctionAt(sy.getAddress());
                    if (g != null)
                        objs.put(nm, String.format("{\"ea\": %d, \"func\": true, \"type\": %s, \"printed\": [%s], \"size\": 0, \"is_str\": false, \"purged\": %d}",
                                sy.getAddress().getOffset(), esc(g.getSignature(true).getPrototypeString(true).replaceFirst("\\s*" + java.util.regex.Pattern.quote(g.getName()) + "\\s*\\(", "(")), esc(nm), (g.getStackPurgeSize() == Function.UNKNOWN_STACK_DEPTH_CHANGE || g.getStackPurgeSize() < 0 ? -1 : g.getStackPurgeSize())));
                    else
                        objs.put(nm, String.format("{\"ea\": %d, \"func\": false, \"type\": \"undefined4\", \"printed\": [%s], \"size\": 4, \"is_str\": false}",
                                sy.getAddress().getOffset(), esc(nm)));
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
            for (Map.Entry<String, String> e : marked.entrySet()) { if (ib.length() > 1) ib.append(", "); ib.append(esc(e.getKey())).append(": ").append(e.getValue()); }
            ib.append("}");
            StringBuilder xb = new StringBuilder("[");
            for (String n : x87Locals(f, r.getHighFunction())) { if (xb.length() > 1) xb.append(", "); xb.append(esc(n)); }
            xb.append("]");
            out.println(String.format("{\"ea\": %d, \"name\": %s, \"type\": %s, \"text\": %s, \"objs\": %s, \"args\": %s, \"ret\": %s, \"cc\": %s, \"purged\": %d, \"icalls\": %s, \"icalls_unmarked\": %s, \"x87_locals\": %s, \"source\": \"ghidra\"}",
                    ea.getOffset(), esc(f.getName()), esc(f.getSignature(true).getPrototypeString(true)), esc(text), ob, ab,
                    esc(f.getReturnType().getDisplayName()), esc(cc == null ? "__cdecl" : cc), Math.max(f.getStackPurgeSize(), 0), ib, unmarked ? "true" : "false", xb));
            ok++;
        }
        out.close();
        d.dispose();
        println("decompiled " + ok + ", failed " + bad);
    }
}
