"""Minimal MSVC name demangling, enough to match how Hex-Rays prints FLIRT/library names:
?name@Scope1@Scope2@@... -> Scope2::Scope1::name ; ??0X@@ -> X::X ; ??1X@@ -> X::~X ;
??<op>X@@ -> X::operator<op>."""
import re
OPS = {'2': 'operator new', '3': 'operator delete', '4': 'operator=', '5': 'operator>>', '6': 'operator<<',
       '7': 'operator!', '8': 'operator==', '9': 'operator!=', 'A': 'operator[]', 'C': 'operator->',
       'D': 'operator*', 'E': 'operator++', 'F': 'operator--', 'G': 'operator-', 'H': 'operator+',
       'I': 'operator&', 'J': 'operator->*', 'K': 'operator/', 'L': 'operator%', 'M': 'operator<',
       'N': 'operator<=', 'O': 'operator>', 'P': 'operator>=', 'Q': 'operator,', 'R': 'operator()',
       'S': 'operator~', 'T': 'operator^', 'U': 'operator|', 'V': 'operator&&', 'W': 'operator||',
       'X': 'operator*=', 'Y': 'operator+=', 'Z': 'operator-='}
def demangle(n):
    n = re.sub(r'_\d+$', '', n)               # IDA's duplicate suffix (_0, _12, ...)
    m = re.match(r'^\?\?([0-9A-Z])([^@]+(?:@[^@]+)*)@@', n)
    if m:
        code, scopes = m.group(1), m.group(2).split('@')
        cls = '::'.join(reversed(scopes))
        last = scopes[0]
        if code == '0': return cls + '::' + last
        if code == '1': return cls + '::~' + last
        if code in OPS: return cls + '::' + OPS[code]
        return None
    m = re.match(r'^\?([A-Za-z_]\w*)((?:@[A-Za-z_]\w*)*)@@', n)
    if m:
        scopes = [s for s in m.group(2).split('@') if s]
        return '::'.join(list(reversed(scopes)) + [m.group(1)])
    return None
