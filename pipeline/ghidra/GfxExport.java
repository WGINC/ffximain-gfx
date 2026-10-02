// GfxExport.java -- Ghidra headless script: decompile listed functions into the pipeline's JSONL
// export format (the same shape as ida_gfx.export_step), so hx2cpp/gate can use Ghidra when IDA is
// not available.
//
//   analyzeHeadless PROJDIR proj -import FFXiMain_unpacked.dll -postScript GfxExport.java ARGS
//   (or -process FFXiMain_unpacked.dll -noanalysis on later runs)
// ARGS: addresses.txt names.json out.jsonl
//   addresses.txt  one hex address per line
//   names.json     {"0xADDR": "name"} applied as function/label names before decompiling
//
// Record: {"ea", "name", "type", "text", "objs": {printed: {ea, func, type, printed, size, is_str}},
//          "args": [[name, type]], "ret", "cc", "purged", "source": "ghidra"}
// @category FFXI

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import ghidra.program.model.pcode.*;
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

    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        List<String> addrs = Files.readAllLines(Paths.get(a[0]));
        if (a.length > 1 && !a[1].equals("-")) applyNames(a[1]);
        DecompInterface d = new DecompInterface();
        DecompileOptions opt = new DecompileOptions();
        d.setOptions(opt);
        d.toggleCCode(true); d.toggleSyntaxTree(true);
        d.openProgram(currentProgram);
        PrintWriter out = new PrintWriter(new FileWriter(a[2]));
        int ok = 0, bad = 0;
        for (String line : addrs) {
            line = line.trim(); if (line.isEmpty()) continue;
            Address ea = addr(line);
            Function f = getFunctionAt(ea);
            if (f == null) { f = createFunction(ea, null); }
            if (f == null) { bad++; continue; }
            DecompileResults r = d.decompileFunction(f, 120, monitor);
            if (r == null || !r.decompileCompleted()) { bad++; continue; }
            String text = r.getDecompiledFunction().getC();
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
                    String sig = g != null ? g.getSignature(true).getPrototypeString(true) : "int __cdecl(void)";
                    int purge = g != null ? g.getStackPurgeSize() : 0;
                    objs.put(nm, String.format("{\"ea\": %d, \"func\": true, \"type\": %s, \"printed\": [%s], \"size\": 0, \"is_str\": false, \"purged\": %d}",
                            ta.getOffset(), esc(sig.replaceFirst("\\s*" + java.util.regex.Pattern.quote(nm) + "\\s*\\(", "(")), esc(nm), Math.max(purge, 0)));
                } else if (t instanceof ClangVariableToken) {
                    HighVariable hv = ((ClangVariableToken) t).getHighVariable();
                    if (!(hv instanceof HighGlobal)) continue;
                    HighSymbol hs = hv.getSymbol();
                    Address ga = hs != null && hs.getStorage() != null ? hs.getStorage().getMinAddress() : null;
                    if (ga == null || !ga.isMemoryAddress()) continue;
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
            out.println(String.format("{\"ea\": %d, \"name\": %s, \"type\": %s, \"text\": %s, \"objs\": %s, \"args\": %s, \"ret\": %s, \"cc\": %s, \"purged\": %d, \"source\": \"ghidra\"}",
                    ea.getOffset(), esc(f.getName()), esc(f.getSignature(true).getPrototypeString(true)), esc(text), ob, ab,
                    esc(f.getReturnType().getDisplayName()), esc(cc == null ? "__cdecl" : cc), Math.max(f.getStackPurgeSize(), 0)));
            ok++;
        }
        out.close();
        d.dispose();
        println("decompiled " + ok + ", failed " + bad);
    }
}
