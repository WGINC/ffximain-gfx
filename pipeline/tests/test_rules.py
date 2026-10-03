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
