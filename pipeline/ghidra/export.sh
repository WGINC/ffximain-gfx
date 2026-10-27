#!/usr/bin/env bash
# export.sh -- the Ghidra route's export: decompile listed functions into the pipeline's JSONL (GfxExport.java),
# with each function's convention, purge and float return set from the game's code first (conv_facts.py).
#
#   GHIDRA=/path/to/ghidra PROJ=/path/to/project-dir ./export.sh ADDRESSES.txt OUT.jsonl
#
# PROJ holds a Ghidra project `ffxi` into which FFXiMain_unpacked.dll was imported and analysed once:
#   $GHIDRA/support/analyzeHeadless $PROJ ffxi -import $FFXI_DLL
# (about 20 minutes, 4 cores); GHIDRA_PROGRAM names the program in the project when it is not FFXI_DLL's basename.
# Names: the tree's (tree/placement.json symbols, `::` as `_`), so the text reads
# like the IDA route's; they are cosmetic, every object is bound by address (hx2cpp).
# OUT.jsonl goes to run.sh: `./run.sh translate|cases|gate OUT.jsonl RUN`.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../.."
GHIDRA=${GHIDRA:?set GHIDRA to the Ghidra install (the directory with support/analyzeHeadless)}
PROJ=${PROJ:?set PROJ to the Ghidra project directory}
ADDRS=$(realpath "${1:?addresses file}"); OUT=$(realpath -m "${2:?output jsonl}")
W=$(mktemp -d)
python3 "$H/conv_facts.py" "$W/conv.txt" >/dev/null
# XI_RETW: a file of addresses to export returning a 4-byte value in EAX where the prototype returns nothing
# (GfxExport.java `retw`): the translation hands back what the original leaves in EAX, which the gate compares
# imports by ordinal only, named by the SDK's import library (data/import_libs.json, import_libs.py)
python3 -c 'import json,sys; [print("ordinal", k, v) for k, v in json.load(open(sys.argv[1]))["ordinals"].items()]' \
  "$ROOT/data/import_libs.json" >> "$W/conv.txt"
if [ -n "${XI_RETW:-}" ]; then sed -e 's/^\(0x[0-9A-Fa-f]*\).*/\1 retw 4/' "$XI_RETW" >> "$W/conv.txt"; fi
python3 - "$ROOT/tree/placement.json" "$W/names.json" <<'EOF'
import json, sys
p = json.load(open(sys.argv[1]))
json.dump({k: v['symbol'].replace('::', '_') for k, v in p.items() if v.get('symbol')}, open(sys.argv[2], 'w'))
EOF
"$GHIDRA/support/analyzeHeadless" "$PROJ" ffxi -process "${GHIDRA_PROGRAM:-$(basename "${FFXI_DLL:-FFXiMain_unpacked.dll}")}" -noanalysis -readOnly \
    -scriptPath "$H" -postScript GfxExport.java "$ADDRS" "$W/names.json" "$OUT" "$W/conv.txt" > "$W/ghidra.log" 2>&1 \
  || { tail -30 "$W/ghidra.log"; exit 1; }
grep -E "conventions set|signatures committed|exported|ERROR" "$W/ghidra.log" | tail -8
echo "export: $(wc -l < "$OUT") records in $OUT"
rm -rf "$W"
