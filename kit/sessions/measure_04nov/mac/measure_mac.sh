#!/bin/bash
# measure_mac.sh -- the measurement session's CrossOver part (../README.md, "CrossOver on the Mac"). Run in Terminal:
#
#     bash measure_mac.sh BOTTLE PHASE [LABEL]
#
#   BOTTLE  the CrossOver bottle the game is installed in (its name in CrossOver)
#   PHASE   W1_clean, W2_where or W3_deep (the same settings as on Windows)
#   LABEL   what the bottle's graphics are set to for this run, in your words (e.g. wined3d, dxvk, d3dmetal,
#           dgvoodoo+d3dmetal); it only names the results folder
#
# It records what the Mac itself shows while the game runs (Wine's processes and their CPU, context switches and
# system calls, where the Mac's own code spends its time), copies the phase's settings into the game's folder in the
# bottle, starts xiload in the bottle (it loads the kit into pol.exe), starts the game, and at the end copies the
# kit's log folder out of the bottle. Results: ~/Desktop/ffxi_measure_<date>_<PHASE>_<LABEL>.zip
# Nothing here changes CrossOver's or the bottle's settings, and nothing needs sudo except the optional powermetrics.
set -u
BOTTLE="${1:-}"; PHASE="${2:-}"; LABEL="${3:-default}"
HERE="$(cd "$(dirname "$0")" && pwd)"; SESSION="$(cd "$HERE/.." && pwd)"
if [ -z "$BOTTLE" ] || [ ! -f "$SESSION/$PHASE/xikit.ini" ]; then
  echo "usage: bash measure_mac.sh BOTTLE W1_clean|W2_where|W3_deep [LABEL]"; exit 2; fi
CXAPP="/Applications/CrossOver.app"
CXBIN="$CXAPP/Contents/SharedSupport/CrossOver/bin"
BROOT="$HOME/Library/Application Support/CrossOver/Bottles/$BOTTLE"
[ -d "$BROOT" ] || { echo "no bottle named $BOTTLE in $HOME/Library/Application Support/CrossOver/Bottles"; ls "$HOME/Library/Application Support/CrossOver/Bottles"; exit 1; }
# the game's folder in the bottle: where pol.exe is
POLEXE="$(find "$BROOT/drive_c" -maxdepth 7 -iname pol.exe 2>/dev/null | head -1)"; POLDIR="$(dirname "$POLEXE")"
[ -f "$POLDIR/pol.exe" ] || { echo "pol.exe not found in the bottle"; exit 1; }
[ -f "$POLDIR/xikit.dll" ] || { echo "xikit.dll is not next to pol.exe ($POLDIR): copy the kit's files there first (README.md)"; exit 1; }
TS="$(date +%Y%m%d_%H%M%S)"; OUT="$HOME/Desktop/ffxi_measure_${TS}_${PHASE}_${LABEL}"; mkdir -p "$OUT"
log() { echo "$(date +%H:%M:%S) $*" | tee -a "$OUT/measure_mac.log"; }
# the Wine process that runs pol.exe: the busiest process whose command line names pol.exe, not CrossOver's wrapper
polpid() { ps -axo pid=,pcpu=,command= | grep -i 'pol\.exe' | grep -v -e grep -e '/bin/wine ' -e 'measure_mac' | sort -k2 -nr | awk 'NR==1{print $1}'; }
log "bottle $BOTTLE, phase $PHASE, label $LABEL; game folder $POLDIR"

