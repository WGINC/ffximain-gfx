#!/usr/bin/env bash
# setup_vc6.sh -- the game's compiler, assembled from the owner's Visual Studio 6 Enterprise archive (en_vs6_ent.zip:
# the three CD images, en_vs6_sp6.exe, vcpp5.exe). Like the game DLL it never enters the repository: point VC6_ZIP at
# a local copy and VC6 at a folder outside the repo.
#
#   VC6_ZIP=/path/en_vs6_ent.zip VC6=/path/vc6 pipeline/vc6/setup_vc6.sh
#
# Which compiler built FFXiMain.dll is read from the DLL itself: its Rich header lists, per object, the @comp.id the
# compiler stamped (prodid << 16 | build). 1,007 objects carry 48/49 build 9044 and 189 carry 10/11 build 9782. A
# test compile reads the same stamp from this toolchain's objects (match.py --compid): Visual C++ 6.0 SP6's front ends
# (cl.exe 12.0.8804, c1.dll / c1xx.dll 12.0.9782) with the Processor Pack's code generator (vcpp5.exe: c2.dll
# 13.0.9044): which @comp.id they stamp is what match.py --compid prints (docs/RECOMPILE.md has the results).
# Folders made: $VC6/BIN (SP6's cl, c1, c1xx, link; the Processor Pack's c2; the CD's mspdb60 and the rest),
# $VC6/INCLUDE, $VC6/LIB. (SP6's front ends refuse the CD's c2.dll, "C1900: Il mismatch", and this archive's SP6 holds
# no c2.dll of its own, so the 10/11 build 9782 objects have no toolchain here: docs/RECOMPILE.md.) Needs: unzip, cabextract, python3 with pycdlib (pip install pycdlib), wine to run it.
set -euo pipefail
: "${VC6_ZIP:?VC6_ZIP=en_vs6_ent.zip}"; : "${VC6:?VC6=the folder to build the toolchain in}"
H="$(cd "$(dirname "$0")" && pwd)"
W="$VC6/.work"; rm -rf "$W"; mkdir -p "$W" "$VC6"
unzip -o -q "$VC6_ZIP" en_vs6_ent_cd1.iso en_vs6_sp6.exe vcpp5.exe -d "$W"
python3 -I - "$W/en_vs6_ent_cd1.iso" "$W/cd1" <<'PY'
import os, sys, pycdlib
iso = pycdlib.PyCdlib(); iso.open(sys.argv[1]); out = sys.argv[2]
fa = iso.get_joliet_facade() if iso.has_joliet() else iso.get_iso9660_facade()
key = 'joliet_path' if iso.has_joliet() else 'iso_path'
for root in ('/VC98', '/COMMON/MSDEV98/BIN'):
    for d, _, files in fa.walk(root):
        for f in files:
            p = d.rstrip('/') + '/' + f; dst = os.path.join(out, p.lstrip('/'))
            os.makedirs(os.path.dirname(dst), exist_ok=True); fa.get_file_from_iso(dst, **{key: p})
PY
(cd "$W" && mkdir -p sp6 pp && cabextract -q -d sp6 en_vs6_sp6.exe && for c in sp6/*.cab sp6/*.CAB; do [ -e "$c" ] && cabextract -q -L -d sp6/x "$c" >/dev/null 2>&1 || true; done
              cabextract -q -d pp vcpp5.exe)
rm -rf "$VC6/BIN" "$VC6/INCLUDE" "$VC6/LIB"
# one file per name: the CD's are upper case, SP6's and the Processor Pack's lower case; a replaced one is removed so
# that neither Wine's case-insensitive lookup nor a reader can pick the other
put() { rm -f "$2"/"$(basename "$1" | tr a-z A-Z)" "$2"/"$(basename "$1" | tr A-Z a-z)"; cp "$1" "$2/$(basename "$1" | tr A-Z a-z)"; }
cp -r "$W/cd1/VC98/BIN" "$VC6/BIN"; cp "$W/cd1/COMMON/MSDEV98/BIN/MSPDB60.DLL" "$VC6/BIN/"
for f in cl.exe c1.dll c1xx.dll link.exe; do put "$W/sp6/x/vc98/bin/$f" "$VC6/BIN"; done
put "$W/pp/c2.dll" "$VC6/BIN"
cp -r "$W/cd1/VC98/INCLUDE" "$VC6/INCLUDE"; cp -r "$W/cd1/VC98/LIB" "$VC6/LIB"
rm -rf "$W"
python3 "$H/match.py" --compid
