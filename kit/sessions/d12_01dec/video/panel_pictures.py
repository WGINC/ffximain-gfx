import numpy as np,collections,sys
def pictures(fn):
    r=np.fromfile(fn,dtype=np.uint8).reshape(-1,1700).astype(np.float32)[:,200:1500]
    d=np.abs(np.diff(r,axis=0)).mean(axis=1)
    pk=[i for i in range(2,len(d)-2) if d[i]>=d[i-1] and d[i]>d[i+1] and d[i]-min(d[i-2:i+3])>1.5]
    mv=np.convolve(d,np.ones(120)/120,mode='same')>2.5     # moving: 0.5 s mean change above 2.5
    return np.array(pk),mv,len(d)
for fn,name,R in (('1_INulWLfAMVYUrgDXRUIL-_-9h_Ad9_G.row','ProMotion',1000/120),('1Z8lxcqNYfIlU7BsyjhzslaC3F22don_7.row','fixed 60 Hz',1000/60)):
    pk,mv,n=pictures(fn)
    g=[(b-a)*1000/240 for a,b in zip(pk,pk[1:]) if mv[a] and mv[b]]
    secs=mv.sum()/240
    pics=sum(1 for p in pk if mv[p])
    h=collections.Counter(min(int(round(x/R)),5) for x in g)
    print('%s: moving %.1f s of %.1f; %.0f new pictures a second (the refresh allows %.0f)' % (name,secs,n/240,pics/secs,1000/R))
    print('   gaps in refreshes: '+', '.join('%s: %.0f%%' % (k if k<5 else '5+',100*h[k]/len(g)) for k in sorted(h)))
    print('   gaps of 33 ms or more: %.1f a second; 50 ms or more: %.1f a second' % (sum(1 for x in g if x>=31)/secs, sum(1 for x in g if x>=48)/secs))
