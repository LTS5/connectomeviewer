""" This script provides a toy example of the NBS """

import numpy as np
import cviewer.libs.pyconto.algorithms.statistics.nbs as nbs
from pylab import imshow, show, title

# Generate simulated data.
# Generate population X. In particular, generate 10 instantiations of a 
# 20 x 20 connectivty matrix, where each element is sampled from a standard 
# normal distribution. Each connectivity matrix represents a distinct member
# of population X. Elements below the main diagonal are never used.

X = np.random.random( (20,20,10) )
n = X.shape[0]

# Generate population Y in the same way as population X. However, this time,
# simulate a difference between the two populations at two distinct 
# components. A component is a set of interconnected edges. Each of the two 
# components is simulated by adding a constant factor, c, to the
# standard normal distribution of each edge comprising the component. This
# gives a contrast-to-noise ratio of c, given that the variance of the noise
# is unity. 
Y =  np.random.random( (20,20,10) )

# Additive factor, also equal to the contrast-to-noise ratio 
c = 2

# Edges comprising component
set1 = np.array([1,3,1,2,2,3,2,5,
                 2,6,3,4,4,5,4,6]) - 1
set1.resize( (8,2) )

set2 = np.array([18,20,18,19,19,20,17,18,16,20,16,17,16,18]) - 1
set2.resize( (7,2) )                 

# Simulate the component.
for i in range(10):
    Y[set1[:,0],set1[:,1]] = Y[set1[:,0],set1[:,1]] + c
    Y[set2[:,0],set2[:,1]] = Y[set2[:,0],set2[:,1]] + c

# Run the NBS with the following parameter options: 
# Set an appropriate threshold. It is difficult to provide a rule of thumb 
# to guide the choice of this threshold. Trial-and-error is always an option
# with the number of permutations generated per trial set low. 
THRESH=3

# Generate 100 permutations. Many more permutations are required in practice
# to yield a reliable estimate. 
K=100

# Set TAIL to left, and thus test the alternative hypothesis that mean of 
# population X < mean of population Y
TAIL='left'

# Run the NBS
PVAL, ADJ, NULL = nbs.compute_nbs(X,Y,THRESH,K,TAIL); 

print "pval", PVAL
print "null", NULL

imshow(ADJ, interpolation='nearest')
title('Edges identified by the NBS')
show()

# Index of true positives
#ind_tp=[ind_set1;ind_set2]; 
ind_tp = np.vstack( (set1, set2) )

# Index of positives idenfitied by the NBS
#ind_obs=find(adj); 
ind_obs = np.array(np.where(np.triu(ADJ))).T

# False positive rate
#fp=length(setdiff(ind_obs,ind_tp))/(20*19/2);
fp_idx = nbs.setdiff2d(ind_obs, ind_tp)
fp = fp_idx.shape[0] / (n * (n-1) / 2.) # only upper triangular matrix is taken into account

# True positive rate
#tp=length(intersect(ind_tp,ind_obs))/length(ind_tp);
tp_idx = nbs.intersect2d(ind_obs, ind_tp)
tp = tp_idx.shape[0] * 1. / len(ind_tp) 

print "True positive rate # %0.3f. False positive rate: # %0.3f" % (tp, fp)
