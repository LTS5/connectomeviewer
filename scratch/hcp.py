import h5py
import numpy as np

# How to use HDF5 for connectome data
 
def create_hdf():

    f=h5py.File('connectomes.hdf5','w')
    
    # first create a group for the subject
    subj = f.create_group('mysubject')
    
    # the hierarchy for a subject might look like this
    # /subject
    #     /fsaverage
    #        /vertices_white
    #        /vertices_inflated
    #        /triangles
    #        /labeling1
    
    #     /denseconnectome
    #        /labeling (e.g. selecting 20000 ROIs from fsaverage by labeling the vertices)
    #        COMMENT: it might be more natural to define a labeling on the faces (i.e. triangles)
    #                 e.g. for fiber-triangle intersections, thus one would have to index on the triangles array 
    #        /connectome (the sparse 20000x20000x3 matrix, 3rd dimension
    #                      can be different measures)
    #        /fsfmri (the dense 20000x20000 matrix)
        
    #     /parcelconnectome
    #        /labeling (e.g. selecting 500ROIs from fsaverage
    #        /connectome
    #        /rsfmri
    
    #     /tractography
    #        /fibers
    #        /offset (to keep track of individual fibers)
    #        /clustering1 (each fiber has a cluster label for e.g. 500 clusters)
    #        /skeleton1 (each clustering can be represented as a skeletonized version
    #                    of the tractography. see diffusion in python package dipy)
    #        /skeleton1_offset
    
    # Advantages
    # - common & fast interface to all datasets
    # - data is memory-mapped, i.e. can stay on the disk util really necessary
    #   to load it into memory, e.g. also for data slices (subsets) -> very fast
    # - metadata can be used to annotate each node in the hierarchy
    
    # the commands to create such a layout of an hdf5 file and populate it
    # with some random numbers
    
    #     /fsaverage
    fsaverage = subj.create_group('fsaverage')
    
    #        /vertices_white
    da = np.random.random( (10000,3) )
    vert_w = fsaverage.create_dataset('vertices_white', data = da)
    
    #        /vertices_inflated
    da = np.random.random( (10000,3) )
    vert_i = fsaverage.create_dataset('vertices_inflated', data = da)
    
    #        /triangles
    da = np.random.random_integers(0, 10000, (5000,3)).astype(np.uint)
    triangles = fsaverage.create_dataset('triangles', data = da)

    #        /labeling1 (#2000 ROIs)
    da = np.random.random_integers(0, 2000, (10000,3)).astype(np.uint)
    labeling1 = fsaverage.create_dataset('labeling1', data = da)

    
    #     /denseconnectome
    decon = subj.create_group('denseconnectome')
    # add an attribute
    decon.attrs['desc'] = 'This is the description of this dense connectome.'
    
    #        /labeling (e.g. selecting 2000 ROIs from fsaverage)
    da = np.random.random_integers(0, 2000, (10000,3)).astype(np.uint)
    labeling = decon.create_dataset('labeling', data = da)
    
    #        /connectome (the sparse 20000x20000x3 matrix)
    da = np.random.random( (2000,2000,3) )
    connectome = decon.create_dataset('connectome', data = da)
    connectome.attrs['z0'] = 'length'
    connectome.attrs['z1'] = 'adcmean'
    connectome.attrs['z2'] = 'number'
    
    #        /fsfmri (the dense 20000x20000 matrix)
    da = np.random.random( (2000,2000) )
    fsfmri = decon.create_dataset('rsfmri', data = da)
    
    # for the /parcelconnectome, it is analogous
    
    #     /tractography
    stream=subj.create_group('tractography')
    
    #        /fibers
    st=np.random.random( (15,3))
    stream.create_dataset('fibers', data=st)

    #        /offset (to keep track of individual fibers)
    # we store here 3 fibers: 0 to 3, 4 to 9, and 10 to 14
    of=np.array( [0, 4, 10, 15], dtype=np.uint)
    stream.create_dataset('offset',data=of)

    #        /clustering1 (each fiber has a cluster label for e.g. 500 clusters)
    # define a clustering into 3 cluster 0,1,2 for 3 fibers only
    la=np.random.random_integers(0,2, (len(of)-1,))
    stream.create_dataset('clustering1',data=la)
    
    # the skeleton dataset is skipped here, but it is basically
    # a downsampled version a set of fibers (=cluster)
    # for fast representation and visualization (interactivity)
    
    f.close()


def open_hdf():
    
    f=h5py.File('connectomes.hdf5', 'r')
    
    print f.items()

    # resting state fmri connectivity matrix
    print f['/mysubject/denseconnectome/rsfmri/'].value
    
    # print second fiber
    i = 1
    off0, off1 = f['/mysubject/tractography/offset'].value[i:i+2]
    print f['/mysubject/tractography/fibers'].value[off0:off1]
    
    f.close()

create_hdf()  
open_hdf()
