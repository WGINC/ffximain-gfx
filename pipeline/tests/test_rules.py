"""test_rules.py -- fixed inputs and expected outputs for the translator's text rules and the stack solver.

    python3 -m unittest discover -s pipeline/tests        (or: ./run.sh test)
Each expectation is what the rule must produce by its definition, not a recording of what it happens to
produce; a failure means a rule changed meaning.
"""
import os, sys, unittest
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
import hx2cpp as H
import sp_solve as S
from fractions import Fraction


def body(*lines): return '{\n' + '\n'.join('  ' + l for l in lines) + '\n}'


class NanExactBranches(unittest.TestCase):
    def setUp(self):
        import hx_patches
        self.H = hx_patches; self.saved = hx_patches._FCMP

    def tearDown(self): self.H._FCMP = self.saved

    def rec(self, text, rows):
        self.H._FCMP = {'0x10000000': rows}
        return self.H.fcmp_exact({'ea': 0x10000000, 'text': text})['text']

    def test_skip_on_less_only_runs_body_on_nan(self):
        # sqgxColorUpdate 0x102834C3: test ah,5; jp -- the skip is taken on lt only
        self.assertEqual(self.rec('  if ( x >= 0.001 )\n', [['0x1', '>=', 'x', '0.001', ['lt']]]), '  if ( !(x < 0.001) )\n')

    def test_flag_assignment_left_alone(self):
        self.assertEqual(self.rec('  v7 = x >= 0.5;\n', [['0x1', '>=', 'x', '0.5', ['lt']]]), '  v7 = x >= 0.5;\n')

    def test_occurrences_must_match_the_jumps(self):
        t = '  if ( x >= 1.0 )\n  while ( x >= 1.0 )\n'
        self.assertEqual(self.rec(t, [['0x1', '>=', 'x', '1.0', ['lt']]]), t)


class PtrTypes(unittest.TestCase):
    def test_returns_pointer_to_function_pointer(self):
        # 0x1003B8B0, called from CMoSchedularTask_Interpret: the return is a pointer in EAX
        self.assertEqual(H.ptr_type('int (__thiscall ***__thiscall(_DWORD *this))(_DWORD)'), 'void * (__thiscall *)(void *)')


class CtreeSigns(unittest.TestCase):
    def cmp(self, op, sgn, l, r, size, lt, rt): return [0, [op, sgn], l, r, size, lt, rt]

    def test_signed_compare_of_dword_is_cast_signed(self):
        rec = {'cmps': [self.cmp('<', 1, 'i', '*(_DWORD *)(a3 + 68)', 4, 'int', '_DWORD')]}
        self.assertEqual(H.ctree_signs(rec, body('for ( i = 0; i < *(_DWORD *)(a3 + 68); ++i )')),
                         body('for ( i = 0; (int)(i) < (int)(*(_DWORD *)(a3 + 68)); ++i )'))

    def test_unsigned_compare_of_ints_is_cast_unsigned(self):
        rec = {'cmps': [self.cmp('<', 0, 'v7', 'v8', 4, 'int', 'int')]}
        self.assertEqual(H.ctree_signs(rec, body('if ( v7 < v8 && b )')),
                         body('if ( (unsigned int)(v7) < (unsigned int)(v8) && b )'))

    def test_matching_signedness_is_left_alone(self):
        rec = {'cmps': [self.cmp('<', 1, 'a', 'b', 4, 'int', 'int'), self.cmp('>', 0, 'v5', '2u', 4, 'unsigned int', 'unsigned int')]}
        b = body('if ( a < b ) x = v5 > 2u;')
        self.assertEqual(H.ctree_signs(rec, b), b)

    def test_text_inside_a_larger_operand_is_not_touched(self):
        rec = {'cmps': [self.cmp('<', 0, 'v7', 'v8', 4, 'int', 'int')]}
        b = body('if ( a + v7 < v8 ) ;')                   # `v7 < v8` here is not the comparison's text
        self.assertEqual(H.ctree_signs(rec, b), b)

    def test_same_text_read_both_ways_is_ambiguous(self):
        rec = {'cmps': [self.cmp('<', 0, 'v7', 'v8', 4, 'int', 'int'), self.cmp('<', 1, 'v7', 'v8', 4, 'int', 'int')]}
        b = body('if ( v7 < v8 ) ;', 'if ( v7 < v8 ) ;')
        self.assertEqual(H.ctree_signs(rec, b), b)

    def test_byte_compare_uses_its_own_width(self):
        rec = {'cmps': [self.cmp('<', 1, 'v3', 'v4', 1, '_BYTE', '_BYTE')]}
        self.assertEqual(H.ctree_signs(rec, body('if ( v3 < v4 ) ;')), body('if ( (__int8)(v3) < (__int8)(v4) ) ;'))

    def test_unknown_type_is_left_alone(self):
        rec = {'cmps': [self.cmp('<', 1, 'a', 'b', 4, 'struct X', 'int')]}
        b = body('if ( a < b ) ;')
        self.assertEqual(H.ctree_signs(rec, b), b)

    def test_no_cmps_is_identity(self):
        b = body('if ( a < b ) ;')
        self.assertEqual(H.ctree_signs({}, b), b)

    def test_c_signed(self):
        self.assertIs(H.c_signed('_DWORD'), False)
        self.assertIs(H.c_signed('unsigned int'), False)
        self.assertIs(H.c_signed('char *'), False)            # pointers compare unsigned
        self.assertIs(H.c_signed('int'), True)
        self.assertIs(H.c_signed('const __int16'), True)
        self.assertIsNone(H.c_signed('struct Foo'))


