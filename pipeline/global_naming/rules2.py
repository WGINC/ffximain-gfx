import json,re
# (1) MSVC static-init guard bytes; (2) globals only one function in the whole image references -> <F>_static_XXXX
D=[json.loads(l) for l in open('/home/user/ffximain-gfx/runs/globals/dossier.jsonl')]
done=set()
for f in ('names.tsv','skip.tsv'): done|={l.split('\t')[0] for l in open('/home/user/ffximain-gfx/runs/globals/'+f)}
pend=json.load(open('/home/user/ffximain-gfx/data/names_pending_idb.json'))
def fname(f):
    return f if not f.startswith('0x') else 'Fn'+f[2:]
out=open('/home/user/ffximain-gfx/runs/globals/names.tsv','a'); n1=n2=0
for o in D:
    if o['ea'] in done: continue
    fns=list(o['functions']); data=[r for r in o['refs'] if 'from_data' in r]
    if len(fns)!=1 or data: continue
    F=fname(fns[0]).replace('sub_','Fn')
    nm=o['name']; guard=all(re.search(r'\(\s*%s & \w+\) == 0|%s \|= \w+'%(nm,nm),l) for l in o['lines']) and o['lines']
    if guard and o['type'].startswith(('char','unsigned __int8')) or (guard and o['size']==1):
        out.write('%s\t%s_staticGuards\thigh\tevery use is (g & bit) == 0 followed by g |= bit, all in %s: the MSVC init guards of that function\'s local statics\n'%(o['ea'],F,fns[0])); n1+=1
    else:
        out.write('%s\t%s_static_%s\thigh\tscope only: every reference in the whole image (%d) is in %s, so it is that function\'s static; its meaning is not named\n'%(o['ea'],F,o['ea'][-4:],sum(o['kinds'].values()),fns[0])); n2+=1
print('guards',n1,'single-function statics',n2)
