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


if __name__ == '__main__':
    unittest.main()
