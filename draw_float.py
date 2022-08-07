import matplotlib.pyplot as plt
import numpy as np

#[S, E, P] = [1, 5, 2]
[S, E, P] = [1, 4, 3]
#[S, E, P] = [1, 6, 9]
#[S, E, P] = [1, 8, 23]
#[S, E, P] = [1, 5, 10]



NUM = sum([S, E, P])
NUM_EP = E + P
bias = (2 ** E) / 2 - 1

vs = []
for i in range(0, 2 ** NUM):
    if i == 0:
        v = 0
    else:
        minus = (i >> NUM_EP) & 1 
        e = (i >> P) & ((1 << E) - 1)
        p = i & ((1 << P) - 1)
        if e == (1 << E) - 1:
            continue
        p |= (1 << P)
        s = str(bin(p))
        assert s.startswith('0b')
        s = s[2:]
        assert len(s) == P + 1, (len(s), P)
        r = 1.0
        v = 0
        for u in s:
            if u == '1':
                v += r
            r *= 0.5
        v *= (2 ** (e - bias)) - 0
        if minus:
            v = -v
    vs.append(v)
vs.sort()

#vs = list(filter(lambda x: x >= -1000 and x <= 1000, vs))

#plt.hist(vs, bins=NUM)
#plt.title('FP8 (1-5-2)')
#plt.title('FP8 (1-4-3)')
plt.title('FP8 (1-4-3), bias=4')
#plt.title('FP16 (1-6-9)')
plt.plot(vs, vs, 'o')
plt.show()
