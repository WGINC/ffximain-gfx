"""ida_fix_reexport.py -- one pass in IDA: apply everything the pipeline holds as data, then re-export.

Run inside IDA 9.x (File > Script file...) on the FFXiMain_unpacked.dll IDB, from a checkout of this repo
(the script finds data/ relative to itself). Save the IDB first (or work on a copy). It
  1. applies data/names_pending_idb.json and data/type_fixes.json (ida_apply_names.apply / apply_types);
  2. types the two viewport globals as D3DVIEWPORT8 (24 bytes; the IDB has them as pointers, which made
     their copies read through the first dword: sub_1011AA40, sub_10162010);
  3. corrects IDA's stack pointer from data/real_sp.json, the depth the original really has at each
     instruction, recorded by running it (pipeline/real_sp.py). Wherever IDA's SP change across a call
     differs from the real one, a user SP change point is set there (what Alt+K does), and the function is
     re-analysed until IDA agrees with the real depth at every recorded instruction. Each correction is
     commented in the IDB and listed in data/exports/ida_sp_report.json;
  4. decompiles every function of data/exports/gfx_export_current.jsonl again and writes
     data/exports/gfx_export_ida_reexport.jsonl (same record format as ida_gfx.export_one).

Then, outside IDA:  ./run.sh all data/exports/gfx_export_ida_reexport.jsonl RUN
Hand patches (data/hx_patches) whose text no longer matches are reported stale and skipped.
"""
import json, os, sys, time
import idc, ida_auto, ida_funcs, ida_frame, ida_typeinf, ida_hexrays

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import ida_apply_names, ida_gfx

DATA = os.path.join(HERE, '..', 'data')
VIEWPORTS = [0x104E1B70, 0x10621904]
VIEWPORT_DECL = 'struct D3DVIEWPORT8 { unsigned int X; unsigned int Y; unsigned int Width; unsigned int Height; float MinZ; float MaxZ; };'


def is_call(ea):
    return idc.print_insn_mnem(ea) == 'call'


def first_disagreement(real):
    """the first call (in address order) across which IDA's SP change differs from the real one:
    (call ea, IDA's change, real change), or None. real: {ea: depth}, depth = bytes below the entry SP."""
    eas = sorted(real)
    for ea in eas:
        nxt = ea + idc.get_item_size(ea)
        if nxt not in real or not is_call(ea) or idc.get_spd(ea) is None or idc.get_spd(nxt) is None: continue
        c_ida = idc.get_spd(nxt) - idc.get_spd(ea)
        c_real = real[ea] - real[nxt]               # IDA's SP falls as the stack grows: a pop is positive
        if c_ida != c_real: return ea, c_ida, c_real
    return None


def mismatches(real):
    return sum(1 for ea, d in real.items() if idc.get_spd(ea) != -d)


def set_change(call, value):
    nxt = call + idc.get_item_size(call)
    ida_frame.del_stkpnt(ida_funcs.get_func(call), nxt)
    ida_frame.add_user_stkpnt(nxt, value)
    ida_funcs.reanalyze_function(ida_funcs.get_func(call)); ida_auto.auto_wait()
    return idc.get_spd(nxt) - idc.get_spd(call)


def fix_sp(path=os.path.join(DATA, 'real_sp.json')):
    report = {}
    for fk, rec in json.load(open(path)).items():
        f = int(fk, 16)
        real = {int(a, 16): d for a, d in rec.items()}
        if not ida_funcs.get_func(f): continue
        before = mismatches(real)
        if not before: continue
        fixes = []
        for _ in range(32):
            bad = first_disagreement(real)
            if not bad: break
            call, c_ida, c_real = bad
            got = set_change(call, c_real)
            if got != c_real:                       # the other sign convention for the stored value
                got = set_change(call, -c_real)
            fixes.append({'call': '0x%08X' % call, 'ida_change_was': c_ida, 'real_change': c_real, 'now': got})
            idc.set_cmt(call, '[sp fix %s] the call changes SP by %d (measured by running the original); IDA had %d'
                        % (time.strftime('%Y-%m-%d'), c_real, c_ida), 0)
            if got != c_real: break                 # could not be corrected: reported below
        ida_hexrays.mark_cfunc_dirty(f)
        after = mismatches(real)
        report[fk] = {'name': idc.get_func_name(f), 'mismatched_before': before, 'mismatched_after': after, 'fixes': fixes}
        print('  SP %s %s: %d instructions disagreed, %d after %d fixes' % (fk, idc.get_func_name(f), before, after, len(fixes)))
    json.dump(report, open(os.path.join(DATA, 'exports', 'ida_sp_report.json'), 'w'), indent=1)
    return report


def main():
    t0 = time.time()
    ida_apply_names.apply()
    ida_apply_names.apply_types()

    if not ida_typeinf.get_named_type(ida_typeinf.get_idati(), 'D3DVIEWPORT8', ida_typeinf.NTF_TYPE):
        idc.parse_decls(VIEWPORT_DECL, 0)
    for g in VIEWPORTS:
        print('  viewport global 0x%X typed D3DVIEWPORT8: %s' % (g, bool(idc.SetType(g, 'D3DVIEWPORT8'))))
    ida_auto.auto_wait()

    rep = fix_sp()
    print('ida_fix_reexport: SP corrected in %d functions, %d still disagree'
          % (len(rep), sum(1 for r in rep.values() if r['mismatched_after'])))

    eas = [json.loads(l)['ea'] for l in open(os.path.join(DATA, 'exports', 'gfx_export_current.jsonl'))]
    out = os.path.join(DATA, 'exports', 'gfx_export_ida_reexport.jsonl')
    skipped = 0
    with open(out, 'w') as f:
        for i, ea in enumerate(eas):
            ida_hexrays.mark_cfunc_dirty(ea)
            rec = ida_gfx.export_one(ea)
            skipped += 'skip' in rec
            f.write(json.dumps(rec) + '\n')
            if i % 50 == 0: print('  exported %d of %d' % (i, len(eas)))
    print('ida_fix_reexport: %d functions written to %s (%d marked skip), %.0f s' % (len(eas), out, skipped, time.time() - t0))


if __name__ == '__main__':
    main()