# ---- the machine, macOS, CrossOver, the bottle (once)
{
  echo "== sw_vers"; sw_vers
  echo "== uname"; uname -a
  echo "== sysctl"; sysctl hw.model hw.memsize hw.ncpu hw.perflevel0.logicalcpu hw.perflevel1.logicalcpu machdep.cpu.brand_string kern.osversion 2>/dev/null
  echo "== pmset (power: low power mode, on battery?)"; pmset -g; pmset -g batt
  echo "== thermal"; pmset -g therm 2>/dev/null
  echo "== CrossOver"; /usr/libexec/PlistBuddy -c 'Print :CFBundleShortVersionString' "$CXAPP/Contents/Info.plist" 2>/dev/null
  echo "== the wine binaries (x86_64 under Rosetta, or arm64)"; for f in "$CXAPP/Contents/SharedSupport/CrossOver/CrossOver-Hosted Application/wine"* "$CXAPP/Contents/SharedSupport/CrossOver/lib/wine/x86_64-unix/"*.so; do [ -e "$f" ] && file "$f"; done 2>/dev/null | head -20
  echo "== Rosetta"; /usr/bin/pgrep -q oahd && echo "oahd (Rosetta) running" || echo "oahd not running"
} > "$OUT/mac_system.txt" 2>&1
system_profiler SPHardwareDataType SPDisplaysDataType SPPowerDataType > "$OUT/mac_profiler.txt" 2>&1
{
  echo "== cxbottle.conf"; cat "$BROOT/cxbottle.conf" 2>/dev/null
  for r in user.reg system.reg; do
    echo "== $r: Direct3D, DllOverrides, Mac Driver, DXVK, D3DMetal, MSync/ESync, AppDefaults"
    awk 'BEGIN{p=0} /^\[/{p=($0 ~ /Direct3D|DllOverrides|Mac Driver|DXVK|D3DM|AppDefaults|CrossOver|Wine\\\\X11|DirectInput|DirectSound|Explorer\\\\Desktops/)} p' "$BROOT/$r" 2>/dev/null
  done
  echo "== the bottle's Direct3D DLLs (which implementation they are: size and version)"
  for d in d3d8 d3d9 d3d10core d3d11 d3d12 dxgi d3dcompiler_47 ddraw opengl32 vulkan-1 winemetal; do
    for p in "$BROOT/drive_c/windows/syswow64/$d.dll" "$BROOT/drive_c/windows/system32/$d.dll" "$POLDIR/$d.dll"; do
      [ -f "$p" ] && echo "$(stat -f '%z %Sm' "$p")  $p  $(strings -el "$p" 2>/dev/null | grep -m1 -iE 'dxvk|dgvoodoo|wine|d3dmetal|FileVersion' )"
    done
  done
  echo "== the game folder"; ls -la "$POLDIR" | head -80
  [ -f "$POLDIR/dgVoodoo.conf" ] && { echo "== dgVoodoo.conf"; cat "$POLDIR/dgVoodoo.conf"; }
} > "$OUT/bottle.txt" 2>&1
log "system and bottle described: mac_system.txt, mac_profiler.txt, bottle.txt"

# ---- the phase's settings into the game folder
cp "$SESSION/$PHASE/xikit.ini" "$POLDIR/xikit.ini"
[ -f "$SESSION/$PHASE/speedfix.ini" ] && cp "$SESSION/$PHASE/speedfix.ini" "$POLDIR/speedfix.ini"
printf '%s %s\r\n' "$PHASE" "$LABEL" > "$POLDIR/xikit_phase.txt"
before="$(ls -1 "$POLDIR/xikit_logs" 2>/dev/null | sort)"

# ---- xiload and the game, in the bottle (the environment below reaches every Wine process they start)
export WINEDEBUG="${WINEDEBUG:--all}"
[ -n "${HUD:-}" ] && export MTL_HUD_ENABLED=1 DXVK_HUD=fps,frametimes,gpuload
WINE="$CXBIN/wine"
log "starting xiload in the bottle"
# (no --once: in CrossOver the game runs in a second pol.exe the first one starts; the owner's first M1 run loaded the
#  kit into the first one only. xiload keeps watching and loads it into each new pol.exe; it is stopped at the end)
"$WINE" --bottle "$BOTTLE" "$POLDIR/xiload.exe" > "$OUT/xiload_console.txt" 2>&1 &
XLPID=$!
sleep 3
if [ -z "${NOSTART:-}" ]; then
  log "starting the game (pol.exe) in the bottle; log in as usual"
  ( cd "$POLDIR" && "$WINE" --bottle "$BOTTLE" "$POLDIR/pol.exe" > "$OUT/pol_console.txt" 2>&1 ) &
