#!/usr/bin/env bash
# make_check.sh OUT.zip -- the device check (13 Nov): t8.exe, d3d8on11 (kit/d3d8on11/dist), 600 random cases in four files
# (random_cases.py seed 7) and run_check.bat, which draws every scene and case through Direct3D 11 and Direct3D 12 on
# the machine it runs on. cmpcheck.py DIR compares the results it returns (unzipped into DIR).
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; OUT="$(realpath -m "$1")"; T="$(mktemp -d)"; P="$T/d3d12_check"; mkdir -p "$P"
cp "$H/../t8.exe" "$H/../../dist/d3d8.dll" "$H/../../dist/d8x.exe" "$H/run_check.bat" "$H/run_variants.bat" "$H/run_formats.bat" "$P/"
python3 "$H/../random_cases.py" 7 600 | python3 -c '
import sys; t = sys.stdin.read(); b = t.split("case "); pre = b[0]; cs = ["case " + x for x in b[1:]]
for i in range(4): open(sys.argv[1] + "/cases%d.txt" % (i + 1), "w").write(pre + "".join(cs[i * 150:(i + 1) * 150]))' "$P"
cat > "$P/README.txt" <<'TXT'
d3d8on11's picture check (13 Nov). Close the game first. Put this folder anywhere (the desktop is fine) and run
run_formats.bat (the third round, under a minute: Direct3D 12's texture formats; send d3d12_formats_results.zip), or
run_variants.bat (the second round: Direct3D 12 six ways; send d3d12_variants_results.zip), or run_check.bat (the first). About 3 to 5 minutes; small windows open and close by themselves. Then send d3d12_check_results.zip
(made in this folder). It draws d3d8on11's test scenes and 600 cases of the render states FFXI sets, through Direct3D 11
and through Direct3D 12, so that whatever Direct3D 12 draws differently on this machine shows up exactly.
TXT
sed -i 's/\r*$/\r/' "$P/README.txt" "$P/run_check.bat" "$P/run_variants.bat" "$P/run_formats.bat"
rm -f "$OUT"; (cd "$T" && zip -qr "$OUT" d3d12_check); rm -rf "$T"; echo "wrote $OUT"
