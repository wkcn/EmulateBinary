import matplotlib.pyplot as plt
import numpy as np

# [S, E, P] = [1, 5, 2]
[S, E, P] = [1, 6, 9]



NUM = sum([S, E, P])
NUM_EP = E + P
bias = (2 ** E) / 2 - 1

vs = []
for i in range(0, 2 ** NUM):
    minus = (i >> NUM_EP) & 1 
    e = (i >> P) & ((1 << E) - 1)
    p = i & ((1 << P) - 1)
    v = (1 + 2 ** (-p)) * (2 ** (e - bias))
    if minus:
        v = -v
    vs.append(v)
vs.sort()

#plt.hist(vs, bins=NUM)
# plt.title('FP8 (1-5-2)')
plt.title('FP16 (1-6-9)')
plt.plot(vs, vs, 'o')
plt.show()
