.. _external-index:

=======================================
Overview of included external libraries
=======================================

.. warning:: This content is deprecated.

.. note: Remember to update the .gitignore in the root folder not to include the git repositories.
Maybe one could use also git `submodules <http://www.kernel.org/pub/software/scm/git/docs/user-manual.html#submodules>`_ ?

''Criteria to decide whether a package is included here or as a dependency:''

* Does the library add benefits for the neuroimaging researcher?
* Is the package included in an up-to-date Ubuntu package?
* What are the dependencies of the library?
* Has the author of the library agreed to include it?
* Has the library an open license (e.g. BSD, GPL, LGPL, MIT)?
* Is it easy to include the library in terms of installation, e.g. by numpy.distutils recursive
scheme? 
* Add it as standalone repository, symbolic link to a repository, or frozen released version?
(How to automate the update procedure?) 
* If the library is not available as Ubuntu package or via PyPi, this indicates that the packaging and distribution
is not mature enough (?)


joblib
------
Description : Joblib is a set of tools to provide lightweight pipelining in Python.
URL : `http://packages.python.org/joblib/`_
Dev-URL : `http://github.com/joblib/joblib`_
Copyright : Copyright (c) 2009-2010, Gael Varoquaux
License : BSD-licensed
Dependency : NumPy
Current version : 0.4.1

PyEEG
-----
Description : PyEEG, a Python module to extract EEG features, v 0.01b
Copyleft : 2009 Forrest Sheng Bao
URL : `http://code.google.com/p/pyeeg/`_
License : GPL v3
Current version : 0.01_r4

tomoviz
-------
Description : Mayavi 4D Visualization for large datasets
URL : http://github.com/emmanuelle/tomoviz
License : new BSD

PyXNAT
------

URL : `http://packages.python.org/pyxnat/`

OpenMEEG
--------
URL : `http://www-sop.inria.fr/odyssee/software/OpenMEEG/`_


Parallel Python
---------------
URL : `http://www.parallelpython.com`_

PyConTo
-------
Python Connectome Toolbox

* Includes Brain Connectivity Toolbox Python Wrappers
* Network based statistics for case-control, task-control studies
* Wrapper for CSSR, Spectral Graph Toolbox
* Hierarchical Community Detection

pbrain
------
???, is included in nipy

URL : `http://github.com/nipy/pbrain`_

pytables
--------

URL : `http://www.pytables.org/moin`_
Dependency : Numexpr

nipy
----

nitime
------

nibabel
-------

dipy
----
Diffusion Imaging in Python

Fos
---
Scientific 3D Visualization

nipype
------

xipy
----

cfflib
------

python-statlib
--------------
`http://code.google.com/p/python-statlib/wiki/PstatDoc`_

NamedAxis
---------
By Fernando Perez `<http://github.com/fperez/datarray>`_
