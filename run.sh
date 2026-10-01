#!/usr/bin/env bash
# run.sh -- the graphics reconstruction pipeline, end to end, from an IDA export.
#
#   FFXI_DLL=/path/to/FFXiMain_unpacked.dll ./run.sh all [EXPORT] [RUN]
#
# Steps (each can be run alone: ./run.sh translate|cases|gate|clean|cleangate|residue ...):
#   translate  Hex-Rays export -> one C++ unit per function (runs/RUN/raw), build, cast repairs
#   cases      gate cases for every translated function
#   gate       equivalence gate, original vs reconstruction (2 shards)
#   clean      class-shaped tree from the passing functions -> runs/RUN/tree
#   cleangate  gate the clean tree exactly as built; annotate each function with its result
#   residue    RESIDUE.md: every graphics function not in the verified tree, with the reason
# The game DLL is read from $FFXI_DLL and is never copied into the repo.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"; P="$ROOT/pipeline"
EXP=$(realpath "${2:-$ROOT/data/exports/gfx_export_final.jsonl}"); RUN=${3:-dev}; W="$ROOT/runs/$RUN"
mkdir -p "$W"
gate2() {  # gate2 DLL CASES OUTPREFIX: two shards in parallel
  python3 -c "import json,sys;c=json.load(open('$2'));json.dump(c[0::2],open('$3_c0.json','w'));json.dump(c[1::2],open('$3_c1.json','w'))"
  for s in 0 1; do python3 "$P/diffemu_gfx.py" "${FFXI_DLL:?set FFXI_DLL}" "$1" "$3_c$s.json" "$3_$s.jsonl" --trials 100 --seed 11 > "$3_$s.log" 2>&1 & done; wait
  cat "$3_0.jsonl" "$3_1.jsonl" > "$3.jsonl"
}
case "$1" in
  translate|all) rm -rf "$W/raw"; python3 "$P/hx2cpp.py" "$EXP" "$W/raw"; "$P/build_raw.sh" "$W/raw" | tail -1
                 python3 "$P/repair_compile.py" "$W/raw" | tail -1; "$P/build_raw.sh" "$W/raw" | tail -1 ;;&
  cases|all)     python3 "$P/gfx_cases.py" "$EXP" "$W/raw/meta.json" "$ROOT/data/protos_fixed.json" "$W/cases_all.json"
                 python3 -c "import json,os;c=[x for x in json.load(open('$W/cases_all.json')) if os.path.exists('$W/raw/obj/fn_%s.o'%x['original'][2:])];json.dump(c,open('$W/cases.json','w'));print(len(c),'gateable')" ;;&
  gate|all)      gate2 "$W/raw/gfx_raw.dll" "$W/cases.json" "$W/gate_raw" ;;&
  clean|all)     "$P/regen_clean.sh" "$EXP" "$W/gate_raw.jsonl" "$W/tree" ;;&
  cleangate|all) python3 "$P/map_clean_cases.py" "$W/tree/build/gfx_recon.dll" "$W/tree/placement.json" "$W/cases.json" "$W/cases_clean_all.json" | tail -1
                 python3 -c "import json;g={json.loads(l)['original'] for l in open('$W/gate_raw.jsonl') if json.loads(l)['verdict']=='PASS'};c=[x for x in json.load(open('$W/cases_clean_all.json')) if x['original'] in g];json.dump(c,open('$W/cases_clean.json','w'))"
                 gate2 "$W/tree/build/gfx_recon.dll" "$W/cases_clean.json" "$W/gate_clean"
                 python3 "$P/annotate_gate.py" "$W/tree" "$W/gate_clean.jsonl" ;;&
  residue|all)   (cd "$W" && python3 "$P/gen_residue.py" "$EXP" "$W/raw/meta.json" "$W/gate_raw.jsonl" "$ROOT/verification/localize_fail.json" "$W/no_byte_returns.json" "$W/gate_clean.jsonl" "$W/RESIDUE.md") ;;
  *) ;;
esac