class SignedDwordCompares(unittest.TestCase):
    def test_compare_with_literal(self):
        self.assertIn('*(int *)(a1 + 4) + 1 > 0', H.signed_dword_compares(body('int i;', 'x = *(_DWORD *)(a1 + 4) + 1 > 0;')))

    def test_division(self):
        self.assertIn('*(int *)(a1 + 8) / 2', H.signed_dword_compares(body('int i;', 'v = *(_DWORD *)(a1 + 8) / 2;')))

    def test_signed_local_loop_bound(self):
        self.assertIn('i < *(int *)(a3 + 68)', H.signed_dword_compares(body('int i;', 'for ( i = 0; i < *(_DWORD *)(a3 + 68); ++i )')))

    def test_rule_off_when_export_has_cmps(self):
        b = body('int i;', 'v = *(_DWORD *)(a1 + 8) / 2;')
        self.assertNotIn('*(int *)', H.body_rules(0x10000000, b, exact_signs=True))


class VmethodThis(unittest.TestCase):
    def test_stdcall_vftable_call_becomes_thiscall(self):
        self.assertEqual(H.vmethod_this('(*((void (__stdcall **)(int, float))g_pProcessor->vftable + 33))(5, 1.0);'),
                         '(*((void (__thiscall **)(void *, int, float))g_pProcessor->vftable + 33))(g_pProcessor, 5, 1.0);')

    def test_other_calls_untouched(self):
        s = '(*(void (__stdcall **)(int))(*(_DWORD *)a1 + 36))(a1);'
        self.assertEqual(H.vmethod_this(s), s)


class TypeFixes(unittest.TestCase):
    def fixed(self, rec, ftype):
        import hx_patches as P
        saved = P._T; P._T = {rec['ea']: ftype}
        try: return P.fix_types(rec)
        finally: P._T = saved

    def test_added_this_sets_convention_and_parameter(self):
        rec = {'ea': 1, 'name': 'f', 'cc': 'stdcall', 'ret': 'void', 'objs': {},
               'text': 'void __stdcall f(float *a1, int a2)\n{\n}', 'args': [['a1', 'float *', 4, 'float', 4], ['a2', 'int', 4, None, None]]}
        r = self.fixed(rec, 'void __thiscall(void *this, float *a1, int a2)')
        self.assertEqual(r['cc'], 'thiscall')
        self.assertEqual(r['args'], [['this', 'void *', 4, None, None], ['a1', 'float *', 4, 'float', 4], ['a2', 'int', 4, None, None]])

    def test_added_parameter_of_undefined_size_is_refused(self):
        rec = {'ea': 1, 'name': 'f', 'cc': 'stdcall', 'ret': 'void', 'objs': {}, 'text': 'void __stdcall f(int a1)\n{\n}',
               'args': [['a1', 'int', 4, None, None]]}
        with self.assertRaises(ValueError): self.fixed(rec, 'void __stdcall(struct S s, int a1)')


