=======
Plugins
=======

ConnectomeViewer is built using `Enthought Envisage <http://code.enthought.com/projects/envisage/>`_,
a very powerful application architecture, which allows for seamless integration of plugins and services.

If you would like to develop a plugin for ConnectomeViewer, there are `external tutorials <https://svn.enthought.com/enthought/wiki/EnvisageDevGuide>`_
or you can look at the source code in the plugin folder. Feel free to `contact us <mailto:info AT connectomics DOT org>`_
if you have any questions or would like to add a plugin to the list. Additionally, you should
visit `ConnectomeViewer Wiki <http://wiki.github.com/unidesigner/connectomeviewer/>`_ to see ongoing developments.


sLORETA Plugin
`````````````````````````````````
**Summary**: sLORETA is a low resolution brain electromagnetic tomography software that
can be applied to EEG/MEG time series. This plugins allows one to convert functional
connectivity output from sLORETA to connectome files. It has also a converter from voxel-based
data (*.slor files*) to Nifti.
    
**Status**: Integrated in Current Release / In Development / Externally Available

**Tutorial**: :ref:`sloretacon`

**Author**: Stephan Gerhard in collaboration with `Roberto Pascual-Marqui <http://www.researcherid.com/rid/A-2012-2008>`_
    
**URL**: `http://www.uzh.ch/keyinst/loreta.htm <http://www.uzh.ch/keyinst/loreta.htm>`_


Diffusion in Python Plugin
`````````````````````````````````
**Summary**: The Diffusion Imaging Analysis in Python is part of the NiPy effort
and under development. It includes readers/writers of diffusion and track data,
and algorithms for automatic tract segmentation. It uses Cython for speed.
    
**Status**: Integrated in Current Release

**Tutorial**: :ref:`pydipy`

**Author**: All the credit goes to the NiPy/DiPy team. Stephan Gerhard wrote the plugin code and the integration.
    
**URL**: `http://nipy.sourceforge.net/dipy/ <http://nipy.sourceforge.net/dipy/>`_


Bindings Plugin
`````````````````````````````````
**Summary**: This exposes the currently loaded connectome file to the Python Shell by
adding the reference *cfile* to its name space.
    
**Status**: Integrated in Current Release

**Author**: Stephan Gerhard


TextEditor Plugin
`````````````````````````````````
**Summary**: This includes a straightforward text editor with Python syntax-highlighting
to easily write scripts. You can run the scripts by pressing Ctrl-R. See File menu to open and save files.

**Status**: Integrated in Current Release

**Author**: Derived from the Enthought Envisage Text Editor Plugin.
    


