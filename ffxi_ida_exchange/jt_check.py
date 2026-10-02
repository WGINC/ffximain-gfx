# Runs inside IDA, read-only. Finds pipeline "function starts" that are really pointer tables.
import json, os
D=r"C:\Users\wadeglover\Downloads\ffxi_ida_exchange"
P=json.load(open(os.path.join(D,"pipeline_functions.json")))
TXT=(0x10001000,0x10329000)
def is_txt(v): return TXT[0]<=v<TXT[1]
bad=[]
for k,v in P.items():
    a=int(k,16)
    d0=ida_bytes.get_dword(a); d1=ida_bytes.get_dword(a+4)
    if is_txt(d0) and is_txt(d1):
        # count run length of text pointers
        n=0
        while is_txt(ida_bytes.get_dword(a+4*n)) and n<4096: n+=1
        size=sum(int(e,16)-int(s,16) for s,e in v['chunks'])
        # IDA functions whose starts are swallowed by this pipeline entry
        swallowed=[]
        for s,e in v['chunks']:
            for f in idautils.Functions(int(s,16), int(e,16)):
                if f!=a: swallowed.append(hex(f))
        # the function owning the targets (IDA)
        owner=ida_funcs.get_func(d0)
        bad.append({"start":k,"ptr_run":n,"pipeline_size":size,"ninsn":v['ninsn'],"swallowed_ida_funcs":swallowed,
                    "targets_owner_ida":hex(owner.start_ea) if owner else None})
json.dump(bad,open(os.path.join(D,"jt_check_out.json"),"w"),indent=1)
result={"n":len(bad),"total_swallowed":sum(len(b["swallowed_ida_funcs"]) for b in bad),"first":bad[:5]}