class FreeSelf(unittest.TestCase):
    def test_local_named_self_is_renamed(self):
        import hx_patches as P
        rec = {'ea': 1, 'args': [['this', '_DWORD *']], 'text': 'void __thiscall f(_DWORD *this)\n{\n  _DWORD self[16];\n  g(self, this);\n}'}
        r = P.free_self(rec)
        self.assertEqual(r['text'], 'void __thiscall f(_DWORD *this)\n{\n  _DWORD self_v[16];\n  g(self_v, this);\n}')

    def test_first_parameter_named_self_is_this(self):
        import hx_patches as P
        rec = {'ea': 1, 'args': [['self', 'void *'], ['a2', 'float']], 'text': 'int __thiscall f(void *self, float a2)\n{\n  return g(self);\n}'}
        self.assertEqual(P.free_self(rec), rec)


class GlobalNamedLikeALocal(unittest.TestCase):
    """a global is a macro in the clean tree: one named like any function's parameter or local is renamed"""
    def recs(self):
        a = {'ea': 1, 'name': 'f', 'objs': {'C': {'ea': 0x106631D4, 'func': False}},
             'text': 'int f(int a1)\n{\n  return C + Foo::C;\n}'}
        b = {'ea': 2, 'name': 'g', 'objs': {'C': {'ea': 0x106631D4, 'func': False}},
             'text': 'int g(_DWORD *C)\n{\n  int v2; // eax\n\n  v2 = ::C;\n  return *C + v2;\n}'}
        return {1: a, 2: b}

    def test_declared_names(self):
        import hx2clean as C
        r = self.recs()
        self.assertEqual(C.declared_names(r[2]), {'C', 'v2'})
        self.assertEqual(C.declared_names({'text': 'void *h()\n{\n  return &CDx_classdesc;\n}'}), set())

    def test_renamed_where_global_shadowed_where_not(self):
        import hx2clean as C
        R = self.recs(); C.canonicalise_names(R, {})
        self.assertEqual(R[1]['text'], 'int f(int a1)\n{\n  return g_C_106631D4 + Foo::C;\n}')
        self.assertEqual(R[2]['text'], 'int g(_DWORD *C)\n{\n  int v2; // eax\n\n  v2 = g_C_106631D4;\n  return *C + v2;\n}')


class FloatCompares(unittest.TestCase):
    """fcmp.py: the outcomes an x87 compare branch takes, from the flag arithmetic; the NaN-exact C condition"""
    class I:
        def __init__(s, m, o=''): s.mnemonic, s.op_str = m, o

    def outcomes(self, *seq):
        import fcmp as F
        L = [self.I('fcomp', 'dword ptr [esi]'), self.I('fnstsw', 'ax')] + [self.I(*x) for x in seq]
        return set(F.taken(L, len(L) - 1)[0])

    def test_condition_codes(self):
        self.assertEqual(self.outcomes(('test', 'ah, 5'), ('jp', '0x10')), {'gt', 'eq', 'un'})     # !(a < b)
        self.assertEqual(self.outcomes(('test', 'ah, 5'), ('jnp', '0x10')), {'lt'})
        self.assertEqual(self.outcomes(('and', 'eax, 0x4100'), ('jne', '0x10')), {'lt', 'eq', 'un'})
        self.assertEqual(self.outcomes(('test', 'ah, 0x44'), ('jnp', '0x10')), {'eq'})
        self.assertEqual(self.outcomes(('test', 'ah, 0x41'), ('jne', '0x10')), {'lt', 'eq', 'un'})
        self.assertEqual(self.outcomes(('sahf',), ('jae', '0x10')), {'gt', 'eq'})

    def test_exact_condition(self):
        import fcmp as F
        self.assertEqual(F.exact('x', '>=', 'y', frozenset({'gt', 'eq', 'un'})), '!(x < y)')
        self.assertEqual(F.exact('x', '>=', 'y', frozenset({'lt'})), '!(x < y)')       # the not-taken side
        self.assertEqual(F.exact('x', '<', 'y', frozenset({'lt'})), 'x < y')
        self.assertEqual(F.exact('x', '==', 'y', frozenset({'eq'})), 'x == y')
        self.assertIsNone(F.exact('x', '<', 'y', frozenset({'eq'})))                    # text does not match code


