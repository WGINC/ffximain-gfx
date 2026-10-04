import json,sys
# todo.py START COUNT : the dossier text of globals not yet in names.tsv / skip.tsv
done=set()
for f in ('names.tsv','skip.tsv'):
    try: done|={l.split('\t')[0] for l in open('/home/user/ffximain-gfx/runs/globals/'+f)}
    except FileNotFoundError: pass
L=[json.loads(l) for l in open('/home/user/ffximain-gfx/runs/globals/dossier.jsonl')]
todo=[o for o in L if o['ea'] not in done]
s,n=int(sys.argv[1]),int(sys.argv[2])
print('%d left'%len(todo))
for o in todo[s:s+n]:
    print('== %s %s type=%s size=%s %s init=%s'%(o['ea'],o['name'],o['type'],o['size'],o['section'],json.dumps(o['init'])))
    print('   uses: '+', '.join('%s x%d'%(k,v) for k,v in sorted(o['kinds'].items(),key=lambda x:-x[1])))
    print('   in: '+', '.join('%s(%d)'%(k,v) for k,v in list(o['functions'].items())[:8]))
    for l in o['lines'][:5]: print('   | '+l[:170])
