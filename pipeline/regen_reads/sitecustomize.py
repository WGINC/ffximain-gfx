"""Loaded by every Python process regen_facts.py starts (PYTHONPATH): records what the process reads.

Each line appended to $REGEN_READS is one event, tab-separated:
  r <abs path>   a file opened for reading (open, io.open_code -- imports too -- os.open without write flags)
  w <abs path>   a file opened for writing
  l <abs path>   a directory listed (os.listdir, os.scandir) by the tool's own code
  L <abs path>   a directory listed by Python's import system while finding a module (frames of importlib)
  i <name>       a module imported (the `import` audit event): with L, only entries an import could resolve to
                 matter, so a new file in pipeline/ is not a change for a tool that never imports its name
  x <what>       a process or program started: its reads are not seen, so the tool's read set is incomplete
regen_facts.py decides from these alone whether a tool must run again; nothing about a tool is declared by hand.
The system's own sitecustomize (if any) is run after the hook is in place.
"""
import os, sys

_log = os.environ.get('REGEN_READS')
if _log:
    _fd = os.open(_log, os.O_WRONLY | os.O_APPEND | os.O_CREAT, 0o644)
    _busy = [False]
    _W = os.O_WRONLY | os.O_RDWR | os.O_CREAT | os.O_TRUNC | os.O_APPEND

    def _emit(kind, what):
        os.write(_fd, ('%s\t%s\n' % (kind, what)).encode('utf-8', 'surrogateescape'))

    def _path(p):
        if isinstance(p, int): return None
        if isinstance(p, bytes): p = os.fsdecode(p)
        try: return os.path.abspath(os.fspath(p))
        except Exception: return None

    def _hook(ev, args):
        if _busy[0]: return
        _busy[0] = True
        try:
            if ev == 'open':
                p = _path(args[0])
                if p is None: return
                mode, flags = args[1], args[2]
                if mode is not None: wr = any(c in mode for c in 'wax+')
                else: wr = bool((flags or 0) & _W)
                _emit('w' if wr else 'r', p)
            elif ev in ('os.listdir', 'os.scandir'):
                p = _path(args[0] if args and args[0] is not None else '.')
                if p is not None:
                    f, imp = sys._getframe(1), False
                    while f is not None:
                        if f.f_code.co_filename.startswith('<frozen importlib'): imp = True; break
                        f = f.f_back
                    _emit('L' if imp else 'l', p)
            elif ev == 'import':
                _emit('i', args[0])
            elif ev in ('subprocess.Popen', 'os.system', 'os.exec', 'os.posix_spawn', 'os.spawn', 'os.fork'):
                _emit('x', ev + ' ' + repr(args[:2])[:200].replace('\t', ' ').replace('\n', ' '))
        finally:
            _busy[0] = False

    sys.addaudithook(_hook)

# the system's sitecustomize, which this module shadows
_here = os.path.dirname(os.path.abspath(__file__))
for _d in sys.path:
    _f = os.path.join(_d or '.', 'sitecustomize.py')
    if os.path.abspath(_d or '.') != _here and os.path.isfile(_f):
        exec(compile(open(_f, 'rb').read(), _f, 'exec'), {'__name__': 'sitecustomize', '__file__': _f})
        break
