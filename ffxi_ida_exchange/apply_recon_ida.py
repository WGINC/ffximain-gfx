"""apply_recon_ida.py -- bring the FFXiMain.dll (2026-09-03) IDB in line with the reconstruction
pipeline, using only changes that were cross-checked against IDA's own analysis (30 Sep 2026).

Run inside IDA with plan.json next to this file. Set DRY_RUN = True to only count.
Take a database snapshot first (the session that wrote this took one:
FFXiMain_unpacked.dll_20260930180806.i64).

Phases
  1  DancingMad corrections (3 names moved/added, stale label removed). Verified in IDA:
     only code storing vtable 0x1032AD20 is 0x10041032 (in 0x10041020); slot 6 of 0x1032B3B8
     is 0x10052ED0; slot 6 of 0x1032B2F8 is 0x10052DF0; 0x10040F50 is not an instruction head.
  2  Remove IDA functions that swallowed a separate function (IDA folded a tail-jumped-to
     function that begins after ret+padding) and tiny FLIRT fragments (`mov al,1; ret` named
     std::codecvt_base::do_always_noconv etc. inside game functions). IDA functions whose start
     the pipeline lacks are KEPT (IDA judged right: D3DX / CRT / import stubs).
  3  Create pipeline functions IDA lacks (with pipeline chunk bounds), and recreate the ones
     removed in phase 2. Skipped: 13 switch tables the pipeline mis-seeded as functions, and
     350 starts IDA keeps as tail chunks (EH funclets / shared tails: both readings defensible).
  4  Extend owners of removed fragments to the pipeline's end.
  5  Structs for the 242 reconstructed classes (pack 1, base class as first member). Existing
     user types (CMoProcessor, CXiSkeletonActor, CXiSkeletonActorRes) are never replaced; a
     tree class with a clashing name is added as <Name>_recon.
  6  Prototypes for functions created here that have no type: pipeline convention, corrected
     by an ECX read-before-write check (pass-through `this` into thiscall callees counts as a
     read; prologue `push ecx` does not; __alloca_probe/__EH_prolog/__SEH_prolog preserve ECX;
     a tail-wrapper that loads ECX itself is not thiscall). Class members get `Class *this`.
     Existing types are never changed.
  7  Names and comments for the 4,254 gate-verified reconstructions. Renames only dummy names
     (sub_/nullsub_/j_sub_/unknown_libname_) and tiny-FLIRT names on class vtable slots; the
     old name goes into the comment. Every one gets a one-line `[recon]` function comment.
"""
import json, os, re, collections
import ida_funcs, ida_bytes, ida_name, ida_typeinf, ida_nalt, ida_auto, ida_gdl, ida_ua, ida_idp, idautils, idc

DRY_RUN = globals().get('DRY_RUN', True)
HERE = globals().get('RECON_DIR', os.path.dirname(os.path.abspath(__file__)) if '__file__' in globals() else '.')
P = json.load(open(os.path.join(HERE, 'plan.json')))
LOG = collections.defaultdict(list)
C = collections.Counter()

def h(x): return int(x, 16)

# ---------------------------------------------------------------- phase 1
DM_FIX = [
    (0x10041020, 'CMoD3mSpecularElem_Ctor', "CMoD3mSpecularElem constructor: calls CMoElem's constructor 0x10044890, stores vtable 0x1032AD20 (the only code in the image that does) and stamps 448 (0x1C0) at +0x176. CORRECTED 2026-09-30 (was 0x10040F50, which is 0xE0 bytes into 0x10040E70 and not an instruction head)."),
    (0x10052ED0, 'CXi471011C8_VT_1032B3B8_Dtor', "Scalar deleting destructor, slot 6 of vtable 0x1032B3B8. Stores vtable 0x1032AD80 (inlined base dtor: suggests this class derives from the 0x1032AD80 class -- a lead, not settled), calls CMoElem's shared cleanup 0x10044970, then operator delete if flags&1. CORRECTED 2026-09-30 (was 0x10052E00, a call inside 0x10052DF0). Gate-verified by diffemu."),
    (0x10052DF0, 'CMoVuDist_ScalarDeletingDtor', "Slot 6 of CMoVuDist's vtable 0x1032B2F8. Destroys the owned buffer member at +0x198 (0x10063C60), then CMoElem's shared cleanup 0x10044970, then operator delete if flags&1. Gate-verified by diffemu."),
]
def phase1():
    if ida_name.get_name(0x10052E00) == 'CXi471011C8_VT_1032B3B8_Dtor':
        LOG['p1_removed_label'].append(['0x10052E00', ida_name.get_name(0x10052E00), idc.get_cmt(0x10052E00, 0)])
        if not DRY_RUN:
            ida_name.set_name(0x10052E00, '', ida_name.SN_NOWARN)
            idc.set_cmt(0x10052E00, '', 0)
        C['p1_label_removed'] += 1
    for ea, nm, cmt in DM_FIX:
        old = ida_name.get_name(ea)
        LOG['p1_named'].append([hex(ea), old, nm])
        if not DRY_RUN:
            if ida_name.get_name_ea(idc.BADADDR, nm) not in (idc.BADADDR, ea):
                ida_name.set_name(ida_name.get_name_ea(idc.BADADDR, nm), '', ida_name.SN_NOWARN)
            ida_name.set_name(ea, nm, ida_name.SN_NOWARN | ida_name.SN_NOCHECK)
            cur = idc.get_func_cmt(ea, 1) or ''
            if 'CORRECTED 2026-09-30' not in cur and 'Gate-verified' not in cur:
                idc.set_func_cmt(ea, (cur + '\n' if cur else '') + cmt, 1)
        C['p1_named'] += 1

