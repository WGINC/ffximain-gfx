"""shader_catalogue.py -- the game's own shaders, from the binary: source text and the exact tokens the game creates.

Where they are (read from the code, analysed build):
  * 0x10178250 builds 5 vertex shaders and 1 pixel shader. Each comes from a table of pointers to source lines
    in .data, with a line count pushed next to it:
        push byte [CDx+0x920]; push COUNT; push TABLE; lea ecx, text; call 0x10177090
    then hands the text to Gfx_AssembleVertexShader (0x10177240: D3DXAssembleShader with Flags 0, then the
    device's CreateVertexShader with the declaration built on 0x10178250's stack) or to 0x101772E0 (Flags 2, then
    CreatePixelShader).
  * 0x10177090 (the text): for each line, sprintf("%s%c", line, '\\n'). A line that starts with '#' is dropped when
    the flag byte is non-zero; when it is zero the '#' is replaced by a space and the line is kept. So each shader
    has two variants, by [CDx+0x920].
The tokens: the text of each variant is run through the game's own statically linked D3DXAssembleShader
(0x102DAA0D) in Unicorn, with the flags the game passes, and read back through the ID3DXBuffer the game's
code gets (GetBufferPointer, GetBufferSize, both run). Imports the assembler calls are answered only for the
process heap (HeapAlloc / HeapFree / HeapReAlloc / HeapSize / GetProcessHeap: a block, as Win32 documents) and
critical sections (void; one thread enters at once); any other import stops the run and is named (nothing is guessed).

Output:
  data/shader_catalogue.json   committed: per shader and variant, the table, line count, flag, D3DX flags, token
                               count and SHA-256 of the tokens (no game bytes)
  OUT_DIR/*.txt, *.tok          not committed: the source text and the token stream of each (game content)
vshader.py names a captured shader (xikit_vshader_*.bin) by the SHA-256 of its code tokens.

    python3 shader_catalogue.py [OUT_DIR]          (uses $FFXI_DLL; OUT_DIR default runs/shaders)"""
import hashlib, json, os, struct, sys
import pefile
from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_PROT_ALL, UC_HOOK_CODE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_FS, UC_X86_REG_GDTR,
                               UC_X86_REG_CS, UC_X86_REG_SS, UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_GS)

H = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.normpath(os.path.join(H, '..', '..'))
BASE = 0x10000000
BUILDER = 0x10178250                      # makes the 5 vertex shaders and the pixel shader
TEXT_FN = 0x10177090                      # table -> text
ASSEMBLE = 0x102DAA0D                     # _D3DXAssembleShader@24 (statically linked D3DX 8.1)
VS_CREATE, PS_CREATE = 0x10177240, 0x101772E0
TRAP, TRAP_SIZE = 0x70000000, 0x10000     # import entry points: one dword each
HEAP, HEAP_SIZE = 0x60000000, 0x4000000
STACK_TOP, STACK_SIZE = 0x7FF00000, 0x100000
TEB, GDT = 0x7F7F0000, 0x7F7E0000
SENTINEL = 0x7E000000


def shaders_of(img):
    """the (table, count, kind) triples 0x10178250 passes to 0x10177090, read from its pushes: `push COUNT;
    push TABLE; lea ecx, ...; call 0x10177090`, then the next call is the create function that takes that text"""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    ins = [i for a, b in ext['0x%08X' % BUILDER] for i in md.disasm(img[a - BASE:b - BASE], a)]
    out = []
    for n, i in enumerate(ins):
        if i.mnemonic == 'call' and i.op_str == hex(TEXT_FN):
            p = [x for x in ins[max(0, n - 4):n] if x.mnemonic == 'push']
            table, count = int(p[-1].op_str, 16), int(p[-2].op_str, 16)
            nxt = next(x for x in ins[n + 1:] if x.mnemonic == 'call')
            kind = {VS_CREATE: 'vs', PS_CREATE: 'ps'}.get(int(nxt.op_str, 16))
            if kind is None: sys.exit('shader_catalogue.py: the call after 0x%08X is not a known create function' % i.address)
            out.append((table, count, kind, i.address))
    return out


def declaration_of(img):
    """the vertex declaration 0x10178250 writes to its stack before the first call (`mov dword [esp+N], imm`, in
    order, up to D3DVSD_END 0xFFFFFFFF): the one Gfx_AssembleVertexShader passes to CreateVertexShader"""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
    ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
    out = []
    for a, b in ext['0x%08X' % BUILDER]:
        for i in md.disasm(img[a - BASE:b - BASE], a):
            if i.mnemonic == 'call': return out
            o = i.operands
            if i.mnemonic == 'mov' and len(o) == 2 and o[0].type == capstone.x86.X86_OP_MEM and o[1].type == capstone.x86.X86_OP_IMM:
                out.append(o[1].imm & 0xFFFFFFFF)
                if out[-1] == 0xFFFFFFFF: return out
    return out


