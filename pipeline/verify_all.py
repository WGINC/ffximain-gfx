"""verify_all.py -- everything committed, checked one to one against the game binary.

Each check either re-derives a result from the DLL and compares it with what is committed, or checks a
committed result against the code directly. Nothing is taken from an earlier run's word.

  1 binary     the DLL is the analysed build (sha256, data/binary.json); every live dump is that build
               (verification/live/dump_verify.json: module timestamp = the analysed one)
  2 facts      every fact file re-derived from the DLL (and the committed export) and compared with data/
  3 agreement  no call has two different stack effects across purges, sp_solve, vcall_resolve, com_calls,
               real_sp and the IDA SP audit
  4 self-tests the rule tests and sp_solve's self-test (every known pop hidden and re-solved)
  5 export     IDA's SP in the committed export against the exact depths (ida_sp_check): no unexplained
               difference; hand corrections (hx_patches, type_fixes, constructs) each apply and carry evidence
  6 accepted   every accepted, unproven declaration, with what rests on it (reported, never a pass)
  7 tree       tree/ regenerated from the committed export and gate verdicts: byte-identical sources
  8 gate       tree/ as committed, built, and every function in it run against the original in the DLL:
               the fuzz gate and, with LIVE="name=live.json ...", the gate on each verified dump; every
               function must PASS everywhere it is comparable (--quick skips this)

    FFXI_DLL=... [LIVE="town=... ..."] python3 pipeline/verify_all.py [--quick] [--jobs N]
Writes verification/verify_all.json; exits 1 if any check fails.
"""
import filecmp, glob, hashlib, json, os, shutil, subprocess, sys, tempfile, collections
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.dirname(P); D = os.path.join(ROOT, 'data')
V = os.path.join(ROOT, 'verification'); EXP = os.path.join(D, 'exports', 'gfx_export.jsonl')
DLL = os.environ['FFXI_DLL']
res = collections.OrderedDict(); ok_all = True


def report(name, ok, **kw):
    global ok_all
    res[name] = dict(ok=ok, **kw); ok_all &= ok is not False
    print('%-10s %s  %s' % ('PASS' if ok else ('INFO' if ok is None else 'FAIL'), name,
                            '; '.join('%s=%s' % (k, v if not isinstance(v, (list, dict)) or len(v) < 8 else '%d items' % len(v)) for k, v in kw.items())))


def sh(cmd, **kw):
    r = subprocess.run(cmd, shell=True, cwd=ROOT, capture_output=True, text=True, **kw)
    return r.returncode, r.stdout + r.stderr


def load(p):
    return json.load(open(p))


def jdiff(a, b):
    """keys whose values differ between two JSON objects (top level), or a type mismatch"""
    if isinstance(a, dict) and isinstance(b, dict):
        return sorted(k for k in set(a) | set(b) if a.get(k) != b.get(k))
    return [] if a == b else ['<whole file>']


