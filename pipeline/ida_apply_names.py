"""ida_apply_names.py -- push data/names_pending_idb.json into the IDB (run inside IDA, File > Script file,
or apply(path) from the MCP bridge).

Each entry renames one address and appends a comment with its confidence and evidence. An address whose
current name is not the expected placeholder (renamed in the IDB since) is reported and left alone.
After applying, re-export: the names then come from the IDB and the entries can be removed from the file.

apply_types() does the same for data/type_fixes.json (corrected prototypes), with the evidence as a comment.
"""
import json, os
import idc, ida_name, ida_bytes

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT = os.path.join(HERE, '..', 'data', 'names_pending_idb.json')
TAG = '[name %s 2026-10-02, %s confidence, from the code and the IDA listing]'


def apply(path=DEFAULT, dry_run=False):
    done, skipped = 0, []
    for k, v in json.load(open(path, encoding='utf-8')).items():
        ea = int(k, 16)
        cur = ida_name.get_name(ea)
        if cur == v['name']:
            continue
        if cur != v['old']:
            skipped.append((k, cur, v['name']))
            continue
        if dry_run:
            done += 1
            continue
        if not ida_name.set_name(ea, v['name'], ida_name.SN_NOWARN | ida_name.SN_NOCHECK):
            skipped.append((k, cur, v['name']))
            continue
        note = '%s %s' % (TAG % (v['name'], v['confidence']), v['evidence'])
        func = idc.get_func_attr(ea, idc.FUNCATTR_START) == ea
        old = (idc.get_func_cmt(ea, 0) if func else idc.get_cmt(ea, 0)) or ''
        if note not in old:
            new = (old + '\n' + note).strip()
            idc.set_func_cmt(ea, new, 0) if func else idc.set_cmt(ea, new, 0)
        done += 1
    print('ida_apply_names: %d applied, %d skipped' % (done, len(skipped)))
    for s in skipped:
        print('  skipped %s: IDB has %r, wanted %r' % s)
    return done, skipped


def apply_types(path=os.path.join(HERE, '..', 'data', 'type_fixes.json'), dry_run=False):
    done, failed = 0, []
    for k, v in json.load(open(path, encoding='utf-8')).items():
        ea = int(k, 16)
        nm = ida_name.get_name(ea) or 'f'
        t = v['type']
        if '(' in t: decl = t.replace('(', ' %s(' % nm, 1) + ';'                 # function prototype
        elif '[' in t: decl = t.replace('[', ' %s[' % nm, 1) + ';'               # array data
        else: decl = '%s %s;' % (t, nm)
        if dry_run:
            done += 1
            continue
        if not idc.SetType(ea, decl):
            failed.append((k, decl))
            continue
        note = '[type 2026-10-02] %s %s' % (v['why'], v['evidence'])
        func = idc.get_func_attr(ea, idc.FUNCATTR_START) == ea
        old = (idc.get_func_cmt(ea, 0) if func else idc.get_cmt(ea, 0)) or ''
        if note not in old:
            new = (old + '\n' + note).strip()
            idc.set_func_cmt(ea, new, 0) if func else idc.set_cmt(ea, new, 0)
        done += 1
    print('ida_apply_names: %d prototypes applied, %d failed' % (done, len(failed)))
    for f in failed:
        print('  failed %s: %s' % f)
    return done, failed


def apply_sp(path=os.path.join(HERE, '..', 'data', 'ida_sp_fixes.json'), dry_run=False):
    # data/ida_sp_fixes.json: the real SP change at instructions where IDA's analysis is wrong
    import ida_frame
    done = 0
    for k, v in json.load(open(path, encoding='utf-8'))['fixes'].items():
        ea = int(k, 16)
        if not dry_run:
            # the SP change takes effect after the instruction; for a call, positive = bytes popped
            ida_frame.add_user_stkpnt(ea + idc.get_item_size(ea), -v['delta'])
            idc.set_cmt(ea, '[sp fix 2026-10-02] ' + v['why'], 0)
        done += 1
    print('ida_apply_names: %d SP corrections applied' % done)
    return done


if __name__ == '__main__':
    apply()
    apply_types()
    apply_sp()