class Solver(unittest.TestCase):
    def test_determined(self):
        # p0 + p1 = 8 and p1 = 4  ->  p0 = 4, p1 = 4
        v, ok = S.solve([S.Lin(-8, {0: 1, 1: 1}), S.Lin(-4, {1: 1})], 2)
        self.assertTrue(ok); self.assertEqual(v, {0: Fraction(4), 1: Fraction(4)})

    def test_underdetermined_leaves_out(self):
        v, ok = S.solve([S.Lin(-8, {0: 1, 1: 1})], 2)
        self.assertTrue(ok); self.assertEqual(v, {})

    def test_inconsistent(self):
        v, ok = S.solve([S.Lin(-8, {0: 1}), S.Lin(-4, {0: 1})], 1)
        self.assertFalse(ok)

    def test_redundant_consistent(self):
        v, ok = S.solve([S.Lin(-8, {0: 1}), S.Lin(-16, {0: 2})], 1)
        self.assertTrue(ok); self.assertEqual(v, {0: Fraction(8)})

    def test_lin_arith(self):
        a = S.Lin(4, {0: 1}).add(-4, 0, -1)
        self.assertEqual(a, S.Lin(0, {}))


class EcxUse(unittest.TestCase):
    def eff(self, hexbytes):
        import vcall_resolve as V
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
        return V.ecx_effect(next(md.disasm(bytes.fromhex(hexbytes), 0x1000)))

    def test_idioms_are_writes(self):
        self.assertEqual(self.eff('83C9FF'), (set(), {'ecx'}, False))     # or ecx, -1
        self.assertEqual(self.eff('33C9'), (set(), {'ecx'}, False))       # xor ecx, ecx
        self.assertEqual(self.eff('8BC8'), (set(), {'ecx'}, False))       # mov ecx, eax

    def test_reads(self):
        self.assertIn('ecx', self.eff('8BD1')[0])                         # mov edx, ecx
        self.assertIn('ecx', self.eff('81C178010000')[0])                 # add ecx, 178h
        self.assertIn('ecx', self.eff('8B4904')[0])                       # mov ecx, [ecx+4]

    def test_push_ecx_is_undecided(self):
        self.assertEqual(self.eff('51'), (set(), set(), True))            # push ecx (also reserves 4 bytes)

    def test_partial_register(self):
        import vcall_resolve as V
        rd, wr, _ = self.eff('668B4F08')                                  # mov cx, [edi+8]
        self.assertEqual(wr, {'cx'})
        rd2, _, _ = self.eff('662B0F')                                    # sub cx, [edi]
        self.assertTrue(all(V.covered(p, frozenset(wr)) for p in rd2))   # reads only what was written
        self.assertFalse(V.covered('ecx', frozenset(wr)))                 # the upper half is still the input


    def run_paths(self, join_hex):
        """`je` to `mov ecx, [eax]` or fall through to `mov cl, [eax]`; both reach the join instruction, then `ret`"""
        import vcall_resolve as V
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
        ins = {i.address: i for i in md.disasm(bytes.fromhex('85C0' '7404' '8A08' 'EB02' '8B08' + join_hex + 'C3'), 0x1000)}
        V.FIRST_READ.clear()
        return V.ecx_inputs(None, {0x1000: ins}, {0x1000})[0x1000]

    def test_join_keeps_the_parts_both_paths_write(self):
        self.assertFalse(self.run_paths('8808'))      # mov [eax], cl: CL is written on both paths (as CL and as ECX)
        self.assertTrue(self.run_paths('8908'))       # mov [eax], ecx: the upper bytes are the input on one path


