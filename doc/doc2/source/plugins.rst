=======
Plugins
=======

Connectome Viewer is built using `Enthought Envisage <http://code.enthought.com/projects/envisage/>`_,
a very powerful application architecture, which allows for seamless integration of plugins and services.

If you would like to develop a plugin for Connectome Viewer, there are `external tutorials <https://svn.enthought.com/enthought/wiki/EnvisageDevGuide>`_
or you can look at the source code in the plugin folder. Feel free to `contact us <mailto:info AT connectomics DOT org>`_
if you have any questions or would like to add a plugin to the list.

Network Based Statistic Plugin
`````````````````````````````````
**Summary**: Computes the network-based statistic (NBS) as described in the reference.
    
**Status**: Integrated in Current Release

**Tutorial**: :ref:`nbs`

**Reference**: `Zalesky A, Fornito A, Bullmore ET (2010) Network-based statistic: Identifying differences in brain networks. NeuroImage. 10.1016/j.neuroimage.2010.06.041 <http://people.eng.unimelb.edu.au/azalesky/paper_nbs.pdf>`_

**Author**: Original code written by Andrew Zalesky, rewritten for Python by Stephan Gerhard.


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
    


