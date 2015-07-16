#!/usr/bin/python
'''
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
'''

__author__ = 'Lucas van Oosterhout'
import matplotlib.pyplot as plt
import numpy as np

mu, sigma = np.log(2.0e-4), 0.6
maxV, minV = 3.0e-4, 1.5e-4

s = np.random.lognormal(mu, sigma, 100000)
trunc = []

for v in s:
    if minV < v < maxV:
        trunc.append(v)

s = np.array(trunc)

#Plot
count, bins, ignored = plt.hist(s, 100, normed=True, align='mid')
plt.xlabel('Pore body Size')
plt.ylabel('Frequency')
plt.title(r'Pb Size Distribution: $\mu={}$, $\sigma={}$'.format(np.e**mu, sigma))
plt.subplots_adjust(left=0.15)
plt.savefig('hist.pdf')

f = open('pb_size.txt', 'w')
for size in s:
    f.write(str(size))
    f.write('\n')
f.close()

f = open('pb_dist.txt', 'w')
f.write('mu: {}, sigma: {}\n'.format(np.exp(mu), sigma))
f.write('max: {}, min: {}\n'.format(maxV, minV))
f.write('nr of Pb_sizes: {}\n'.format(len(s)))
f.close()


mean = 2.0e-4
stdDev = 0.6
maxpbSize = 3.0e-4
minPbSize = 0.75e-4