class RegparmEntry(unittest.TestCase):
    """hx2cpp.regparm_entry: a register input becomes GCC regparm with the convention's own pop rule"""
    def test_thiscall_with_eax(self):
        H.PURGES['0x0BADF00D'] = 4
        (a, p), _ = H.regparm_entry(0x0BADF00D, '__thiscall', 'void *self, int a, undefined4 xi_eax', ['ECX', 'stack', 'EAX'])
        self.assertEqual(a, '__attribute__((stdcall, regparm(3)))')
        self.assertEqual(p, 'undefined4 xi_eax, uint32_t xi_edx_unused, void *self, int a')

    def test_cdecl_with_eax(self):
        H.PURGES['0x0BADF00E'] = 0
        (a, p), _ = H.regparm_entry(0x0BADF00E, '__cdecl', 'int a, undefined4 xi_eax', ['stack', 'EAX'])
        self.assertEqual((a, p), ('__attribute__((cdecl, regparm(1)))', 'undefined4 xi_eax, int a'))

    def test_pop_count_must_match(self):
        H.PURGES['0x0BADF00F'] = 8
        a, why = H.regparm_entry(0x0BADF00F, '__stdcall', 'int a, undefined4 xi_eax', ['stack', 'EAX'])
        self.assertIsNone(a)


class Ps2Demangle(unittest.TestCase):
    """ps2_match.demangle: CodeWarrior (cfront-style) names to (class, method)"""
    def test_names(self):
        import ps2_match as M
        self.assertEqual(M.demangle('ExecuteTag__15YmSchedulerTaskFP12YmSchCodeTag'), ('YmSchedulerTask', 'ExecuteTag'))
        self.assertEqual(M.demangle('__ct__8CTkMouseFv'), ('CTkMouse', 'Constructor'))
        self.assertEqual(M.demangle('OnMove__Q211YmSoundElem18YmVolumeChangeTaskFv'), ('YmSoundElem::YmVolumeChangeTask', 'OnMove'))
        self.assertEqual(M.demangle('IsEvent__FUc'), (None, 'IsEvent'))
        self.assertIsNone(M.demangle('ReqEventInfo'))


class FcmpSimplify(unittest.TestCase):
    """fcmp_simplify.equivalent: NaN-test spellings to the one comparison with the same IEEE truth table"""
    def test_forms(self):
        import fcmp_simplify as F
        self.assertEqual(F.equivalent('(!NAN(a) && !NAN(b)) && a < b != (a == b)'), 'a <= b')
        self.assertEqual(F.equivalent('NAN(a) || NAN(b) || a < b'), '!(a >= b)')
        self.assertEqual(F.equivalent('!NAN(a) && !NAN(b) && a == b'), 'a == b')

    def test_not_one_comparison(self):
        import fcmp_simplify as F
        self.assertIsNone(F.equivalent('NAN(a) || NAN(b)'))                  # unordered: no single comparison
        self.assertIsNone(F.equivalent('!NAN(f(a)) && f(a) < b'))            # a call: evaluated once, not twice

    def test_runs_the_table_in_c(self):
        # the six states evaluated by the compiler, not by this file's model of C
        import fcmp_simplify as F, subprocess, tempfile, os
        src = '#include <math.h>\n#include <stdio.h>\n#define NAN_(x) ((x) != (x))\nint main(){double v[][2]={{1,2},{2,2},{3,2},{NAN,2},{2,NAN},{NAN,NAN}};int bad=0;\nfor(int i=0;i<6;i++){double a=v[i][0],b=v[i][1];\n'
        for e in ['(!NAN_(a) && !NAN_(b)) && a < b != (a == b)', 'NAN_(a) || NAN_(b) || a < b', '!NAN_(a) && !NAN_(b) && a == b']:
            r = F.equivalent(e.replace('NAN_', 'NAN'))
            src += 'if ((%s) != (%s)) bad++;\n' % (e, r)
        src += '}printf("%d", bad);return 0;}\n'
        d = tempfile.mkdtemp(); open(os.path.join(d, 't.c'), 'w').write(src)
        subprocess.run(['gcc', '-O2', os.path.join(d, 't.c'), '-o', os.path.join(d, 't')], check=True)
        self.assertEqual(subprocess.run([os.path.join(d, 't')], capture_output=True, text=True).stdout, '0')

