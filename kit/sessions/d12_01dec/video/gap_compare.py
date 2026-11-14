import numpy as np
recq=21903.965; P0=recq+11.685; M0=recq-5.955
ph=np.load('phone_sx.npy'); msx=np.load('mac_sx.npy'); mts=np.load('mac_ts.npy')
def phone_steps(a,b):  # absolute qpc times of new pictures on the panel
    st=[]
    for i in range(int((a-P0)*240),int((b-P0)*240)):
        if abs(ph[i])>=2:
            t=P0+(i+1)/240
            if not st or t-st[-1]>0.006: st.append(t)
    return np.array(st)
def mac_steps(a,b,thr=3):
    t=M0+mts[1:]; m=(t>=a)&(t<b)&(np.abs(msx)>=thr)
    return t[m]
def gaps(s): return [(x,y) for x,y in zip(s,s[1:]) if y-x>=0.030]
for pa,pb in ((7.8,9.8),(35.6,37.8),(61.8,63.8)):
    a,b=P0+pa,P0+pb
    ps,ms=phone_steps(a,b),mac_steps(a,b)
    pg,mg=gaps(ps),gaps(ms)
    # a phone gap is matched when the mac has no new picture strictly inside it (allow 6 ms at the edges)
    matched=sum(1 for x,y in pg if not np.any((ms>x+0.006)&(ms<y-0.006)))
    back=sum(1 for x,y in mg if not np.any((ps>x+0.006)&(ps<y-0.006)))
    print('turn at phone %.1f s: phone %d pictures (%d gaps of 30+ ms), Mac recording %d (%d gaps of 30+ ms)' % (pa,len(ps),len(pg),len(ms),len(mg)))
    print('   phone gaps with no Mac picture inside: %d of %d; Mac gaps with no phone picture inside: %d of %d' % (matched,len(pg),back,len(mg)))
