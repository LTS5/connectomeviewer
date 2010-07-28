.. _pydipy:

=============================================
Using the Diffusion in Python (DiPy) Toolbox 
=============================================

.. note:: DiPy currently under heavy development and cannot yet be considered stable. Nevertheless, it already
          offers great functionality.

The Diffusion in Python (DiPy) package aims at achieving these goals::

    The purpose of DiPy is to make it easier to do better diffusion MR imaging research.
    Following up with the NiPy mission statement we aim to build software that is

    * clearly written
    * clearly explained
    * a good fit for the underlying ideas
    * a natural home for collaboration
    
We are very glad to be able to include this great package in ConnectomeViewer, leveraging
the impact it might have on future diffusion and multi-modal studies.
The `DiPy documentation <http://nipy.sourceforge.net/dipy/index.html>`_ and the `development trunk <http://github.com/Garyfallidis/dipy>`_ give further details.

In this tutorial, you will learn the following::

* How is DiPy integrated in ConnectomeViewer?
* Basic loading of a TrackVis File into memory
* Running a tract segmentation on example data
* Visualization of the results using DiPy Fos

How is DiPy integrated in ConnectomeViewer?
-------------------------------------------

The DiPy library was integrated as-it-is in the ConnectomeViewer distribution. Additionally,
a plugin was added to ConnectomeViewer exposing the DiPy functionality in the ConnectomeViewer
Python Shell::

    from cviewer.libs.dipy.dipy.io import trackvis as tv
    from cviewer.libs.dipy.dipy.core import track_performance as pf
    from cviewer.libs.dipy.dipy.core import track_metrics as tm
    from cviewer.libs.dipy.dipy.viz import fos

During the installation procedure, you might have seen some compilation going on.
DiPy uses the Cython extension to make some of the analysis methods running really fast.

Upon start-up the ConnectomeViewer in verbose mode, you should see the message::

    Added Diffusion in Python (DiPy) Plugin


Basic loading of a TrackVis File into memory
--------------------------------------------

The Connectome File Format uses the TrackVis file format to represent its track data.
DiPy lets you load and save `TrackVis files <http://www.trackvis.org/docs/?subsect=fileformat>`_.
Here are the corresponding docstrings::

    def read(fileobj):
        ''' Read trackvis file, return header, streamlines, endianness
    
        Parameters
        ----------
        fileobj : string or file-like object
           If string, a filename; otherwise an open file-like object
           pointing to trackvis file (and ready to read from the beginning
           of the trackvis header data)
    
        Returns
        -------
        streamlines : sequence
           sequence of 3 element sequences with elements:
    
           #. points : ndarray shape (N,3)
              where N is the number of points
           #. scalars : None or ndarray shape (N, M)
              where M is the number of scalars per point
           #. properties : None or ndarray shape (P,)
              where P is the number of properties
              
        hdr : structured array
           structured array with trackvis header fields
    
        Notes
        -----
        The endianness of the input data can be deduced from the endianness
        of the returned `hdr` or `streamlines`
        '''

    def write(fileobj, streamlines,  hdr_mapping=None, endianness=None):
        ''' Write header and `streamlines` to trackvis file `fileobj` 
    
        The parameters from the streamlines override conflicting parameters
        in the `hdr_mapping` information.  In particular, the number of
        streamlines, the number of scalars, and the number of properties are
        written according to `streamlines` rather than `hdr_mapping`.
    
        Parameters
        ----------
        fileobj : filename or file-like
           If filename, open file as 'wb', otherwise `fileobj` should be an
           open file-like object, with a ``write`` method.
        streamlines : sequence
           sequence of 3 element sequences with elements:
    
           #. points : ndarray shape (N,3)
              where N is the number of points
           #. scalars : None or ndarray shape (N, M)
              where M is the number of scalars per point
           #. properties : None or ndarray shape (P,)
              where P is the number of properties
    
        hdr_mapping : None, ndarray or mapping, optional
           Information for filling header fields.  Can be something
           dict-like (implementing ``items``) or a structured numpy array
        endianness : {None, '<', '>'}, optional
           Endianness of file to be written.  '<' is little-endian, '>' is
           big-endian.  None (the default) is to use the endianness of the
           `streamlines` data.
    
        Returns
        -------
        None
        
        '''

You will need the Dataset 2 from `here <http://connectomeviewer.org/viewer/datasets>`_ that includes
single subject example data with a track file. Load this file in the recent ConnectomeViewer version.
You should be able to see the TrackVis file in the Connectome File View:

.. figure:: ../../_static/trackvis_in_view.png

Now, load the actual track data for the first network into memory by going to the Python shell and executing these steps::

    cfile.networks[0].tracks[0].load_trackfile()

Your trackvis file is now loaded. You can access its header::

    cfile.networks[0].tracks[0].hdr
    
You can then display the number of actual streams available::

    len(cfile.networks[0].tracks[0].streams)

To see the points that make up the first track (ndarray), just type::

    cfile.networks[0].tracks[0].streams[0][0]


Running a tract segmentation on example data
--------------------------------------------

These steps are taken from an example script *tractography_clustering.py* shipped with DiPy.
You can enter them directly in the Python Shell to follow what is happening::

    import time
    import numpy as np
    
    print 'Copying tracks...'
    T=[i[0] for i in cfile.networks[0].tracks[0].streams]
    
    print 'Representing tracks using only 3 pts...'
    tracks=[tm.downsample(t,3) for t in T]
        
    print 'Hidden Structure Clustering...'
    now=time.clock(); C=pf.local_skeleton_clustering(tracks,d_thr=20); print 'Done in', time.clock()-now,'s.'
    
    print 'Reducing the number of points...'
    T=[pf.approximate_ei_trajectory(t) for t in T]


That's it. We have now a clustering of our tracks. Notice how fast it executes!
(Executing the code in this way blocks the ConnectomeViewer application. Later, the support to execute
these computations in a separate thread will be included.)


Visualization of the results using DiPy Fos
--------------------------------------------

ConnectomeViewer interfaces with TrackVis for track visualization. You can automatically
generate ROIs in TrackVis of your selected nodes to filter the tracks.

DiPy comes with its own rendering engine wrapping VTK, namely **Phos** (which means light in greek).
This will give you powerful capabilities to programatically generate the visualizations you
need of your data. We will here use Phos solely for the purpose of displaying the results of our segmentation::

    print 'Showing dataset after clustering.'
    r=fos.ren()
    colors=np.zeros((len(T),3))
    for c in C:
        color=np.random.rand(1,3)
        for i in C[c]['indices']:
            colors[i]=color
    fos.add(r,fos.line(T,colors,opacity=1))
    fos.show(r)

As a result, you should see the beautifully colored brain fibers:

.. figure:: ../../_static/dipy_result.png