class PartialRegs(unittest.TestCase):
    """partial_regs.py: each rewrite is compiled next to the macro it replaces (pipeline/xi_hexrays.hpp) and both
    are run over values that exercise every bit of every part"""
    TYPES = ['char', 'unsigned __int8', '__int16', 'unsigned __int16', 'int', 'unsigned int', '_DWORD',
             '__int64', 'unsigned __int64']
    CASE = ('  { %(t)s x = (%(t)s)v, y = (%(t)s)v, y0 = (%(t)s)v; long long e = (long long)w;\n'
            '    long long a = (long long)%(m)s(x); long long r; %(rd)s long long b = r;\n'
            '    %(m)s(y0) = e; %(wr)s\n'
            '    if (a != b || y0 != y) { printf("%(t)s %(m)s %%llx %%llx\\n", v, w); return 1; } }')

    def test_rewrites_equal_the_macros(self):
        import partial_regs as P, subprocess, tempfile, shutil
        cases = []
        for t in self.TYPES:
            W = P.INT[P.norm(t)][0]
            for mac, (u, idx, sg) in sorted(P.MACROS.items()):
                k = idx(W)
                if k < 0 or (k + 1) * u > W: continue
                rd, st = P.rewrite('{\n  r = %s(x);\n}' % mac, {'x': t})
                self.assertEqual(st, {'read rewritten': 1}, (t, mac))
                wr, st = P.rewrite('{\n  %s(y) = e;\n}' % mac, {'y': t})
                self.assertEqual(st, {'store rewritten': 1}, (t, mac))
                cases.append(self.CASE % {'t': t, 'm': mac, 'rd': rd.strip('{}\n '), 'wr': wr.strip('{}\n ')})
        src = ('#include <cstdint>\n#include <cstdio>\n#ifndef __int64\n#define __int64 long long\n#endif\n#include "xi_hexrays.hpp"\nint main() {\n'
               '  const unsigned long long vals[] = {0, 1, 0x7F, 0x80, 0xFF, 0x100, 0x7FFF, 0x8000, 0xFFFF, 0x12345678,\n'
               '    0x80000000ull, 0xFFFFFFFFull, 0x0123456789ABCDEFull, 0xFEDCBA9876543210ull, 0x8000000000000000ull, ~0ull};\n'
               '  for (unsigned long long v : vals) for (unsigned long long w : vals) {\n' + '\n'.join(cases) +
               '\n  }\n  return 0;\n}\n')
        d = tempfile.mkdtemp()
        try:
            open(os.path.join(d, 't.cpp'), 'w').write(src)
            inc = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
            c = subprocess.run(['g++', '-std=c++17', '-fno-strict-aliasing', '-I', inc, '-o', os.path.join(d, 't'),
                                os.path.join(d, 't.cpp')], capture_output=True, text=True)
            self.assertEqual(c.returncode, 0, c.stderr[-3000:])
            r = subprocess.run([os.path.join(d, 't')], capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stdout)
        finally:
            shutil.rmtree(d)
        self.assertGreater(len(cases), 40)

    def test_left_alone(self):
        import partial_regs as P
        T = {'f': 'float', 'p': 'int', 'q': 'int', 's': 'int', 'h': 'int'}
        for src in ['{\n  r = LOBYTE(f);\n}', '{\n  r = LOBYTE(g);\n}', '{\n  BYTE1(p) &= ~4u;\n}',
                    '{\n  r = HIDWORD(q);\n}', '{\n  g(&s);\n  LOBYTE(s) = 1;\n}', '{\n  LOBYTE(h) = h++;\n}',
                    '{\n  g(&LOBYTE(p));\n}']:
            self.assertEqual(P.rewrite(src, T)[0], src)

    def test_bitwise_and_is_a_read(self):
        import partial_regs as P
        self.assertEqual(P.rewrite('{\n  r = a & BYTE1(p);\n}', {'p': 'int'})[0], '{\n  r = a & (_BYTE)((uint32_t)(p) >> 8);\n}')



