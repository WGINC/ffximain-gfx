#!/usr/bin/env bash
# build.sh -- the in-game kit into kit/xikit/dist/ (MinGW-w64 i686; uses $FFXI_DLL for the manifest):
#   gfx_recon_ingame.dll   the clean tree (../../tree) built with XI_INGAME
#   xikit_manifest.txt     make_manifest.py
#   xikit_manifest_<stamp>.txt  manifest_for_build.py, for each other build in builds/ ($FFXI_DLL_<stamp>)
#   xikit.dll              xikit.c + ../d3dprof/d3dprof.c (XIKIT)
#   xikit.ini              the default settings
#   xisamp_arm64.exe, xisamp_x64.exe  the native sampler (../xisamp, sample=2)
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../.."; OUT="$H/dist"; W="$H/build"
mkdir -p "$OUT" "$W"
rm -rf "$W/tree"; cp -r "$ROOT/tree" "$W/tree"; rm -rf "$W/tree/build"
(cd "$W/tree" && XI_INGAME=1 ./build.sh | tail -1)
cp "$W/tree/build/gfx_recon_ingame.dll" "$OUT/"
python3 "$H/close_gaps.py" "$OUT/gfx_recon_ingame.dll"        # Windows refuses gaps between sections (error 193)
python3 "$H/make_manifest.py" "$OUT/gfx_recon_ingame.dll" "$OUT/xikit_manifest.txt" "$W/manifest_report.json" | head -1
python3 "$ROOT/sdk/gen_sdk.py"                                # sdk/include/xikit_game.hpp follows the manifest
# other builds of the game (build translation, docs/KIT.md): a manifest for each builds/<stamp>.json whose unpacked DLL
# is given as $FFXI_DLL_<stamp> (pipeline/unpack_pol.py; game binaries never enter the repository)
for b in "$H"/builds/*.json; do
  [ -e "$b" ] || continue
  st="$(basename "$b" .json)"; var="FFXI_DLL_$st"
  if [ -n "${!var:-}" ]; then python3 "$H/manifest_for_build.py" "$OUT/xikit_manifest.txt" "$b" "${!var}" "$OUT/xikit_manifest_$st.txt"
  else echo "build $st: \$$var not set, no manifest for it"; rm -f "$OUT/xikit_manifest_$st.txt"; fi
done
# the UNRESOLVED call sites, for facts=1: those of the committed gate records, Frame_Render's timer calls in its
# wait, where speedfix's frame_step fork gate stops UNRESOLVED (kit/speedfix/test/test_framestep.py), and
# data/facts_wanted.json (pipeline/facts_wanted.py: the frame set's gate, and the calls through [0x1066941C])
python3 "$ROOT/pipeline/fact_sites.py" sites "$OUT/xikit_facts.txt" 0x10012B28 0x10012B86 0x10012B91 0x10012BCB 0x10012BF5 --from-gates --from-file "$ROOT/data/facts_wanted.json" > "$W/fact_sites.log"
head -1 "$W/fact_sites.log"                                   # (the refused sites, each with why, in build/fact_sites.log)
i686-w64-mingw32-gcc -O2 -Wall -Wno-unused-function -std=gnu99 -DXIKIT -shared -o "$OUT/xikit.dll" \
    "$H/xikit.c" "$ROOT/kit/d3dprof/d3dprof.c" "$ROOT/kit/d3dprof/fcap.c" -static-libgcc -Wl,--kill-at -Wl,--no-insert-timestamp -lwinmm
"$ROOT/kit/xisamp/build.sh" | tail -1                     # the native sampler (sample=2), from the same folder
cp "$ROOT/kit/xisamp/dist/xisamp_arm64.exe" "$ROOT/kit/xisamp/dist/xisamp_x64.exe" "$OUT/"
python3 - "$H/xikit.c" "$OUT/xikit.ini" <<'PY'
import re, sys
s = open(sys.argv[1]).read()
m = re.search(r'static const char DEFAULT_INI\[\] =\n(.*?);\n', s, re.S)
txt = ''.join(eval(x) for x in re.findall(r'"(?:[^"\\]|\\.)*"', m.group(1)))
open(sys.argv[2], 'w', newline='').write(txt)
PY
# the build stamp: source commit (+modified when the working tree differs from it), date, the tree's gate, file hashes;
# xikit.dll logs its first line at start
python3 - "$ROOT" "$OUT" <<'PY'
import datetime, hashlib, json, os, subprocess, sys
root, out = sys.argv[1:3]
git = lambda *a: subprocess.run(['git', '-C', root] + list(a), capture_output=True, text=True).stdout.strip()
rev = git('rev-parse', '--short=12', 'HEAD') or 'unknown'
mod = '+modified' if git('status', '--porcelain', '--untracked-files=no') else ''
g = [json.loads(l)['verdict'] for l in open(os.path.join(root, 'verification', 'gate_clean_final.jsonl'))]
files = sorted(f for f in os.listdir(out) if f != 'VERSION.txt')
with open(os.path.join(out, 'VERSION.txt'), 'w', newline='\r\n') as f:
    f.write('xikit kit, source %s%s, built %s UTC; tree gate %d PASS of %d\n' % (rev, mod,
            datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%d %H:%M'), g.count('PASS'), len(g)))
    for n in files: f.write('sha256 %s  %s\n' % (hashlib.sha256(open(os.path.join(out, n), 'rb').read()).hexdigest(), n))
PY
ls -la "$OUT"
