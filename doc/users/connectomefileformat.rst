.. _cfformat:

========================
 Connectome File Format
========================

The ConnectomeViewer application defines its own data format, the Connectome File Format,
adhering to `open standards <http://www.opensource.org/osr-intro>`_.

None of the existing formats alone are apt to meet the requirements for a truly
integrated endeavour into structural neuroimaging research on multiple levels.

Goal: On this page, you will learn about the basics of this file format, why it
is useful, and how you generate your own connectome files.

Basic Structure
---------------

The Connectome File Format has file name ending *.cff*. But in fact, it is
a `ZIP file <http://en.wikipedia.org/wiki/ZIP_(file_format)>`_.
You can rename the ending to *.zip* and extract the file therein.

You can find an `example file <http://www.connectome.ch/datasets/homo_sapiens_02.cff>`_.

As an example, a file might have the following structure::

   meta.xml
   Gifti/
      testsubject.gii
      testsubject_labels.gii
   Network/
      network_res83.graphml
      network_res150.graphml
   Nifti/
      ROI_scale33.nii
      ROI_scale60.nii
   Tracks/
      fibers_transformed.trk
  

We will discuss first the components and then show, how the interrelate.


Volume data (Nifti)
```````````````````
|volume|

The de-facto standard in the neuroimaging community dealing with voxel data.
There exists a very good library to use and manipulate the data with Python,
`PyNifti <http://niftilib.sourceforge.net/pynifti/>`_.

Connectomics research on a fine scale usually deals with image stacks. From them,
cells and neurites are segmented. This are essentially the same data processing
steps as on a coarser level. It is therefore straightforward to store image
stacks as a 3D scalar field array.

Raw and segmented volumetric data can be stored in a Connectome File.

.. |volume| image:: ../_static/cff/volume.png

   
Surface data (Gifti)
````````````````````
.. figure:: ../_static/cff/surface.resized.png
   :align: left

After segmentation of structures from volumetric datasets, and after some
post-processing steps, one obtains surfaces meshes of these particular structures.

This surfaces are essentially vertices (points in 3D space) and triangles connecting
these. Since there is a jungle of formats here, the neuroimaging community decided
to define the `Gifti data format <http://www.nitrc.org/projects/gifti/>`_ which is apt
to their needs. ConnectomeViewer supports Gifti files natively to store and render
surfaces, and also to label them.


Networks (GraphML)
``````````````````
.. figure:: ../_static/cff/graph.resized.png

There are a lot of formats to express networks, which are essentially graphs.
Reasons for choosing `GraphML file format <http://graphml.graphdrawing.org/>`_ to describe networks:

* A well-supported data format in various applications
* Straightforward support by the `NetworkX <http://networkx.lanl.gov/>`_ Python network library
* Expressiveness for

  * arbitrary data on nodes and edges
  * hypergraphs
  * hierarchical graphs

Hierarchical and hypergraphs are not yet implemented in the current ConnectomeViewer release,
but it is planned to do so.


Track data (TrackVis)
`````````````````````
.. figure:: ../_static/cff/fibers.resized.png
   :align: left
   
Reconstructed fiber tracks from Diffusion MRI are stored in the file format defined
by the `TrackVis application <http://www.trackvis.org/docs/?subsect=fileformat>`_.

If appropriately linked to the networks and volumetric segmentations within
the Connectome File, and easy interface is provided to visualize tracks in TrackVis
using selected nodes as regions of interest.

Matlab scripts to manipulate TrackVis data is available from the
`Brain Connectivity Challenge <http://pbc.lrdc.pitt.edu/?q=2009b-resource>`_.

Metadata (XML)
``````````````
Using a custom `Connectome Schema Definition <http://connectome.ch/connectome.xsd>`_,
XML is apt to store metadata about the Connectome File and link the different data types together.

Furthermore, it is possible to extend a connectome file by any data you might
want to store for your purposes:

* Which instruments collected data?
* What algorithms have been used to process them?
* Subject data: collection of DNA samples, demographic information and behavioral data


Detailed Example
-----------------

To prepare a Connectome File for correct loading in the ConnectomeViewer, adhering to
the conventions is key. The section :ref:`createcff` gives a primer on how to create
connectome files from your data. Additionally, you can unzip an `example connectome file <http://www.connectome.ch/datasets/homo_sapiens_02.cff>`_
and explore it for yourself.
