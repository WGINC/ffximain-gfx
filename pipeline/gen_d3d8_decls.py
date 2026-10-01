"""Generate IDA C declarations for the Direct3D 8 COM interfaces (vtable structs, method order
exactly as MinGW-w64 d3d8.h) and the enums that make device calls readable."""
import re, json
H='/usr/share/mingw-w64/include/'
src=open(H+'d3d8.h').read(); tsrc=open(H+'d3d8types.h').read()
ENUMS=['D3DRENDERSTATETYPE','D3DTRANSFORMSTATETYPE','D3DTEXTURESTAGESTATETYPE','D3DPRIMITIVETYPE','D3DFORMAT','D3DPOOL','D3DRESOURCETYPE','D3DDEVTYPE','D3DMULTISAMPLE_TYPE','D3DBACKBUFFER_TYPE','D3DSTATEBLOCKTYPE']
KEEP=set(ENUMS)
def ptype(t):
    t=t.strip().replace('CONST ','const ').replace('struct ','')
    t=re.sub(r'\s+',' ',t)
    base=t.replace('const ','').replace('*','').strip()
    stars=t.count('*')
    if base.startswith('IDirect3D') and stars: return base+' '+'*'*stars
    if stars: return 'void '+'*'*stars
    if base in KEEP: return base
    if base=='float': return 'float'
    return 'unsigned int'
ifaces={}; order=[]
for m in re.finditer(r'DECLARE_INTERFACE_\((\w+),(\w+)\)\s*\{(.*?)\n\};',src,re.S):
    name,base,body=m.groups()
    meths=[]
    for mm in re.finditer(r'STDMETHOD(?:_\(([^,]+),\s*(\w+)\s*\)|\((\w+)\))\s*\((.*?)\)\s*PURE;',body,re.S):
        ret,n1,n2,params=mm.groups(); n=(n1 or n2).strip(); ret=(ret or 'HRESULT').strip()
        params=params.replace('THIS_','').replace('THIS','').strip()
        ps=[]
        for p in [p.strip() for p in params.split(',') if p.strip()]:
            pm=re.match(r'(.*?)(\w+)\s*(\[\s*\d*\s*\])?$',p)
            ty,pn=pm.group(1),pm.group(2)
            if not ty.strip(): ty,pn=pn,'a%d'%len(ps)
            ps.append((ptype(ty),pn))
        rt='float' if ret=='float' else 'int'
        meths.append((n,rt,ps))
    ifaces[name]=meths; order.append(name)
enums=[]
for e in ENUMS:
    m=re.search(r'typedef enum _?%s\s*\{(.*?)\}\s*%s\s*;'%(e,e),tsrc,re.S)
    if not m: print('enum missing',e); continue
    body=re.sub(r'/\*.*?\*/','',m.group(1),flags=re.S); body=re.sub(r'//[^\n]*','',body)
    items=[]
    for it in body.split(','):
        it=it.strip()
        if not it or it.startswith('#'): continue
        k,_,v=it.partition('=')
        items.append('%s%s'%(k.strip(),' = '+v.strip() if v else ''))
    items=[i for i in items if not i.endswith('FORCE_DWORD = 0x7fffffff') and 'FORCE_DWORD' not in i]
    enums.append('enum %s : unsigned int {\n  %s\n};'%(e,',\n  '.join(items)))
out=['// generated from MinGW-w64 d3d8.h / d3d8types.h']
out+=enums
for n in order: out.append('struct %s;'%n)
for n in order:
    lines=[]
    for mn,rt,ps in ifaces[n]:
        args=', '.join(['%s *This'%n]+['%s %s'%(t,p) for t,p in ps])
        lines.append('  %s (__stdcall *%s)(%s);'%(rt,mn,args))
    out.append('struct %sVtbl {\n%s\n};'%(n,'\n'.join(lines)))
    out.append('struct %s { struct %sVtbl *lpVtbl; };'%(n,n))
open('d3d8_ida.h','w').write('\n'.join(out)+'\n')
json.dump({n:[m[0] for m in ifaces[n]] for n in order},open('d3d8_methods.json','w'),indent=0)
print({n:len(v) for n,v in ifaces.items()})
