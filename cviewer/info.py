''' Release data for Connectome Viewer

This script should do no imports.  It only defines variables.
'''
version = '2.0.0'
is_release = False

long_description = \
"""
Connectome Viewer - A Framework for the Visualization and Analysis of Multi-Modal Multi-Scale
Connectome Data in Python

The aim of Connectome Viewer is to produce a platform-independent Python framework for
the analysis and visualization of Connectome data using an open development model.

The Connectome Viewer is a extensible, scriptable, pythonic software tool for visualization
and analysis in neuroimaging research on many spatial scales. Employing the Connectome File
Format, diverse data such as networks, surfaces, volumes, tracks and metadata are handled
and integrated.

Specifically, we aim to:

   1. Provide an open source, mixed language scientific programming
      framework for rapid development and quantitative analysis

   2. Provide a visualization platform to readily visualize multi-modal data for
      interactive data exploration

   3. Allow for enhanced brain connectivity analysis and plotting
   
   4. Provide the Connectome File Format to store a wide range of data types:
      metadata, networks, surfaces, volumes, fiber tracks, time series

   5. Create and maintain a wide base of developers to contribute plugins to
      this framework.

   6. To integrate this framework with software packages in neuroimaging and provide
      an easily installable bundle.

"""

# these have to be done per install_requires
envisagecore_min_version = '3.1.2'
traitsbackendwx_min_version = '3.2.1'
envisageplugins_min_version = '3.1.2'
enthoughtbase_min_version = '3.0.4'
traitsgui_min_version = '3.1.1'
chaco_min_version = '3.2.1'
lxml_min_version = '2.2.6'
pymysql_min_version = '0.2'

# these can be done safely
traits_min_version = '3.2.0'
networkx_min_version = '1.4'
mayavi_min_version = '3.3.2'
h5py_min_version = '1.2.0'
scipy_min_version = '0.5'


# for ubuntu 10.04
###
envisagecore_min_version = '3.1.1' # python-envisagecore
envisageplugins_min_version = '3.1.1' # python-envisageplugins

traitsbackendwx_min_version = '3.2.0' # python-traitsbackendwx
traitsbackendqt_min_version = '3.2.0' # python-traitsbackendqt
traitsgui_min_version = '3.1.0' # python-traitsgui
traits_min_version = '3.2.0' # python-traits

python_enthought_traits_ui = '3.1.0' # python-enthought-traits-ui
enthoughtbase_min_version = '3.0.3' # python-enthoughtbase
chaco_min_version = '3.2.0' # python-chaco
lxml_min_version = '2.2.4' # python-lxml
scipy_min_version = '0.5' # python-scipy (0.7.0)
numpy_min_version = '1.3.0' # python-numpy
h5py_min_version = '1.2.1' # python-h5py
mayavi_min_version = '3.3.0' # mayavi2

# python-apptools (3.3.0), python-enthought-traits (3.1.0), python-numpy (1.3.0)

# outdated or not available
pymysql_min_version = '0.2' # not available
networkx_min_version = '1.4' # python-network (0.99)