# ---------------------------------------------------------------- phases 2-4
def is_code_head(ea):
    f = ida_bytes.get_flags(ea)
    return ida_bytes.is_code(f) and ida_bytes.is_head(f)

def phase2():
    for x in P['delete']:
        ea = h(x); fn = ida_funcs.get_func(ea)
        if not fn or fn.start_ea != ea:
            C['p2_already_gone'] += 1; continue
        LOG['p2_deleted'].append([x, ida_name.get_name(ea), [[hex(s), hex(e)] for s, e in idautils.Chunks(ea)],
                                  idc.get_func_cmt(ea, 0), idc.get_func_cmt(ea, 1), bool(fn.flags & ida_funcs.FUNC_LIB)])
        if not DRY_RUN:
            ida_funcs.del_func(ea)
        C['p2_deleted'] += 1
    frag = set(P['frag'])
    for x in P['frag']:
        # a removed fragment is not a function start: drop its misleading library label
        ea = h(x)
        if x in P['create']: continue
        if not DRY_RUN:
            ida_name.set_name(ea, '', ida_name.SN_NOWARN)
        C['p2_frag_label_cleared'] += 1

def phase3():
    created = []
    for x, chunks in sorted(P['create'].items(), key=lambda kv: h(kv[0])):
        ea = h(x)
        if not DRY_RUN and ida_funcs.get_func(ea) and ida_funcs.get_func(ea).start_ea == ea:
            C['p3_exists'] += 1; continue
        if DRY_RUN:
            fn = ida_funcs.get_func(ea)
            if fn and fn.start_ea == ea and x not in P['delete']:
                C['p3_exists'] += 1; continue
        if not is_code_head(ea):
            if not DRY_RUN:
                ida_ua.create_insn(ea)
            if not is_code_head(ea):
                C['p3_not_code'] += 1; LOG['p3_not_code'].append(x); continue
        main = next(([h(s), h(e)] for s, e in chunks if h(s) <= ea < h(e)), [h(chunks[0][0]), h(chunks[0][1])])
        if DRY_RUN:
            C['p3_would_create'] += 1; created.append(ea); continue
        ok = ida_funcs.add_func(ea, main[1])
        if not ok:
            ok = ida_funcs.add_func(ea)  # let IDA pick the end
            C['p3_created_ida_end' if ok else 'p3_failed'] += 1
            if not ok: LOG['p3_failed'].append(x); continue
        else:
            C['p3_created'] += 1
        created.append(ea)
        fn = ida_funcs.get_func(ea)
        for s, e in chunks:
            s, e = h(s), h(e)
            if s == main[0]: continue
            if ida_funcs.get_func(s) is None:
                if ida_funcs.append_func_tail(fn, s, e): C['p3_tail_added'] += 1
    return created

def phase4():
    for x, (s, e) in P['extend'].items():
        ea = h(x); fn = ida_funcs.get_func(ea)
        if not fn or fn.start_ea != ea:
            C['p4_owner_missing'] += 1; continue
        e = h(e)
        if fn.end_ea >= e: C['p4_already'] += 1; continue
        if not DRY_RUN:
            ok = ida_funcs.set_func_end(ea, e)
            C['p4_extended' if ok else 'p4_failed'] += 1
        else:
            C['p4_would_extend'] += 1

# ---------------------------------------------------------------- phase 5 structs
EXISTING_KEEP = {'CMoProcessor', 'CXiSkeletonActor', 'CXiSkeletonActorRes'}
_til0 = ida_typeinf.get_idati()
for _c in P['class_order']:   # any pre-existing type of the same name is left alone
    _t = ida_typeinf.tinfo_t()
    if not _t.get_named_type(_til0, _c) or _c in EXISTING_KEEP: continue
    try: _ours = 'recon' in (_t.get_type_cmt() or '')
    except Exception: _ours = False
    if not (_t.is_struct() and _ours):
        # a struct from an earlier run of this script is recognised by its comment; anything else is kept
        EXISTING_KEEP.add(_c)
