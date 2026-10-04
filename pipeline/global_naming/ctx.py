import json,re,sys
# ctx.py SYMBOL [N]: every export line mentioning SYMBOL with N lines of context, grouped by function
sym=sys.argv[1]; n=int(sys.argv[2]) if len(sys.argv)>2 else 2
for l in open('/home/user/ffximain-gfx/data/exports/gfx_export.jsonl'):
    r=json.loads(l); L=r['text'].split('\n')
    hits=[i for i,x in enumerate(L) if re.search(r'\b%s\b'%re.escape(sym),x)]
    if not hits: continue
    print('#### %s 0x%X'%(r['name'],r['ea'])); shown=set()
    for i in hits:
        for j in range(max(0,i-n),min(len(L),i+n+1)):
            if j not in shown: print('%5d %s'%(j,L[j][:200])); shown.add(j)
        print('   ...')
