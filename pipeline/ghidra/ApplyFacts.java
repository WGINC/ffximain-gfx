// ApplyFacts.java -- applies ghidra/ffxi_facts.json (make_ghidra_facts.py) to the open program: each name the code
// establishes, where the function still has Ghidra's default name, and each function's facts as a plate comment.
//     analyzeHeadless PROJ_DIR PROJ -process FFXiMain_unpacked.dll -noanalysis -postScript ApplyFacts.java FACTS.json
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class ApplyFacts extends GhidraScript {
    @Override public void run() throws Exception {
        String js = new String(Files.readAllBytes(Paths.get(getScriptArgs()[0])), "UTF-8");
        int named = 0, kept = 0, notes = 0;
        // the file is flat JSON written by make_ghidra_facts.py: "names": {"0x...": "Name"}, "facts": {"0x...": ["..."]}
        Matcher m = Pattern.compile("\"(0x[0-9A-Fa-f]{8})\": \"([^\"]*)\"").matcher(section(js, "\"names\""));
        while (m.find()) {
            Function f = getFunctionAt(toAddr(m.group(1)));
            if (f == null) continue;
            if (f.getSymbol().getSource() == SourceType.DEFAULT || f.getName().startsWith("FUN_")) {
                f.setName(m.group(2), SourceType.USER_DEFINED); named++;
            } else kept++;
        }
        Matcher fm = Pattern.compile("\"(0x[0-9A-Fa-f]{8})\": \\[([^\\]]*)\\]").matcher(section(js, "\"facts\""));
        while (fm.find()) {
            Function f = getFunctionAt(toAddr(fm.group(1)));
            if (f == null) continue;
            List<String> items = new ArrayList<>();
            Matcher im = Pattern.compile("\"((?:[^\"\\\\]|\\\\.)*)\"").matcher(fm.group(2));
            while (im.find()) items.add(im.group(1).replace("\\\"", "\""));
            String note = "[gfx-recon facts 2026-10-21] " + String.join("; ", items);
            String old = f.getComment();
            if (old == null || !old.contains(note)) { f.setComment(old == null ? note : old + "\n" + note); notes++; }
        }
        println("names applied: " + named + " (functions already named: " + kept + "); functions annotated: " + notes);
    }

    private String section(String js, String key) {
        int a = js.indexOf(key); if (a < 0) return "";
        int b = js.indexOf('{', a), depth = 0;
        for (int i = b; i < js.length(); i++) {
            char c = js.charAt(i);
            if (c == '{') depth++;
            else if (c == '}' && --depth == 0) return js.substring(b, i + 1);
        }
        return js.substring(b);
    }
}