TYPEMAP = {'void**': 'void **', 'uint8_t': 'unsigned __int8', 'uint16_t': 'unsigned __int16',
           'uint32_t': 'unsigned __int32', 'int64_t': '__int64', 'float': 'float'}
def struct_name(c):
    return c + '_recon' if c in EXISTING_KEEP else c
def phase5():
    til = ida_typeinf.get_idati()
    decls = []
    for c in P['class_order']:
        k = P['classes'][c]
        nm = struct_name(c)
        body = []
        if k['base']:
            body.append('  %s base;' % struct_name(k['base']))
        for t, f, arr in k['fields']:
            t = TYPEMAP[t]
            body.append('  %s %s%s;' % (t, f, arr))
        if not body: body.append('  unsigned __int8 _empty[%d];' % k['size'])
        decls.append('struct %s {\n%s\n};' % (nm, '\n'.join(body)))
    src = '#pragma pack(push, 1)\n' + '\n'.join(decls) + '\n#pragma pack(pop)\n'
    if DRY_RUN:
        C['p5_would_declare'] = len(decls); return
    open(os.path.join(HERE, 'recon_structs.h'), 'w').write(src)
    LOG['p5_renamed_for_clash'] = sorted(c for c in P['class_order'] if c in EXISTING_KEEP)
    errs = ida_typeinf.parse_decls(til, src, None, ida_typeinf.HTI_DCL)
    C['p5_parse_errors'] = errs
    for c in P['class_order']:
        t = ida_typeinf.tinfo_t()
        if t.get_named_type(til, struct_name(c)):
            ok = t.get_size() == P['classes'][c]['size']
            try: t.set_type_cmt('recon: tree/include/%s.hpp, sizeof 0x%X from the binary\'s descriptor' % (c, P['classes'][c]['size']))
            except Exception: C['p5_cmt_failed'] += 1
            C['p5_size_ok' if ok else 'p5_size_BAD'] += 1
            if not ok: LOG['p5_size_bad'].append([c, t.get_size(), P['classes'][c]['size']])
        else:
            C['p5_missing'] += 1; LOG['p5_missing'].append(c)

# ---------------------------------------------------------------- phase 6 prototypes
ECX_SAFE_HELPERS = ('__alloca_probe', '_alloca_probe', '__chkstk', '_chkstk', '__EH_prolog', '_EH_prolog', '__SEH_prolog', '_SEH_prolog')
PROTO = {h(k): v for k, v in json.load(open(os.path.join(HERE, 'pipeline_protos.json'))).items()} if os.path.exists(os.path.join(HERE, 'pipeline_protos.json')) else {}
def callee_this(t):
    if t in PROTO: return PROTO[t][0] == 'thiscall'
    return '__thiscall' in (idc.get_type(t) or '')
def ecx_read(fea):
    f = ida_funcs.get_func(fea)
    if not f or f.start_ea != fea: return None
    blocks = {b.start_ea: b for b in ida_gdl.FlowChart(f)}
    work = [(fea, True)]; seen = set()
    while work:
        bea, live = work.pop()
        if (bea, live) in seen or bea not in blocks: continue
        seen.add((bea, live)); b = blocks[bea]; ea = b.start_ea
        while ea < b.end_ea and live:
            insn = ida_ua.insn_t()
            if not ida_ua.decode_insn(insn, ea): break
            mn = insn.get_canon_mnem(); feat = insn.get_canon_feature()
            ops = [idc.print_operand(ea, i).lower() for i in range(3)]
            if mn == 'push' and ops[0] == 'ecx' and ea == fea:
                pass
            elif mn == 'call':
                t = idc.get_operand_value(ea, 0)
                if 'ecx' in ops[0]: return True
                if ida_funcs.get_func(t) and callee_this(t): return True
                if ida_name.get_name(t).lstrip('_') in [x.lstrip('_') for x in ECX_SAFE_HELPERS]:
                    pass
                else:
                    live = False; break
            elif mn == 'jmp' and idc.get_operand_type(ea, 0) in (idc.o_near, idc.o_far):
                t = idc.get_operand_value(ea, 0); tf = ida_funcs.get_func(t)
                if tf and tf.start_ea == t and t != fea and callee_this(t): return True
            else:
                for i, o in enumerate(ops):
                    if not o: continue
                    toks = set(re.split(r'[\[\]\+\*,\-\s:]+', o))
                    if not (toks & {'ecx', 'cx', 'cl', 'ch'}): continue
                    if mn == 'xor' and ops[0] == ops[1]: continue
                    if '[' in o or i > 0 or (feat & ida_idp.CF_USE1): return True
                if ops[0] == 'ecx' and (feat & ida_idp.CF_CHG1): live = False; break
            ea = idc.next_head(ea, b.end_ea)
        for s in b.succs(): work.append((s.start_ea, live))
    return False

