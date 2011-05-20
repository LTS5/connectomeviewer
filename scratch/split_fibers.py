# check if fibers touch other grey matter labeled regions

import numpy as np
import cfflib as cf

# load data
a=cf.load('DATAALE/control01_tp1_run3.cff_FILES/meta.cml')

print "Load fiberlabels...."
# fiberlabels
fiblabels = a.get_by_name('Fiber labels (freesurferaparc)')
fiblabels.load()

print "Load fiberarray..."
# fiberarray
fibarr = a.get_by_name('Filtered Tractography')
fibarr.load()

print "Load ROI parcellation..."
# segmentation volume in diffusion space
segvol = a.get_by_name('ROI Scale freesurferaparc (b0 space)')
segvol.load()

print "Load GFA..."
# segmentation volume in diffusion space
gfavol = a.get_by_name('GFA Scalar Map')
gfavol.load()
# XXX: need to use voxel dimension

# short cut references
sd=segvol.data.get_data()
voxdim=fibarr.data[1]['voxel_size']
fib=fibarr.get_fibers_as_numpy()

def print_fiblabels(fib, sd, fromid = 0, toid = 10):
    # loop over fibers
    ret = []
    for i in xrange(fromid,toid+1):
        #print "Path for fiber ", i
        # if voxdim is 1,1,1, we do not have to divide
        # convert mm to vox
        idx = fib[i].astype('int32')
        # retrieve labels along the fiber
        fiblabels = sd[idx[:,0],idx[:,1],idx[:,2]]
#        print "Start: ", fiblabels[0]
#        print "End: ", fiblabels[-1]
#        print "Max inbetween: ###############", fiblabels[1:-1].max()
#        print "----"
        # number of nonzero elements
        #print "Max inbetween: ###############", fiblabels[1:-1]
        ret.append(len(np.where( fiblabels[1:-1] != 0.0 )[0]))
    return ret


def most_select_rois(fib, sd, fromid = 0, toid = 10):
    ret = []
    cnt = 0
    for i in xrange(fromid,toid+1):
#        print "Path for fiber ", i
        # if voxdim is 1,1,1, we do not have to divide
        # convert mm to vox
        idx = fib[i].astype('int32')
        # retrieve labels along the fiber
        fiblabels = sd[idx[:,0],idx[:,1],idx[:,2]]
#        print "Start: ", fiblabels[0]
#        print "End: ", fiblabels[-1]
#        print "Max inbetween: ###############", fiblabels[1:-1]
#        print "----"
        # number of nonzero elements
        #print "Max inbetween: ###############", fiblabels[1:-1]
        fibinbetween = fiblabels[1:-1]
        validx = np.where( fibinbetween != 0.0 )[0]
        #print validx
        if len(validx) == 0:
            cnt += 1
        else:
            #print fibinbetween[validx]
            ret = ret + fibinbetween[validx].tolist()
    print "We counted clean fibers", cnt
    return ret
    
e=most_select_rois(fib,sd, fromid = 0, toid = fib.shape[0]-1)

#inf=print_fiblabels(fib, sd, fromid = 0, toid = fib.shape[0]-1)
#plot(inf)



