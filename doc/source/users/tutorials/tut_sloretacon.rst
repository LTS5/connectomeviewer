.. _sloretacon:

===================================
 Using the sLORETA Converter Plugin
===================================

For this tutorial, we assume that you are familiar with using `sLORETA <http://www.uzh.ch/keyinst/loreta.htm>`_
for low resolution brain electromagnetic tomography. The purpose of this converter
is to transform functional EEG networks derived from sLORETA to the Connectome File Format.
Subsequently, analysis and visualization can then be done from within the ConnectomeViewer.

The dataset we use is available in the folder *examples/sLORETA/* in the source distribution.

The example data set was kindly provided by the KEY Institute for Brain-Mind Research. It was derived from 5 minutes of resting EEG of one participant.

* *List19e.sxyz*:  Contains the x,y,z coordiantes of 19 electrodes/ROIs

* *List19e_40mm-ROI.slor*: Contains labeling of voxels corresponding to the choosen ROIs

* *VP01R1-ROIlaggedCoh.txt*: The lagged coherence networks for a single subject computed between the 19 ROIs for 9 frequency bands

* *A-CorrCoeff-IndVar01.txt*: Computed statistical r-values also in the form of networks between 19 ROIs for 9 frequency bands

You start the sLORETA Converter in the ConnectomeViewer over the menu:
*Plugins -> sLORETA -> sLORETA Converter*. You see the following dialog:

.. figure:: ../../_static/sloreta_conv.png

The procedure is as simple as inserting the correct information in the corresponding fields:(was da no chunt verstahn i überhaupt nöd)

* *Update Metadata*: It is important to fill in these metadata. The (Study)Name is used for the Connectome File Name. With good metadata, you can then later better keep track of the contents of the data.

* *Root Folder*: All your data files, e.g. the coherence and statistics files, should reside in this folder. This is also the output folder of the generated Connectome File.

* *# of ROIs*: This is for sanity checking. This number must correspond to the number of rows/columns in the respective data files and its text header.

* *Electrode coordinates file*: This would be the *List19e.sxyz* file from above. This data is used to position the nodes in 3D space.

* *Create empty electrode network?*: Check this if you want to add a network without edges to the connectome file, where nodes are the electrodes/ROIs.

* *ROI slor file*: If you have a .slor file available containing a volumetric segmentation of the brain into ROIs, you can add it here. It then gets converted to a Nifti file. This field is optional.

* *Convert?*: Depending on what data you want to convert: a) Subjects, b) Statistics: CorrCoeff, b) Statistics: t-Statistics. The screenshots below show how you would fill in the appropriate information with the data given.

* *Load State* / *Save State*: These buttons allow you to save the content of the form into a file which you can reload later again. This saves some typing work.

.. figure:: ../../_static/sloreta_conv2.png

.. figure:: ../../_static/sloreta_conv3.png

Take care of the following:

* The files have to follow the correct naming convention, as derived from sLORETA::
 *VP01R1-ROIlaggedCoh.txt*: #SubjectID#Condition#-#Measure#.txt
 
 *A-CorrCoeff-IndVar01.txt*: #Name#-CorrCoeff-IndVar#Number#.txt

* The number of frequency bands needs to equal the number of available matrices in the data files.

We hope that this tutorial works for your use case. If not or there are any errors, do not hesitate to contact `info[at]connectomics.org <mailto:info[at]connectomics.org>`_.