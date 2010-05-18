''' Release data for ConnectomeViewer

This script should do no imports.  It only defines variables.
'''
version = '0.1.6'
is_release = False

long_description = \
"""
ConnectomeViewer - A Framework for the Visualization and Analysis of Multi-Modal Multi-Scale
Connectome Data in Python

The aim of ConnectomeViewer is to produce a platform-independent Python framework for
the analysis and visualization of Connectome data using an open development model.

The ConnectomeViewer is a extensible, scriptable, pythonic software tool for visualization
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
      an easily installable bundle installable bundle.

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
traits_min_version = '3.2.1'
networkx_min_version = '1.1'
mayavi_min_version = '3.3.1'
h5py_min_version = '1.3.0'
scipy_min_version = '0.5'
cython_min_version = '0.12.1'

# dipy_min_version = ''
# nibabel_min_version = ''
# nipy_min_version = ''
#
#"networkx >= 1.1",
#"h5py >= 1.3.0",
#"EnvisageCore >= 3.1.2",
#"TraitsBackendWX >= 3.2.1",
#"Mayavi >= 3.3.1",
#"EnvisagePlugins >= 3.1.2",
#"EnthoughtBase >= 3.0.4",
#"TraitsGUI >= 3.1.1",
#"Traits >= 3.2.1",
#"Chaco >= 3.2.1"