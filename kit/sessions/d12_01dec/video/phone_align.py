import numpy as np,struct,sys,glob,os,math
d=sys.argv[1]
ph=np.load('phone_sx.npy')
rows=[];hdr=None
for l in open(glob.glob(os.path.join(d,'xikit_frames_*.csv'))[0],errors='replace'):
    if l.startswith('frame,'): hdr=l.strip().split(','); continue
    if hdr and l[:1].isdigit(): rows.append(l.rstrip('\r\n').split(','))
ix={k:i for i,k in enumerate(hdr)}
F=lambda h:struct.unpack('<f',struct.pack('<I',int(h,16)))[0]
T=[];Y=[]
for p in rows:
    try: ex,ez,tx,tz=F(p[ix['cex']]),F(p[ix['cez']]),F(p[ix['ctx']]),F(p[ix['ctz']])
    except: continue
    T.append(int(p[1])/24e6); Y.append(math.atan2(tz-ez,tx-ex))
T=np.array(T); Y=np.unwrap(np.array(Y)); np.save('cam_T.npy',T); np.save('cam_Y.npy',Y)
recq=21903.965
bins=np.arange(recq-120,recq+150,0.125)
yr=np.diff(np.interp(bins,T,Y))
pm=ph[:len(ph)//30*30].reshape(-1,30).sum(axis=1)
best=[]
for off in range(0,len(yr)-len(pm)):
    seg=yr[off:off+len(pm)]
    cc=np.corrcoef(seg,pm)[0,1]; best.append((abs(cc),cc,off))
best.sort(reverse=True)
for a,cc,off in best[:6]: print('phone 0 s = rec mark %+.3f s: corr %.3f' % (bins[off]-recq, cc))