else log "NOSTART=1: start the game from CrossOver yourself now"; fi
for i in $(seq 1 300); do POLPID="$(polpid)"; [ -n "$POLPID" ] && break; sleep 1; done
log "pol.exe is process ${POLPID:-not found}"

echo
echo ">>> Log in and go to the first step's place (README.md). Press Enter here when you are there (do not press"
echo ">>> Ctrl+Alt+F7 yet: press it in the game at the start of each step, as on Windows)."
read -r _
log "recording: top every second (CPU, threads, context switches, system calls, faults of every process)"
top -l 0 -s 1 -o cpu -n 25 -stats pid,command,cpu,threads,csw,sysbsd,sysmach,faults,pageins,mem,state > "$OUT/top.txt" 2>&1 &
TOPPID=$!
PM=""
if [ -n "${POWER:-}" ]; then
  log "powermetrics (sudo): CPU and GPU power and residency, thermal, each second"
  sudo powermetrics --samplers cpu_power,gpu_power,thermal,tasks --show-process-gpu --show-process-energy -i 1000 > "$OUT/powermetrics.txt" 2>&1 &
  PM=$!
fi
sleep 20
WS="$(pgrep -x wineserver | head -1)"; POLPID="$(polpid)"
ps -axo pid,ppid,pcpu,pmem,rss,vsz,state,comm | grep -iE 'wine|pol|xiload|cx' | grep -v grep > "$OUT/ps.txt"
log "sampling the Mac's own stacks: pol.exe ($POLPID) and wineserver ($WS), 15 s each"
[ -n "$POLPID" ] && sample "$POLPID" 15 1 -mayDie -file "$OUT/sample_pol_1.txt" >/dev/null 2>&1
[ -n "$WS" ] && sample "$WS" 15 1 -mayDie -file "$OUT/sample_wineserver_1.txt" >/dev/null 2>&1
[ -n "$POLPID" ] && { ps -M -p "$POLPID" > "$OUT/threads_pol_1.txt" 2>&1; vmmap --summary "$POLPID" > "$OUT/vmmap_pol.txt" 2>&1; }
echo
echo ">>> Go on with the steps. Press Enter here when you reach the busy step (the fight or the crowd)."
read -r _
POLPID="$(polpid)"; WS="$(pgrep -x wineserver | head -1)"
log "sampling again in the busy step"
[ -n "$POLPID" ] && sample "$POLPID" 15 1 -mayDie -file "$OUT/sample_pol_2.txt" >/dev/null 2>&1
[ -n "$WS" ] && sample "$WS" 10 1 -mayDie -file "$OUT/sample_wineserver_2.txt" >/dev/null 2>&1
[ -n "$POLPID" ] && ps -M -p "$POLPID" > "$OUT/threads_pol_2.txt" 2>&1
echo
echo ">>> Finish the steps and close the game normally. This waits for it to close."
while [ -n "$(polpid)" ]; do sleep 2; done
kill "$TOPPID" 2>/dev/null; kill "$XLPID" 2>/dev/null; pkill -f 'xiload\.exe' 2>/dev/null; [ -n "$PM" ] && sudo kill "$PM" 2>/dev/null
sleep 2
after="$(ls -1 "$POLDIR/xikit_logs" 2>/dev/null | sort)"
new="$(comm -13 <(echo "$before") <(echo "$after"))"
for n in $new; do cp -R "$POLDIR/xikit_logs/$n" "$OUT/"; done
for f in xiload.log dgVoodoo.conf d3d8on11.log d3d8on11.ini VERSION.txt; do [ -f "$POLDIR/$f" ] && cp "$POLDIR/$f" "$OUT/"; done
log "copied the kit's log folders: ${new:-none (was xikit.dll loaded? see xiload.log and xiload_console.txt)}"
( cd "$(dirname "$OUT")" && zip -qr "$(basename "$OUT").zip" "$(basename "$OUT")" )
log "results: $OUT.zip"
