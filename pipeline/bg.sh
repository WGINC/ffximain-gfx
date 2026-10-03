#!/usr/bin/env bash
# bg.sh -- run a long pipeline job in the background and stop it safely.
#
#   pipeline/bg.sh start NAME CMD [ARGS...]   runs CMD in its own process group; pid in runs/.bg/NAME.pid,
#                                             output in runs/.bg/NAME.log
#   pipeline/bg.sh stop NAME                  kills that process group (and only it)
#   pipeline/bg.sh status [NAME]              running / finished (exit code)
#   pipeline/bg.sh wait NAME                  blocks until it finishes, prints the exit code
#
# Stopping by process group id, not by `pkill -f PATTERN`: a pattern also matches the shell that typed it
# (the shell's own command line contains the pattern), which killed the caller twice on 2 Oct 2026.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; D="$ROOT/runs/.bg"; mkdir -p "$D"
cmd="${1:-}"; name="${2:-}"
alive() { [ -f "$D/$1.pid" ] && kill -0 "$(cat "$D/$1.pid")" 2>/dev/null; }
case "$cmd" in
  start)
    [ -n "$name" ] && [ $# -ge 3 ] || { echo "usage: bg.sh start NAME CMD..." >&2; exit 2; }
    alive "$name" && { echo "$name is already running (pid $(cat "$D/$name.pid"))" >&2; exit 1; }
    shift 2; rm -f "$D/$name.exit"
    setsid bash -c '"$@" > "$0.log" 2>&1; echo $? > "$0.exit"' "$D/$name" "$@" < /dev/null &
    echo $! > "$D/$name.pid"; echo "$name started (pgid $!), log $D/$name.log" ;;
  stop)
    alive "$name" || { echo "$name is not running"; exit 0; }
    pg="$(cat "$D/$name.pid")"; kill -TERM -- "-$pg" 2>/dev/null
    for _ in 1 2 3 4 5 6 7 8 9 10; do kill -0 "$pg" 2>/dev/null || break; sleep 0.5; done
    kill -KILL -- "-$pg" 2>/dev/null; echo "$name stopped" ;;
  status)
    for f in "$D"/${name:-*}.pid; do
      [ -f "$f" ] || continue; n="$(basename "$f" .pid)"
      if alive "$n"; then echo "$n: running"; elif [ -f "$D/$n.exit" ]; then echo "$n: finished, exit $(cat "$D/$n.exit")"
      else echo "$n: gone (stopped)"; fi
    done ;;
  wait)
    while alive "$name"; do sleep 2; done; cat "$D/$name.exit" 2>/dev/null || echo "stopped" ;;
  *) sed -n '2,10p' "$0"; exit 2 ;;
esac
