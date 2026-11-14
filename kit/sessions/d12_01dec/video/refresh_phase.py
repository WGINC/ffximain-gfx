import csv,glob,os,sys,numpy as np
d=sys.argv[1]; R=1000/120.0
for name,pf in (('one (2 buffers)','presentmon_one_*.csv'),('two (3 buffers)','presentmon_two_*.csv')):
    P=[]
    for r in csv.DictReader(open(glob.glob(os.path.join(d,pf))[0],encoding='utf-8-sig')):
        if r['Application']!='pol.exe' or r['SwapChainAddress']=='0x0': continue
        try: P.append((int(r['CPUStartQPC'])/24e3, float(r['FrameTime']), r['DisplayLatency']))
        except: pass
    P=P[len(P)//5:]   # past login
    # the refresh grid, from the screen times of shown frames, in 2 s pieces (it drifts)
    out=[];nashort=[];shshort=[]
    for k in range(0,len(P)-300,250):
        seg=P[k:k+250]
        scr=np.array([c+float(l) for c,f,l in seg if l not in ('NA','')])
        if len(scr)<20: continue
        ph=np.angle(np.mean(np.exp(2j*np.pi*scr/R)))          # the grid's phase
        cpu=np.array([c for c,f,l in seg])
        out.append(np.exp(2j*np.pi*cpu/R-1j*ph))
    z=np.concatenate(out)
    print('%s: the game frames\' start, against the screen refresh: concentration %.3f (0 = no relation, 1 = locked)' % (name,abs(z.mean())))
    hist=np.histogram((np.angle(z)/(2*np.pi)*R)%R,bins=8,range=(0,R))[0]
    print('   share per 1/8 of a refresh: ' + ' '.join('%.0f%%' % (100*h/len(z)) for h in hist))
    # hypothesis 2: a frame not shown is one followed by another within the same refresh
    for i in range(len(P)-1):
        (nashort if P[i][2] in ('NA','') else shshort).append(P[i][1])
    print('   frame time to the next frame: not shown median %.1f ms (%.0f%% under %.1f ms); shown median %.1f ms (%.0f%% under)' % (
        np.median(nashort), 100*np.mean(np.array(nashort)<R), R, np.median(shshort), 100*np.mean(np.array(shshort)<R)))
