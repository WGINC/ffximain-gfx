import numpy as np
ts=np.array([float(l.split(',')[0]) for l in open('mac_fts.csv') if l.strip()])
r=np.fromfile('macrow.raw',dtype=np.uint8).reshape(-1,2200).astype(np.float32)
n=min(len(ts),len(r)); ts=ts[:n]; r=r[:n]
r=r-r.mean(axis=1,keepdims=True); r*=np.hanning(2200)[None,:]
Fr=np.fft.rfft(r,n=4400,axis=1); X=Fr[1:]*np.conj(Fr[:-1]); c=np.fft.irfft(X,n=4400,axis=1)
L=120; c=np.concatenate([c[:,-L:],c[:,:L+1]],axis=1); sx=(np.argmax(c,axis=1)-L).astype(float)
np.save('mac_sx.npy',sx); np.save('mac_ts.npy',ts)
T=np.load('cam_T.npy'); Y=np.load('cam_Y.npy'); recq=21903.965
st=0.125; bins=np.arange(0,ts[-1],st)
pm=np.array([sx[(ts[1:]>=a)&(ts[1:]<a+st)].sum() for a in bins])
best=[]
for off in np.arange(-150,60,0.125):
    y=np.diff(np.interp(recq+off+np.append(bins,bins[-1]+st),T,Y))
    if np.std(y)==0: continue
    cc=np.corrcoef(y,pm)[0,1]; best.append((abs(cc),cc,off))
best.sort(reverse=True)
for a,cc,off in best[:4]: print('mac 0 s = rec mark %+.3f s: corr %.3f' % (off,cc))
o=best[0][2]
for a,b in ((0,30),(30,60),(60,90),(90,128)):
    tt=np.arange(a,b,1/40); p=np.array([sx[(ts[1:]>=x)&(ts[1:]<x+1/40)].sum() for x in tt])
    bb=max(((np.corrcoef(np.diff(np.interp(recq+off2+np.append(tt,b),T,Y)),p)[0,1] if np.std(p)>0 else 0,off2) for off2 in np.arange(o-0.5,o+0.5,0.005)), key=lambda z:abs(z[0]))
    print('  mac %d-%d s: fine offset %+.3f, corr %.3f' % (a,b,bb[1],bb[0]))