def fact_commands(texp, raw_gate):
    """[(fact file in data/, the pipeline/ command that derives it into {o})], in the order each reads the ones
    before it (regen_facts.py runs the same list)"""
    return [('purges.json', 'purges.py "$FFXI_DLL" {o}'), ('ret_kinds.json', 'ret_kinds.py "$FFXI_DLL" {o}'),
             ('fp_consts.json', 'fpconst.py "$FFXI_DLL" %s {o}' % EXP), ('fp_callsites.json', 'fp_callsites.py "$FFXI_DLL" %s {o}' % EXP),
             ('fpret.json', 'fpret.py "$FFXI_DLL" data/func_extents.json {o}'),
             ('vcalls.json', 'vcall_resolve.py {o}'), ('com_calls.json', 'com_calls.py {o}'), ('indirect_targets.json', 'indirect_targets.py {o}'), ('sp_solved.json', 'sp_solve.py {o}'), ('call_pushes.json', 'call_pushes.py {o}'),
             ('cdecl_args.json', 'cdecl_args.py "$FFXI_DLL" {o}'), ('leaf_funcs.json', 'leaf_funcs.py "$FFXI_DLL" {o}'),
             ('callsite_args.json', 'callsites.py %s {o}' % EXP), ('callsite_audit.json', 'callsite_audit.py %s {o}' % EXP),
             ('ida_sp_audit.json', 'ida_sp_check.py %s {o}' % EXP), ('fcmp_exact.json', 'fcmp.py --export %s {o}' % EXP),
             ('edx_use.json', 'edx_inputs.py {o}'), ('ecx_this_sites.json', 'ecx_this_sites.py {o}'),
             ('reg_kept_audit.json', 'reg_kept_audit.py {o}'), ('entry_regs.json', 'entry_regs.py {o}'), ('reg_keeps.json', 'reg_keeps.py {o}'), ('tail_purges.json', 'tail_purges.py {o}'), ('ret_width.json', 'ret_width.py {o}'), ('ret_full.json', 'ret_width.py /dev/null {o}'), ('caller_reads.json', 'caller_reads.py {o}'), ('eax_entry_unseen.json', 'caller_reads.py /dev/null {o}'), ('import_returns.json', 'import_returns.py {o}'), ('eax_inputs.json', 'caller_reads.py /dev/null /dev/null {o} {o}.sites'), ('eax_kept_sites.json', 'caller_reads.py /dev/null /dev/null {o}.inputs {o}'), ('arg_reads.json', 'arg_reads.py {o}'), ('regs_after.json', 'regs_after.py {o}'), ('import_pops.json', 'import_pops.py {o}'), ('reg_saved.json', 'reg_saved.py {o}'), ('this_flow.json', 'this_flow.py {o}'), ('ida_classes.json', 'make_ida_types.py {o}'),
             ('evidence_names.json', 'evidence_names.py {o}'), ('gfx_device_users.json', 'device_users.py {o}'),
             ('reloc_literals.json', 'reloc_literals.py {o}'), ('ui_functions.json', 'ui_sigs.py "$FFXI_DLL" {o}'), ('idb_members.json', 'idb_members.py {o}'),
             ('local_types.json', 'local_evidence.py %s data/exports/gfx_lvars.jsonl {o}.ev && python3 pipeline/local_types.py {o}.ev %s {o}' % (EXP, EXP)),
             ('reg_leftover_audit.json', 'reg_leftover_audit.py %s %s {o}' % (texp, raw_gate))]     # the verified raw translation's


