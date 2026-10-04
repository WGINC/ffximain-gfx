import json,sys
# addarr.py BASE COUNT STRIDE field,field,... PREFIX CONF EVIDENCE : name every dossier global inside the array
base=int(sys.argv[1],16); n=int(sys.argv[2]); st=int(sys.argv[3]); fields=sys.argv[4].split(','); pre=sys.argv[5]; conf=sys.argv[6]; ev=sys.argv[7]
W=4
D={int(json.loads(l)['ea'],16):json.loads(l) for l in open('/home/user/ffximain-gfx/runs/globals/dossier.jsonl')}
done={l.split('\t')[0] for l in open('/home/user/ffximain-gfx/runs/globals/names.tsv')}
out=open('/home/user/ffximain-gfx/runs/globals/names.tsv','a')
for a in sorted(D):
    if not base<=a<base+n*st or '0x%08X'%a in done: continue
    k,off=divmod(a-base,st)
    f=fields[off//W] if fields!=[''] and off%W==0 and off//W<len(fields) else 'b%X'%off
    nm=pre if a==base else '%s_v%d_%s'%(pre,k,f)
    out.write('0x%08X\t%s\t%s\t%s (vertex %d, +%d = %s)\n'%(a,nm,conf,ev,k,off,f))
    print('0x%08X'%a,nm)