class GhidraIndexAndAdd(unittest.TestCase):
    # p-code PTRADD with an element size other than the printed base's pointee, and INT_ADD (bytes) on a wide pointer
    def test_ptradd_element_size(self):
        self.assertEqual(H.ghidra_ptradds('r = (char)(&DAT_1)/*XI_PA:1*/[i];')[0], 'r = (char)((uint8_t *)((&DAT_1)))[i];')
        self.assertEqual(H.ghidra_ptradds('r = p/*XI_PA:2*/[i];')[0], 'r = ((uint16_t *)(p))[i];')

    def test_ptradd_base_not_parsed(self):
        self.assertIsNone(H.ghidra_ptradds('r = f(x)/*XI_PA:1*/[i];')[0])

    def test_byte_add(self):
        self.assertEqual(H.ghidra_byte_adds('g((int *)(&DAT_1 /*XI_BA:1*/+ i * 0x38));'), 'g((int *)(((char *)&DAT_1) + i * 0x38));')
        self.assertEqual(H.ghidra_byte_adds('*(int *)(&g /*XI_BA:1*/+ p * 4) = x;'), '*(int *)(((char *)&g) + p * 4) = x;')
        self.assertEqual(H.ghidra_byte_adds('q = &g /*XI_BA:2*/+ i;'), 'q = ((uint16_t *)&g) + i;')


class BitCasts(unittest.TestCase):
    # p-code CAST between floating and non-floating keeps the bits; the definition and the cast are one CAST
    def test_written_and_printed_cast(self):
        self.assertEqual(H.ghidra_bitcasts('/*XI_BC:2*/f = /*XI_BC:3*/(float)p[0x16];')[0], 'f = xi_castbits<float>(p[0x16]);')

    def test_written_without_type(self):
        self.assertIsNone(H.ghidra_bitcasts('/*XI_BC:2*/f = g(x);')[0])


class EntryEax(unittest.TestCase):
    # in_EAX where no code sees EAX's entry value: started at 0
    def test_unseen(self):
        H._EAX_UNSEEN = {'0x00001000'}
        r = H.ghidra_entry_eax({'ea': 0x1000, 'text': '\nundefined4 f(void)\n\n{\n  undefined4 in_EAX;\n  \n  return CONCAT31((int3)((uint)in_EAX >> 8),1);\n}\n'})
        self.assertIn('undefined4 xi_entry_eax = 0;', r['text'])
        self.assertNotIn('in_EAX', r['text'])
        self.assertIsNone(H.ghidra_entry_eax({'ea': 0x2000, 'text': '\nundefined4 f(void)\n\n{\n  undefined4 in_EAX;\n}\n'}))
        H._EAX_UNSEEN = None


class Thunk(unittest.TestCase):
    # a function that is one `jmp T`: a call to T with its own parameters
    def test_thunk_body(self):
        H._THUNKS = {0x1000: 0x2000}
        r = H.ghidra_thunk({'ea': 0x1000, 'name': 'thunk_f', 'type': 'int __stdcall thunk_f(int a, int b)', 'ret': 'int',
                            'args': [['a', 'int'], ['b', 'int']], 'objs': {}, 'purged': 8,
                            'text': '\nint __stdcall thunk_f(int a,int b)\n\n{\n  return g(a) + b;\n}\n'})
        self.assertIn('return FUN_00002000(a, b);', r['text'])
        self.assertEqual(r['objs']['FUN_00002000']['type'], 'int __stdcall(int a, int b)')
        self.assertIsNone(H.ghidra_thunk({'ea': 0x3000, 'text': ''}))
        H._THUNKS = None


