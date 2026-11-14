import numpy as np,csv,glob,os,sys,collections
d=sys.argv[1]; base=np.load('align.npy')[0]; ph=np.load('phone_sx.npy')
P=[]
for r in csv.DictReader(open(glob.glob(os.path.join(d,'presentmon_two_*.csv'))[0],encoding='utf-8-sig')):
    if r['Application']!='pol.exe' or r['SwapChainAddress']=='0x0': continue
    P.append((int(r['CPUStartQPC'])/24e6,r['DisplayedTime'],float(r['FrameTime'])))
G=[]
for l in open(glob.glob(os.path.join(d,'2026-11-13_223830_3080','xikit_frames_*.csv'))[0],errors='replace'):
    p=l.split(',')
    if p[0].isdigit(): G.append((int(p[1])/24e6,float(p[2])))
def hist(g): 
    h=collections.Counter(min(round(x/8.333),6) for x in g); n=len(g)
    return ', '.join('%s: %d%%' % ('%d'%k if k<6 else '6+', round(100*h[k]/n)) for k in sorted(h) if k>0)
for a,b in ((7.8,9.8),(35.6,37.8),(61.8,63.8)):
    steps=[(i+1)/240 for i in range(int(a*240),int(b*240)) if abs(ph[i])>=2]
    # merge steps closer than 6 ms (a refresh's change can span two phone frames)
    m=[]
    for s in steps:
        if not m or s-m[-1]>0.006: m.append(s)
    pg=[(y-x)*1000 for x,y in zip(m,m[1:])]
    w=[p for p in P if base+a<=p[0]<base+b]
    dg=[float(p[1]) for p in w if p[1] not in ('NA','')]
    gf=[f for q,f in G if base+a<=q<base+b]
    print('phone %.1f-%.1f s (a turn):' % (a,b))
    print('   game: %d frames (%.0f/s), longest %.0f ms' % (len(gf), len(gf)/(b-a), max(gf)))
    print('   Windows shown: %d (%.0f/s); held for refreshes: %s' % (len(dg), len(dg)/(b-a), hist(dg)))
    print('   panel (phone): %d new pictures (%.0f/s); gaps in refreshes: %s' % (len(m), len(m)/(b-a), hist(pg)))