def phase6(created):
    names = P['names']
    for ea in created:
        if ida_nalt.get_tinfo(ida_typeinf.tinfo_t(), ea):
            C['p6_has_type'] += 1; continue
        x = hex(ea)
        conv, n = P['protos'].get(x, [None, None])
        if conv not in ('thiscall', 'stdcall', 'cdecl') or n is None:
            C['p6_no_conv:%s' % conv] += 1; continue
        purge = idc.get_func_attr(ea, idc.FUNCATTR_ARGSIZE) if not DRY_RUN else None
        r = ecx_read(ea) if not DRY_RUN else None
        final = conv
        if r is True and conv != 'thiscall': final = 'thiscall'
        if r is False and conv == 'thiscall': final = 'stdcall' if (purge or 0) > 0 else 'cdecl'
        if final != conv: C['p6_ecx_corrected'] += 1; LOG['p6_corrected'].append([x, conv, final])
        m = names.get(x)
        cls = m['class'] if m and m['kind'] == 'vt' and not m['static'] else None
        args = ['int a%d' % (i + 1) for i in range(n)]
        if final == 'thiscall':
            args = ['%s *this' % struct_name(cls) if cls else 'void *this'] + args
        decl = 'int __%s f(%s);' % (final, ', '.join(args) if args else 'void')
        if DRY_RUN: C['p6_would_type'] += 1; continue
        tif = ida_typeinf.tinfo_t()
        ok = ida_typeinf.parse_decl(tif, None, decl, ida_typeinf.PT_SILENT) is not None
        if not ok and cls:   # class struct missing: fall back to void *this
            decl = decl.replace('%s *this' % struct_name(cls), 'void *this')
            ok = ida_typeinf.parse_decl(tif, None, decl, ida_typeinf.PT_SILENT) is not None
        if ok and ida_typeinf.apply_tinfo(ea, tif, ida_typeinf.TINFO_DEFINITE):
            C['p6_typed'] += 1
        else:
            C['p6_type_failed'] += 1; LOG['p6_failed'].append([x, decl])

# ---------------------------------------------------------------- phase 7 names/comments
DUMMY = re.compile(r'^(sub_|nullsub_|j_sub_|unknown_libname_|j_nullsub_)')
TINY_FLIRT = re.compile(r'^(\?do_always_noconv@codecvt_base@std@@MBE_NXZ|\?underflow@|\?classic@locale@std@@)')
def phase7():
    for x, m in P['names'].items():
        ea = h(x)
        fn = ida_funcs.get_func(ea)
        if not fn or fn.start_ea != ea:
            C['p7_not_a_function'] += 1; LOG['p7_not_func'].append(x); continue
        old = ida_name.get_name(ea)
        where = '%s::%s' % (m['class'], m['name']) if m['class'] else m['name']
        how = {'vt': 'vtable slot member', 'off': 'member PLACED BY ADDRESS LAYOUT (heuristic)', 'free': 'free function'}[m['kind']]
        cmt = '[recon] tree/%s %s (%s). Gate-verified equivalent to this code (diffemu; see tree/verification/tree_ledger.json).' % (m['file'], where, how)
        new = None
        if m['class'] and m['kind'] == 'vt':
            nice = '%s_%s' % (m['class'], m['name'])
            if DUMMY.match(old) or (TINY_FLIRT.match(old) and (fn.end_ea - fn.start_ea) <= 5):
                new = nice
                if not DUMMY.match(old): cmt += ' Old IDA name %s was a FLIRT false positive.' % old
        if DRY_RUN:
            C['p7_would_rename' if new else 'p7_keep_name'] += 1; C['p7_would_comment'] += 1; continue
        if new:
            if ida_name.get_name_ea(idc.BADADDR, new) != idc.BADADDR:
                new = '%s_%X' % (new, ea)
            if ida_name.set_name(ea, new, ida_name.SN_NOWARN | ida_name.SN_NOCHECK):
                C['p7_renamed'] += 1; LOG['p7_renamed'].append([x, old, new])
            else:
                C['p7_rename_failed'] += 1
        else:
            C['p7_keep_name'] += 1
        cur = idc.get_func_cmt(ea, 0) or ''
        if '[recon]' not in cur:
            idc.set_func_cmt(ea, (cur + '\n' if cur else '') + cmt, 0); C['p7_commented'] += 1

# ---------------------------------------------------------------- run
phase1(); phase2()
created = phase3()
if not DRY_RUN: ida_auto.auto_wait()
phase4()
phase5()
if not DRY_RUN: ida_auto.auto_wait()
phase6(created)
phase7()
if not DRY_RUN:
    ida_auto.auto_wait()
    json.dump(LOG, open(os.path.join(HERE, 'apply_log.json'), 'w'), indent=0)
result = dict(C)