if __name__ == '__main__':
    unittest.main()


class FloatLiteral(unittest.TestCase):
    """source_shape.float_literal: a literal of exactly the constant's value (GCC evaluates constants in long double)"""
    def lit(self, b, n=4):
        import source_shape as SS
        return SS.float_literal(b, n)

    def test_exact(self):
        self.assertEqual(self.lit(0x43000000), '128.0f')
        self.assertEqual(self.lit(0x3F000000), '0.5f')
        self.assertEqual(self.lit(0xBF800000), '(-1.0f)')

    def test_not_shortest_when_inexact(self):
        self.assertEqual(self.lit(0x3EA2FABE), '0.318319261074066162109375f')   # not 0.31831926f
        self.assertIsNone(self.lit(0x3FB999999999999A, 8))                       # 0.1: its exact value is too long


class ByteFlow(unittest.TestCase):
    """byte_flow.step: the value's bits through the registers, from entry EAX; (examined, escaped, EAX bytes kept)"""
    CASES = [
        ('the DX-only store (0x10071E90)', '668b41188d500131c281e2ff7f000031c266895118', [], [], [2, 3]),
        ('a bitfield read', '8a4630c1e80483e00783f803', [], [], []),
        ('a short pushed: its upper bytes escape', '668b0750', [], [2, 3], [2, 3]),
        ('a compare', '3d00100000', [0, 1, 2, 3], [0, 1, 2, 3], [0, 1, 2, 3]),
        ('an address', '8b4804', [0, 1, 2, 3], [0, 1, 2, 3], [0, 1, 2, 3]),
        ('a flag read after arithmetic', '480f84f91f0000', [0, 1, 2, 3], [0, 1, 2, 3], []),
        ('sbb r, r', '19c0', [], [], []),
        ('test with an immediate', 'a9ff000000', [0], [0], [0, 1, 2, 3]),
        ('a byte moved down, compared', 'c1e8083c01', [1], [1], []),
        ('a constant mask in a register (0x103239F8)', '668b4206bfff07000089c1c1e90421f90fb7d985db', [], [], [2, 3]),
    ]

    def run_code(self, hexbytes, strict=False):
        import byte_flow as B
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
        st, ex, es = B.entry_state(), set(), set()
        for i in md.disasm(bytes.fromhex(hexbytes), 0x1000):
            if i.mnemonic.startswith('j'):                       # a conditional jump reads the flags
                ex |= st.flags; es |= st.flags; continue
            st, e, s = B.step(i, st, strict); ex |= e; es |= s
        kept = [k for k in range(4) if (st.p('eax')[k] >> (8 * k)) & 0xFF]
        return sorted(ex), sorted(es), kept

    def test_cases(self):
        for name, code, ex, es, kept in self.CASES:
            self.assertEqual(self.run_code(code), (ex, es, kept), name)

    def test_join_keeps_unchanged_or_absent(self):
        import byte_flow as B
        a = B.entry_state(); b = B.State({})                        # one path: AH unchanged; the other: overwritten
        j = B.join(a, b)
        self.assertEqual([x for x in j.c('eax')], [0, 0, 0, 0])      # never "changed"
        self.assertEqual(j.p('eax')[1], 0xFF00)

    def test_unknown_mask_strict(self):
        code = '89c1230e21d985c9'                                   # mov ecx, eax; and ecx, [esi]; and ecx, ebx; test ecx, ecx
        self.assertEqual(self.run_code(code)[0], [0, 1, 2, 3])
        self.assertEqual(self.run_code(code, strict=True)[0], [])
