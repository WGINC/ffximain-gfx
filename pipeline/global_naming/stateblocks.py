import json,re,collections
# names for state-block handles and tables from the calls that use them (the call fixes each argument's role)
D={int(json.loads(l)['ea'],16):json.loads(l) for l in open('/home/user/ffximain-gfx/runs/globals/dossier.jsonl')}
byname={o['name']:a for a,o in D.items()}
pend=json.load(open('/home/user/ffximain-gfx/data/names_pending_idb.json'))
done={l.split('\t')[0] for l in open('/home/user/ffximain-gfx/runs/globals/names.tsv')}
CALLS=[(re.compile(r'StateBlock_ApplyTSS\(&?(\w+), \(int\)&?(\w+)\)'),('stateBlock','textureStageStates')),
       (re.compile(r'StateBlock_ApplyRSTSS\(&?(\w+), \(int\)&?(\w+), \(int\)&?(\w+)\)'),('stateBlock','renderStates','textureStageStates')),
       (re.compile(r'sub_1006AF00\(&?(\w+), &?(\w+)\)'),('stateBlock','renderStates'))]
WHY={'stateBlock':'the state-block handle (a DWORD holding -1 until the block is recorded, then the ApplyStateBlock token)',
     'renderStates':'the render-state table ((state, value) pairs ending 0x7FFFFFFF, set with SetRenderState while recording)',
     'textureStageStates':'the texture-stage table ((stage, type, value) triples ending 0x7FFFFFFF, set with SetTextureStageState while recording)'}
FN={'sub_1006AF00':'StateBlock_ApplyRS (0x1006AF00, which records SetRenderState only)'}
out=open('/home/user/ffximain-gfx/runs/globals/names.tsv','a'); used=collections.Counter(); seen=set()
for l in open('/home/user/ffximain-gfx/data/exports/gfx_export.jsonl'):
    r=json.loads(l); fn=r['name']
    fname=pend.get('0x%08X'%r['ea'],{}).get('name',fn)
    if fname.startswith('sub_'): fname='Fn%s'%fname[4:]
    k=collections.Counter()
    for rx,roles in CALLS:
        for m in rx.finditer(r['text']):
            for g,role in zip(m.groups(),roles):
                a=byname.get(g)
                if a is None or '0x%08X'%a in done or a in seen: continue
                k[role]+=1; n='%s_%s%s'%(fname,role,'' if k[role]==1 else k[role])
                callee=m.group(0).split('(')[0]
                out.write('0x%08X\t%s\thigh\t%s passes it to %s as %s\n'%(a,n,fn,FN.get(callee,callee),WHY[role])); seen.add(a); print('0x%08X'%a,n)