def main():
    quick = '--quick' in sys.argv
    jobs = sys.argv[sys.argv.index('--jobs') + 1] if '--jobs' in sys.argv else str(os.cpu_count())
    tmp = tempfile.mkdtemp(prefix='verify_all_')

    # 1 binary
    h = hashlib.sha256(open(DLL, 'rb').read()).hexdigest(); want = load(os.path.join(D, 'binary.json'))
    report('binary', h == want['sha256'], sha256=h, expected=want['sha256'])
    recs, dec, s = [], json.JSONDecoder(), open(os.path.join(V, 'live', 'dump_verify.json')).read().strip()
    while s: r, n = dec.raw_decode(s); recs.append(r); s = s[n:].strip()
    bad = [r['dump'] for r in recs if r['timestamp'] != r['analysed_timestamp']]
    report('dumps', not bad and bool(recs), dumps=[r['dump'] for r in recs], not_the_build=bad)

    # the export the verified tree and its verdicts come from (tree/source.json): taken from the commit that
    # holds it, checked by SHA-1 in step 7
    src = load(os.path.join(ROOT, 'tree', 'source.json')); texp = os.path.join(tmp, 'tree_export.jsonl')
    sh('git show %s:data/exports/gfx_export.jsonl > %s' % (src['export_commit'], texp))
    # 2 facts re-derived (each tool reads the committed inputs it depends on, so the chain is checked link by link)
    facts = fact_commands(texp, src['raw_gate'])
    if '--facts-from-state' in sys.argv:
        # regen_facts.py's recorded state stands for re-deriving: a fact whose tool's recorded inputs (files read,
        # imports, the hook and the interpreter) are unchanged and whose data/ file is what the tool produced is what
        # re-deriving would give, as far as the tools are deterministic in their inputs -- the assumption step 2 makes
        # anyway. A tool that started a program (its reads unseen), or any change, is re-derived as below
        import regen_facts as RF
        st = json.load(open(RF.STATE)) if os.path.exists(RF.STATE) else {}
        RF.RUN = RF.run_signature()
        left = []
        for f, cmd in facts:
            s_ = st.get(f)
            fresh = (s_ and not s_.get('programs') and s_.get('run') == RF.RUN and
                     all((RF.import_hash(p[7:], s_.get('names', ())) if p.startswith('import:') else RF.fhash(p)) == h
                         for p, h in s_['reads'].items()) and
                     all(RF.fhash(os.path.realpath(os.path.join(D, g))) == h for g, h in s_['produced'].items()))
            if fresh: report('facts:' + f, True, from_state='runs/regen_facts/state.json: inputs and output unchanged')
            else: left.append((f, cmd))
        facts = left
    for f, cmd in facts:
        o = os.path.join(tmp, f); rc, out = sh('python3 pipeline/' + cmd.format(o=o))
        if rc or not os.path.exists(o): report('facts:' + f, False, error=out.strip().splitlines()[-1:] or rc); continue
        d = jdiff(load(o), load(os.path.join(D, f)))
        report('facts:' + f, not d, differing=d[:20], n_differing=len(d))
        if f == 'vcalls.json':
            d = jdiff(load(os.path.join(tmp, 'ecx_use.json')), load(os.path.join(D, 'ecx_use.json')))
            report('facts:ecx_use.json', not d, n_differing=len(d))

    # 2b the registers each game function reads at its entry against the convention the tree calls it with
    #    (the DynVB_Reserve crash, 6 Oct: read its ECX, typed __stdcall(int)). A wrapper (tree/include/gfx_orig.hpp)
    #    that is not __thiscall/__fastcall must not read its entry ECX; none but __fastcall may read EDX; none may
    #    read EAX/EBX/ESI/EDI/EBP (a `push` of a callee-saved register is a save or undecided, not a read).
    #    A read through a callee (no instruction named) is reported, not failed: the leftover audit judges those.
    import re as _re
    wr = {int(m.group(3), 16): (m.group(1), m.group(2)) for m in _re.finditer(
        r'XI_ORIG [^\n]*? (\w+)\([^)]*\) \{ (?:return )?reinterpret_cast<[^(]*\((__\w+) \*\)\([^)]*\)>\(XI_GAME\((0x[0-9A-F]+)u\)\)',
        open(os.path.join(ROOT, 'tree', 'include', 'gfx_orig.hpp')).read())}
    given = {a: set({'__thiscall': ['ECX'], '__fastcall': ['ECX', 'EDX']}.get(cc, [])) for a, (name, cc) in wr.items()}
    # a register-loading wrapper (hx2cpp.ghidra_reg_call): an asm call that loads the registers set from an argument
    for m in _re.finditer(r'XI_ORIG [^\n]*? (\w+)\(([^)]*)\) \{\n    uint32_t xs\[\d+\] = \{[^}]*\}; uint32_t ra = (\S+), rc = (\S+), rd = (\S+?);'
                          r'[^\n]*\n[^\n]*\n[^\n]*"D"\((0x[0-9A-F]+)u\)', open(os.path.join(ROOT, 'tree', 'include', 'gfx_orig.hpp')).read()):
        a = int(m.group(6), 16); wr[a] = (m.group(1), 'asm')
        given[a] = {r for r, v in zip(('EAX', 'ECX', 'EDX'), m.group(3, 4, 5)) if v != '0'}
    eu, du, er, ei = (load(os.path.join(D, f)) for f in ('ecx_use.json', 'edx_use.json', 'entry_regs.json', 'eax_inputs.json'))
    direct = lambda v: isinstance(v, str) and v.startswith('0x')           # an instruction that reads it
    bad, via = [], []
    for a, (name, cc) in sorted(wr.items()):
        k = '0x%08X' % a
        if 'ECX' not in given[a] and eu.get(k): (bad if direct(eu[k]) else via).append('%s %s %s reads ECX' % (k, name, cc))
        if 'EDX' not in given[a] and du.get(k): (bad if direct(du[k]) else via).append('%s %s %s reads EDX' % (k, name, cc))
        if 'EAX' not in given[a] and (ei.get(k) or {}).get('reads'): bad.append('%s %s %s reads its entry EAX (eax_inputs.json)' % (k, name, cc))
        for r, ins in (er.get(k) or {}).items():
            if r.upper() not in given[a] and r != 'eax' and ' push ' not in ' ' + ins[11:] + ' ':
                bad.append('%s %s %s reads %s: %s' % (k, name, cc, r.upper(), ins))
    # a tree function whose call site reads back EAX bytes the callee hands back from its entry EAX
    # (eax_kept_sites.json) must call it through a wrapper that passes EAX
    kept = {(int(v['caller'], 16), int(v['callee'], 16)) for v in load(os.path.join(D, 'eax_kept_sites.json')).values()}
    by_name = {name: a for a, (name, cc) in wr.items()}
    # unless the clean gate runs that call as the game does (gate_sees_eax.py): its verdict then judges it
    sys.path.insert(0, P); import gate_sees_eax
    gcases = gate_sees_eax.load_cases(os.path.join(V, 'cases_clean_final.json'))
    for fn in sorted(os.listdir(os.path.join(ROOT, 'tree', 'src'))):
        for part in _re.split(r'(?m)^(?=// FUNCTION: FFXIMAIN )', open(os.path.join(ROOT, 'tree', 'src', fn)).read())[1:]:
            cea = int(part.split()[3], 16)
            for name in set(_re.findall(r'\borig::(\w+)\(', part)):
                a = by_name.get(name)
                if a is not None and (cea, a) in kept and 'EAX' not in given[a] and not gate_sees_eax.seen(gcases, cea, a):
                    bad.append('0x%08X calls %s (0x%08X) without EAX, and reads back what it hands back (eax_kept_sites.json)' % (cea, name, a))
    report('wrapper-regs', not bad, wrappers=len(wr), reads_a_register_it_is_not_given=bad, via_a_callee=via)

    # 3 one stack effect per call
    purges = load(os.path.join(D, 'purges.json')); vals = collections.defaultdict(dict)
    for name in ('sp_solved', 'vcalls', 'com_calls'):
        for k, v in load(os.path.join(D, name + '.json')).items():
            if not k.startswith('_'): vals[k][name] = v['pops']
    for fk, r in load(os.path.join(D, 'real_sp.json')).items():
        for ck, p in r['calls'].items(): vals[ck]['real_sp'] = p
    for fk, r in load(os.path.join(D, 'ida_sp_audit.json')).items():
        for fx in r['fixes']:
            if fx['why'].startswith('the call pops'): vals[fx['at']]['ida_sp_audit'] = fx['change']
    conflict = {k: v for k, v in vals.items() if len(set(v.values())) > 1}
    report('agreement', not conflict, calls_with_two_or_more_sources=sum(len(v) > 1 for v in vals.values()), conflicts=conflict)

    # 4 self-tests
    rc, out = sh('python3 -m unittest discover -s pipeline/tests'); report('rule-tests', rc == 0, result=out.strip().splitlines()[-1])
    rc, out = sh('python3 pipeline/sp_solve.py --selftest'); report('sp-selftest', rc == 0, result=out.strip().splitlines()[-1])

    # 5 the export and the hand corrections
    audit = load(os.path.join(D, 'ida_sp_audit.json'))
    report('export-sp', not any(r['unexplained'] for r in audit.values()),
           ida_wrong=sorted(audit), unexplained=[r['unexplained'] for r in audit.values() if r['unexplained']])
    sys.path.insert(0, P); import hx_patches
    # every committed export's record of the address: the IDA export, and the Ghidra exports a patch may be written
    # against (hx_patches applies to a Ghidra record whose text it carries the sha1 of)
    recs = collections.defaultdict(list)
    for ef in [EXP] + sorted(glob.glob(os.path.join(D, 'exports', 'ghidra_*.jsonl'))):
        for l in open(ef): r_ = json.loads(l); recs[r_['ea']].append(r_)
    stale, noev = [], []
    for fn in sorted(os.listdir(os.path.join(D, 'hx_patches'))):
        p = load(os.path.join(D, 'hx_patches', fn)); ea = int(fn[:-5], 16)
        if not (p.get('why') and p.get('evidence')): noev.append(fn)
        fits = [(r, v) for r in recs.get(ea, []) for v in [hx_patches.variant(p, r['text'])] if v is not None]
        if not fits or any(r['text'].count(o) != 1 for r, v in fits for o, _ in v['replace']): stale.append(fn)
    report('hx_patches', not stale and not noev, patches=len(os.listdir(os.path.join(D, 'hx_patches'))), stale=stale, without_evidence=noev)
    tf = load(os.path.join(D, 'type_fixes.json')); items = tf.items() if isinstance(tf, dict) else enumerate(tf)
    noev = [k for k, v in items if not k.startswith('_') and isinstance(v, dict) and not (v.get('evidence') or v.get('why'))] if isinstance(tf, dict) else []
    report('type_fixes', not noev, without_evidence=noev)
    cs = load(os.path.join(D, 'constructs.json'))
    report('constructs', all(v.get('evidence') for k, v in cs.items() if not k.startswith('_')), functions=[k for k in cs if not k.startswith('_')])

    # 6 accepted declarations (not proofs): what rests on them
    for name, fd in load(os.path.join(D, 'com_fields.json')).items():
        if name.startswith('_') or 'status' not in fd: continue
        cc = [k for k, v in load(os.path.join(D, 'com_calls.json')).items() if v.get('field') == name]
        sp = [k for k, v in load(os.path.join(D, 'sp_solved.json')).items() if isinstance(v, dict) and name in v.get('rests_on_accepted', [])]
        report('accepted:' + name, None, status=fd['status'][:60] + '...', com_calls=len(cc), balances=sp)

    # 7 the tree is what its recorded inputs generate (tree/source.json: the export, from the commit that holds
    #   it and checked by SHA-1, and the raw verdicts)
    got = hashlib.sha1(open(texp, 'rb').read()).hexdigest() if os.path.getsize(texp) else None
    report('tree-source', got == src['export_sha1'], export_commit=src['export_commit'], sha1=got,
           same_as_committed_export=got == hashlib.sha1(open(EXP, 'rb').read()).hexdigest())
    # the Ghidra route's exports (functions the IDA export does not hold), each checked by SHA-1, appended after it
    gexp, ggate = [], []
    for k, g in enumerate(src.get('ghidra_exports', [])):
        gs = hashlib.sha1(open(os.path.join(ROOT, g['path']), 'rb').read()).hexdigest()
        report('tree-source:' + g['path'], gs == g['sha1'], sha1=gs, expected=g['sha1'])
        gexp.append(os.path.join(ROOT, g['path']))
        # each export's raw gate, in the same order (the first may be the older single `ghidra_raw_gate`)
        gg = g.get('raw_gate') or (src.get('ghidra_raw_gate') if k == 0 else None)
        if gg: ggate.append(gg)
    # raw verdicts given again under the gate's current rules (hx2clean.py, XI_REGATE), where recorded
    rg = ('XI_REGATE=%s ' % os.path.join(ROOT, src['raw_regate'])) if src.get('raw_regate') else ''
    if src.get('given_drop'):        # functions the round left out with a reason (tree_round.py regen --drop --why)
        json.dump(src['given_drop'], open(os.path.join(tmp, 'given_drop.json'), 'w')); rg += 'XI_DROP=%s ' % os.path.join(tmp, 'given_drop.json')
    t = os.path.join(tmp, 'tree'); rc, out = sh('%spipeline/regen_clean.sh %s %s %s' % (rg, ','.join([texp] + gexp), ','.join([src['raw_gate']] + ggate), t))
    sh('python3 pipeline/annotate_gate.py %s verification/gate_clean_final.jsonl' % t)      # each function's // GATE: line
    diff = []
    for sub in ('src', 'include'):
        c = filecmp.dircmp(os.path.join(ROOT, 'tree', sub), os.path.join(t, sub))
        diff += [sub + '/' + x for x in c.left_only + c.right_only + c.diff_files]
    report('tree-regen', not diff and 'failed: none' in out, differing=diff, build=out.strip().splitlines()[-1:])

    # 8 the committed tree, built, against the original in the DLL
    if quick: report('tree-gate', None, skipped='--quick'); return finish(tmp)
    b = os.path.join(tmp, 'built'); shutil.copytree(os.path.join(ROOT, 'tree'), b)
    rc, out = sh('cd %s && ./build.sh' % b)
    dll = os.path.join(b, 'build', 'gfx_recon.dll')
    if not os.path.exists(dll): report('tree-gate', False, build=out.strip().splitlines()[-1:]); return finish(tmp)
    cases = os.path.join(tmp, 'cases.json')
    sh('python3 pipeline/map_clean_cases.py %s tree/placement.json verification/cases_raw_final.json %s' % (dll, cases))
    placed = {c['original'] for c in load(cases)}
    runs = [('fuzz', None)] + [tuple(x.split('=', 1)) for x in os.environ.get('LIVE', '').split()]
    verdicts = {}
    for name, live in runs:
        env = dict(os.environ, **({'FFXI_LIVE': live} if live else {}))
        out = os.path.join(tmp, 'g_' + name)
        subprocess.run([sys.executable, os.path.join(P, 'gate_some.py'), tmp, 'all', '--dll', dll, '--cases', cases,
                        '--out', os.path.basename(out), '--jobs', jobs], env=env, cwd=ROOT, capture_output=True)
        verdicts[name] = {json.loads(l)['original']: json.loads(l)['verdict'] for l in open(out + '.jsonl')}
    bad = {o: {n: v[o] for n, v in verdicts.items()} for o in placed
           if any(v.get(o) == 'FAIL' for v in verdicts.values()) or not any(v.get(o) == 'PASS' for v in verdicts.values())}
    counts = {n: dict(collections.Counter(v.values())) for n, v in verdicts.items()}
    report('tree-gate', not bad, functions=len(placed), gates=counts, not_passing=bad,
           live='not run (set LIVE)' if len(runs) == 1 else [n for n, _ in runs[1:]])
    finish(tmp)


def finish(tmp):
    shutil.rmtree(tmp, ignore_errors=True)
    json.dump(res, open(os.path.join(V, 'verify_all.json'), 'w'), indent=1)
    print('verify_all: %s (verification/verify_all.json)' % ('every check passed' if ok_all else 'FAILED'))
    sys.exit(0 if ok_all else 1)


if __name__ == '__main__':
    main()