def text_of(img, table, count, flag):
    """0x10177090's text for a flag value"""
    s = b''
    for k in range(count):
        p = struct.unpack_from('<I', img, table - BASE + 4 * k)[0]
        line = img[p - BASE:img.index(b'\0', p - BASE)]
        if line[:1] == b'#':
            if flag: continue
            line = b' ' + line[1:]
        s += line + b'\n'
    return s


class Asm:
    def __init__(self, path):
        pe = pefile.PE(path); self.img = pe.get_memory_mapped_image(); size = len(self.img) + 0xFFF & ~0xFFF
        u = self.u = Uc(UC_ARCH_X86, UC_MODE_32)
        u.mem_map(BASE, size, UC_PROT_ALL); u.mem_write(BASE, self.img)
        self.names = {}
        for e in pe.DIRECTORY_ENTRY_IMPORT:
            for imp in e.imports:
                t = TRAP + 4 * len(self.names)
                self.names[t] = (imp.name or b'#%d' % imp.ordinal).decode()
                u.mem_write(imp.address, struct.pack('<I', t))
        u.mem_map(TRAP, TRAP_SIZE, UC_PROT_ALL); u.mem_write(TRAP, b'\xC3' * TRAP_SIZE)
        u.mem_map(HEAP, HEAP_SIZE, UC_PROT_ALL); u.mem_map(STACK_TOP - STACK_SIZE, STACK_SIZE, UC_PROT_ALL)
        u.mem_map(SENTINEL, 0x1000, UC_PROT_ALL)
        u.mem_map(TEB, 0x1000, UC_PROT_ALL); u.mem_map(GDT, 0x1000, UC_PROT_ALL)
        u.mem_write(TEB, struct.pack('<7I', 0xFFFFFFFF, STACK_TOP, STACK_TOP - STACK_SIZE, 0, 0, 0, TEB))
        def desc(base, lim, access, flags):
            return (lim & 0xFFFF) | ((base & 0xFFFFFF) << 16) | (access << 40) | (((lim >> 16) & 0xF) << 48) | (flags << 52) | ((base >> 24) << 56)
        u.mem_write(GDT + 8, struct.pack('<3Q', desc(0, 0xFFFFF, 0x9B, 0xC), desc(0, 0xFFFFF, 0x93, 0xC), desc(TEB, 0xFFF, 0x93, 0x4)))
        u.reg_write(UC_X86_REG_GDTR, (0, GDT, 4 * 8 - 1, 0x0))
        for reg, sel in ((UC_X86_REG_CS, 1), (UC_X86_REG_SS, 2), (UC_X86_REG_DS, 2), (UC_X86_REG_ES, 2), (UC_X86_REG_GS, 2), (UC_X86_REG_FS, 3)):
            u.reg_write(reg, sel << 3)
        self.next = HEAP + 0x1000; self.sizes = {}; self.stop = None
        u.hook_add(UC_HOOK_CODE, self._imp, begin=TRAP, end=TRAP + TRAP_SIZE - 1)

    def alloc(self, n):
        v = self.next; self.next += (max(n, 1) + 0xF & ~0xF) + 0x10; self.sizes[v] = n
        if self.next > HEAP + HEAP_SIZE: raise MemoryError('heap')
        return v

    HEAPFN = {   # stdcall argument count, answer
        'GetProcessHeap': (0, lambda s, a: 0x00150000),
        'HeapAlloc': (3, lambda s, a: s.alloc(a[2])),
        'HeapFree': (3, lambda s, a: 1),
        'HeapSize': (3, lambda s, a: s.sizes.get(a[2], 0xFFFFFFFF)),
        'HeapReAlloc': (4, lambda s, a: s.realloc(a[2], a[3])),
        # void; one thread, so entering always succeeds at once and nothing else changes
        'EnterCriticalSection': (1, lambda s, a: 0),
        'LeaveCriticalSection': (1, lambda s, a: 0),
        'InitializeCriticalSection': (1, lambda s, a: 0),
        'DeleteCriticalSection': (1, lambda s, a: 0),
        # D3DX's check of the finished tokens by d3d8.dll (0x102ECF59..0x102ED081): it opens
        # HKLM\SOFTWARE\Microsoft\Direct3D for LoadDebugRuntime, finds d3d8d.dll or d3d8.dll and calls its
        # ValidateVertexShader / ValidatePixelShader on the token buffer; the result only sets the error flag, the
        # buffer is not written. Answered as "no key" (ERROR_FILE_NOT_FOUND) and "module not loaded" (NULL), both
        # documented results, so the check is skipped: the tokens are the assembler's, not validated by d3d8.dll
        'RegOpenKeyA': (3, lambda s, a: 2),
        'GetModuleHandleA': (1, lambda s, a: 0),
        'LoadLibraryA': (1, lambda s, a: 0),
    }

    def realloc(self, p, n):
        v = self.alloc(n); old = self.sizes.get(p, 0)
        if old: self.u.mem_write(v, bytes(self.u.mem_read(p, min(old, n))))
        return v

    def _imp(self, u, addr, size, _):
        name = self.names.get(addr, '?')
        if name not in self.HEAPFN:
            esp = u.reg_read(UC_X86_REG_ESP); ret, *a = struct.unpack('<5I', u.mem_read(esp, 20))
            def show(v):
                try: t = bytes(u.mem_read(v, 64)).split(b'\0')[0]
                except UcError: t = b''
                return repr(t.decode('latin-1')) if len(t) > 2 and all(32 <= c < 127 for c in t) else '0x%08X' % v
            self.stop = 'import %s(%s) called from 0x%08X: not modelled' % (name, ', '.join(map(show, a)), ret)
            u.emu_stop(); return
        n, f = self.HEAPFN[name]; esp = u.reg_read(UC_X86_REG_ESP)
        ret, *a = struct.unpack('<%dI' % (n + 1), u.mem_read(esp, 4 * (n + 1)))
        u.reg_write(UC_X86_REG_EAX, f(self, a) & 0xFFFFFFFF)
        u.reg_write(UC_X86_REG_ESP, esp + 4 * (n + 1)); u.reg_write(UC_X86_REG_EIP, ret)

    def call(self, fn, args):
        u = self.u; esp = STACK_TOP - 0x2000 - 4 * (len(args) + 1)
        u.mem_write(esp, struct.pack('<%dI' % (len(args) + 1), SENTINEL, *args))
        u.reg_write(UC_X86_REG_ESP, esp); self.stop = None
        try: u.emu_start(fn, SENTINEL, count=50_000_000)
        except UcError as e: self.stop = self.stop or 'fault: %s at 0x%08X' % (e, u.reg_read(UC_X86_REG_EIP))
        if self.stop: raise RuntimeError(self.stop)
        if u.reg_read(UC_X86_REG_EIP) != SENTINEL: raise RuntimeError('did not return (instruction limit)')
        return u.reg_read(UC_X86_REG_EAX)

    def assemble(self, text, flags):
        src = self.alloc(len(text) + 1); self.u.mem_write(src, text + b'\0')
        out = self.alloc(4); self.u.mem_write(out, b'\0' * 4)
        hr = self.call(ASSEMBLE, [src, len(text), flags, 0, out, 0])
        if hr & 0x80000000: return hr, None
        buf = struct.unpack('<I', self.u.mem_read(out, 4))[0]
        vt = struct.unpack('<I', self.u.mem_read(buf, 4))[0]
        get = lambda k: struct.unpack('<I', self.u.mem_read(vt + 4 * k, 4))[0]
        p, n = self.call(get(3), [buf]), self.call(get(4), [buf])        # GetBufferPointer, GetBufferSize
        return hr, bytes(self.u.mem_read(p, n))


