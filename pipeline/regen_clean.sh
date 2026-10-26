#!/usr/bin/env bash
# regen_clean.sh EXPORT.jsonl[,GHIDRA.jsonl...] GATE.jsonl[,GATE2.jsonl...] OUTDIR -- generate the clean class-shaped tree from the
# gate-passing functions of a Hex-Rays export, build it, and apply the compile repairs
set -u
P="$(cd "$(dirname "$0")" && pwd)"; D="$P/../data"
EXP=${1:?export}; GATE=${2:?gate}; OUT=${3:?outdir}
rm -rf "$OUT" && python3 "$P/hx2clean.py" "$EXP" "$GATE" "$D/tree_manifest.json" "$D/class_model_include" "$OUT" | tail -1
cp "$P"/clean_support/gfx_common.hpp "$P"/clean_support/xi_decomp.hpp "$P/xi_d3d8.hpp" "$P/xi_compat_win.hpp" "$P/xi_ghidra.hpp" "$P/xi_ghidra_shared.hpp" "$OUT/include/"
cp "$P/clean_support/build.sh" "$P/clean_support/README.md" "$P/clean_support/rt.c" "$OUT/"   # the clean tree's runtime: the raw one plus xi_shift (build translation)
(cd "$OUT" && ./build.sh >/dev/null 2>&1); python3 "$P/repair_compile.py" "$OUT" --clean >/dev/null
python3 "$P/source_shape.py" "$OUT" >/dev/null       # decompiler idioms as source, each rewrite proven to change nothing
(cd "$OUT" && ./build.sh 2>&1 | tail -1 | cut -c1-300)
# a function whose unit still fails to compile is left out and the tree generated again without it (drop_failed.py,
# XI_DROP; each listed in left_out.json with the compiler's message), at most three times
for round in 1 2 3; do
    [ -z "$(ls "$OUT/build/err" 2>/dev/null)" ] && break
    DROP="$OUT.drop.json"
    python3 "$P/drop_failed.py" "$OUT" "$DROP.new" "${XI_DROP:-}" && break
    mv "$DROP.new" "$DROP"; export XI_DROP="$DROP"
    echo "regenerating without the functions that do not compile ($(python3 -c "import json;print(len(json.load(open('$DROP'))))"))"
    rm -rf "$OUT" && python3 "$P/hx2clean.py" "$EXP" "$GATE" "$D/tree_manifest.json" "$D/class_model_include" "$OUT" | tail -1
    cp "$P"/clean_support/gfx_common.hpp "$P"/clean_support/xi_decomp.hpp "$P/xi_d3d8.hpp" "$P/xi_compat_win.hpp" "$P/xi_ghidra.hpp" "$P/xi_ghidra_shared.hpp" "$OUT/include/"
    cp "$P/clean_support/build.sh" "$P/clean_support/README.md" "$P/clean_support/rt.c" "$OUT/"
    (cd "$OUT" && ./build.sh >/dev/null 2>&1); python3 "$P/repair_compile.py" "$OUT" --clean >/dev/null
    python3 "$P/source_shape.py" "$OUT" >/dev/null
    (cd "$OUT" && ./build.sh 2>&1 | tail -1 | cut -c1-300)
done
