import csv,sys,glob,os,re,statistics as st,collections
d=sys.argv[1]
def frames(dirn):
    rows=[];hdr=None
    for l in open(glob.glob(os.path.join(d,dirn,'xikit_frames_*.csv'))[0],errors='replace'):
        if l.startswith('frame,'): hdr=l.strip().split(','); continue
        if hdr and l[:1].isdigit(): rows.append(dict(zip(hdr,l.rstrip('\r\n').split(','))))
    log=open(os.path.join(d,dirn,'xikit.log'),errors='replace').read()
    m={t.strip():int(f) for n,f,t in re.findall(r'measure: mark (\d+) at frame (\d+) \(mark\.bat\): (.*)',log)}
    return rows,m
def pm(f):
    P=[]
    for r in csv.DictReader(open(f,encoding='utf-8-sig')):
        if r['Application']!='pol.exe' or r['SwapChainAddress']=='0x0': continue
        try: P.append((int(r['CPUStartQPC'])/24e6,float(r['FrameTime']),r['DisplayedTime'],r['PresentMode'],float(r['GPUBusy']),r['DisplayLatency']))
        except: pass
    return P
def pct(a,p): a=sorted(a); return a[int(p/100*(len(a)-1))]
for name,dirn,pf in (('one (2 buffers)','2026-11-13_223100_10652','presentmon_one_*.csv'),('two (3 buffers)','2026-11-13_223830_3080','presentmon_two_*.csv')):
    rows,m=frames(dirn); P=pm(glob.glob(os.path.join(d,pf))[0])
    q0=int(next(r['qpc'] for r in rows if int(r['frame'])>=m['go']))/24e6
    q1=q0+180
    w=[p for p in P if q0<=p[0]<q1]
    fr=[float(r['frame_ms']) for r in rows if q0<=int(r['qpc'])/24e6<q1]
    disp=[float(p[2]) for p in w if p[2] not in ('NA','') and float(p[2])>0]
    lat=[float(p[5]) for p in w if p[5] not in ('NA','')]
    held=collections.Counter(min(round(x/8.333),5) for x in disp)
    T=w[-1][0]-w[0][0]
    print('%s: %.0f s from mark go; modes %s' % (name,T,collections.Counter(p[3] for p in w).most_common(2)))
    print('   game frames %.1f/s; frame ms median %.2f, p95 %.2f, p99 %.2f; over 25 ms: %d' % (len(fr)/T, st.median(fr), pct(fr,95), pct(fr,99), sum(1 for x in fr if x>25)))
    print('   shown by Windows %.1f/s (%.0f%% of presents); display latency median %.1f ms, p95 %.1f' % (len(disp)/T, 100*len(disp)/len(w), st.median(lat), pct(lat,95)))
    print('   time on screen, refreshes: ' + ', '.join('%s: %.1f/s' % (k if k<5 else '5+', held[k]/T) for k in sorted(held)))
    print('   GPU busy median %.1f ms, p99 %.1f' % (st.median(p[4] for p in w), pct([p[4] for p in w],99)))