def main(out_dir):
    a = Asm(os.environ['FFXI_DLL']); os.makedirs(out_dir, exist_ok=True); cat = []
    sys.path.insert(0, H); import vshader
    dl = declaration_of(a.img)
    for n, (table, count, kind, site) in enumerate(shaders_of(a.img)):
        for flag in (0, 1):
            text = text_of(a.img, table, count, flag)
            hr, tok = a.assemble(text, 0 if kind == 'vs' else 2)
            name = '%s%d_flag%d' % (kind, n if kind == 'vs' else 0, flag)
            r = {'name': name, 'kind': kind, 'table': '0x%08X' % table, 'lines': count, 'cdx_920': flag,
                 'source_lines': text.count(b'\n'), 'built_at': '0x%08X' % site, 'd3dx_flags': 0 if kind == 'vs' else 2,
                 'hresult': '0x%08X' % hr}
            if kind == 'vs': r['declaration'] = vshader.decl(dl)
            if tok is not None:
                r.update(tokens=len(tok) // 4, sha256=hashlib.sha256(tok).hexdigest())
                open(os.path.join(out_dir, name + '.tok'), 'wb').write(tok)
            open(os.path.join(out_dir, name + '.txt'), 'wb').write(text)
            cat.append(r)
            print('%-12s table 0x%08X, %2d of %2d lines, %s' % (name, table, r['source_lines'], count,
                  '%d tokens, sha256 %s' % (r['tokens'], r['sha256'][:16]) if tok is not None else 'D3DXAssembleShader failed: ' + r['hresult']))
    same = sum(1 for x, y in zip(cat[::2], cat[1::2]) if x.get('sha256') == y.get('sha256'))
    print('%d shaders, 2 variants each; %d of them assemble the same with either flag' % (len(cat) // 2, same))
    json.dump({'source': 'kit/d3dprof/shader_catalogue.py (the game\'s own D3DXAssembleShader, in Unicorn; d3d8.dll validation '
                         'not run)', 'shaders': cat},
              open(os.path.join(ROOT, 'data', 'shader_catalogue.json'), 'w'), indent=1)


if __name__ == '__main__':
    main(sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'runs', 'shaders'))
