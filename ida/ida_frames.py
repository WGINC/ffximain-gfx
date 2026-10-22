# ida_frames.py -- IDA's stack frame layout of every tree function (run inside IDA; read-only: the IDB is not changed).
# requests/03_frames.md. IDA derives a frame from the code's own stack accesses, so its offsets and sizes are what the
# MSVC byte matching wants; its names and types are reported only with their origin.
#
# Input: tree/src/*.cpp ("// FUNCTION: FFXIMAIN 0x........" lines).
# Output: data/ida_frames.json, one entry per such address, null when IDA has no frame (or no function starts there):
#   {"frsize", "frregs", "argsize", "fpd"   func_t fields: locals, saved registers, purged argument bytes, frame pointer delta
#    "ebp_frame": FUNC_FRAME,
#    "members": [{"soff": the member's offset in IDA's frame (0 = lowest local), "size", "name",
#                 "entry_off": soff - (frsize + frregs): the byte offset from ESP at function entry, the return
#                              address being at 0 (locals negative, arguments from +4),
#                 "type": the member's type as IDA prints it, "type_user_set": null (see below),
#                 "default_name": the name has IDA's own form, (var|arg)_[s]HEX}]}
#   plus "_doc". The saved-register and return-address pseudo-members are left out; they locate entry_off (the return
#   address is at soff frsize+frregs, which is checked for every function and counted in "_doc").
# type_user_set is null throughout: the frame struct's members carry no flag saying whether a type was set by hand, and
# default_name only says the name has IDA's form, not that nobody typed it.
#
# Run: File > Script file... > ida_frames.py  (IDA 9.x)
import datetime, glob, json, os, re
import idaapi, idc, ida_frame, ida_funcs, ida_kernwin, ida_typeinf

try:
    REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
except NameError:
    REPO = globals().get('REPO') or os.getcwd()
TREE = os.path.join(REPO, 'tree', 'src', '*.cpp')
OUT = os.path.join(REPO, 'data', 'ida_frames.json')
DEFAULT_NAME = re.compile(r'(var|arg)_s?[0-9A-F]+')


def hx(ea):
    return '0x%08X' % ea


def pr(tif):
    s = ida_typeinf.print_tinfo('', 0, 0, ida_typeinf.PRTYPE_1LINE, tif, '', '')
    return s if s is not None else str(tif)


def tree_funcs():
    got = set()
    for p in glob.glob(TREE):
        for m in re.finditer(r'// FUNCTION: FFXIMAIN (0x[0-9A-Fa-f]+)', open(p, encoding='utf-8', errors='replace').read()):
            got.add(int(m.group(1), 16))
    return sorted(got)


def frame_of(ea, st):
    f = ida_funcs.get_func(ea)
    if f is None or f.start_ea != ea:
        st['not_function_start'].append(hx(ea)); return None
    tif = ida_typeinf.tinfo_t()
    if not ida_frame.get_func_frame(tif, f):
        st['no_frame'].append(hx(ea)); return None
    base = f.frsize + f.frregs
    ret = None; members = []
    for i in range(tif.get_udt_nmembers()):
        u = tif.get_udm(i)[1]
        so = u.offset // 8
        if u.is_retaddr(): ret = so; continue
        if u.is_savregs(): continue
        members.append({'soff': so, 'size': u.size // 8, 'name': u.name, 'entry_off': so - base, 'type': pr(u.type),
                        'type_user_set': None, 'default_name': bool(DEFAULT_NAME.fullmatch(u.name))})
        if u.is_gap(): st['gap_members'] += 1
    st['retaddr_at_frsize_frregs' if ret == base else 'retaddr_elsewhere_or_missing'] += 1
    if ret != base: st['retaddr_exceptions'].append(hx(ea))
    members.sort(key=lambda m: m['soff'])
    ebp = bool(f.flags & ida_funcs.FUNC_FRAME)
    st['ebp_frames'] += ebp; st['fpd_nonzero'] += bool(f.fpd); st['members'] += len(members)
    return {'frsize': f.frsize, 'frregs': f.frregs, 'argsize': f.argsize, 'fpd': f.fpd, 'ebp_frame': ebp,
            'members': members}


def main():
    addrs = tree_funcs()
    st = {'not_function_start': [], 'no_frame': [], 'gap_members': 0, 'retaddr_at_frsize_frregs': 0,
          'retaddr_elsewhere_or_missing': 0, 'retaddr_exceptions': [], 'ebp_frames': 0, 'fpd_nonzero': 0, 'members': 0}
    out = {hx(ea): frame_of(ea, st) for ea in addrs}
    doc = {'ida_version': ida_kernwin.get_kernel_version(), 'idb': os.path.basename(idc.get_idb_path()),
           'date': datetime.date.today().isoformat(), 'functions': len(addrs),
           'with_frame': sum(1 for v in out.values() if v is not None), 'null_frame': len(st['no_frame']) + len(st['not_function_start']),
           'no_frame': st['no_frame'], 'not_function_start': st['not_function_start'],
           'members': st['members'], 'ebp_frames': st['ebp_frames'], 'fpd_nonzero': st['fpd_nonzero'],
           'gap_members': st['gap_members'],
           'entry_off': 'soff - (frsize + frregs). IDA lays a frame out as locals [0, frsize), saved registers '
                        '[frsize, frsize+frregs), the return address at frsize+frregs, then arguments; ESP at '
                        'function entry points at the return address, so entry_off = soff minus that offset.',
           'entry_off_check': {'retaddr_at_frsize_plus_frregs': st['retaddr_at_frsize_frregs'],
                               'retaddr_elsewhere_or_missing': st['retaddr_elsewhere_or_missing'],
                               'exceptions': st['retaddr_exceptions']},
           'default_name': 'the name matches (var|arg)_s?HEX, the form IDA generates; it does not say nobody renamed '
                           'a member to such a name',
           'type_user_set': 'null for every member: the frame struct carries no such flag',
           'source': 'ida/ida_frames.py over the FUNCTION lines of tree/src/*.cpp'}
    res = {'_doc': doc}
    res.update(out)
    with open(OUT, 'w') as fh:
        json.dump(res, fh, indent=0, sort_keys=False)
    print('%d functions, %d with a frame, %d members -> %s' % (len(addrs), doc['with_frame'], doc['members'], OUT))
    return doc


if __name__ == '__main__':
    main()
