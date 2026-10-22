"""name_bench_score.py -- score naming runs on name_bench.py dossiers: per model and ground-truth source, names
written and mean word overlap with the true name; writes rows.json for the judge (see docs/PROBLEMS.md P16).
    python3 name_bench_score.py MODEL [MODEL ...]   (run in the bench directory: key.json, out_MODEL/*.json)"""
import json,glob,re,os,collections,sys
B=os.getcwd()
key=json.load(open(B+'/key.json'))
def preds(m):
    out={}
    for f in glob.glob(B+'/out_%s/*.json'%m):
        try: d=json.load(open(f))
        except Exception as e: print('bad json',f,e); continue
        for k,v in d.items():
            if isinstance(v,dict) and v.get('name'): out['0x%08X'%int(k,16)]=v
    return out
def words(n):
    n=re.sub(r'^(C(?=[A-Z][a-z]))','',n)
    w=re.findall(r'[A-Z]?[a-z]+|[A-Z]+(?![a-z])|\d+',n.replace('::','_'))
    return {x.lower() for x in w if x.lower() not in ('unit','fn','sub','c')}
rows=[]
for m in sys.argv[1:]:
    P=preds(m)
    for a,v in key.items():
        p=P.get(a)
        rows.append({'model':m,'addr':a,'source':v['source'],'truth':v['truth'],'pred':p['name'] if p else None,
                     'conf':p['confidence'] if p else None,'summary':p.get('summary') if p else None,
                     'overlap':(len(words(p['name'])&words(v['truth']))/len(words(v['truth'])|words(p['name']))) if p else None})
json.dump(rows,open(B+'/rows.json','w'),indent=1)
for m in sys.argv[1:]:
    for src in ('ps2','dm'):
        R=[r for r in rows if r['model']==m and r['source']==src]
        named=[r for r in R if r['pred']]
        print(m,src,'n',len(R),'named',len(named),'mean word overlap %.2f'%(sum(r['overlap'] for r in named)/max(1,len(named))),
              'by conf',{c:len([r for r in named if r['conf']==c]) for c in ('high','medium','low')})
