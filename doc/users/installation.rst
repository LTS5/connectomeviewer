.. _installation:

==========================
Download and Installation
==========================

.. note:: Please use the `Download Page <http://www.connectomeviewer.org>`_ to get access to the download area.
          The ConnectomeViewer is currently BETA and released to the public. 32bit and 64bit architectures are supported.


Step-by-Step Guide for Installation on Linux (Ubuntu)
-----------------------------------------------------

.. note:: This guide explains the necessary steps to install on a fresh Ubuntu 9.10 version with all updates.

We use the default Python version 2.6. This guide is partly derived from `ETS install instructions <https://svn.enthought.com/enthought/wiki/Build>`_.

* Start a Terminal: Applications -> Accessories -> Terminal

* Following packages are required::

    sudo apt-get remove mayavi2 python-enthoughtbase python-enthought-chaco2 python-enthought-traits python-enthought-traits-ui python-apptools python-chaco python-envisageplugins
    sudo apt-get install subversion python-setuptools libvtk5.2 python-vtk python-numpy python-wxversion python2.6-dev python-sphinx g++ swig python-configobj glutg3 glutg3-dev libxtst-dev ipython python-lxml python-matplotlib python-qscintilla2 cython gcc

* We need the Enthought Tool Suite first. Install first the helper::

    sudo easy_install ETSProjectTools
    
* Now checkout the sources to a permanent location. We are going to install ETS in a way that allows to update it easily later on::
    
    ets co ETS
    
* Now install the sources to the Python distribution as root::
    
    cd ETS_3.3.1/
    sudo ets develop

* Install NetworkX for network analysis::

    sudo easy_install networkx
    
* Install PyMySQL for MySQL connectivity (optional)::

    sudo easy_install PyMySQL

* From the `Download Page <http://www.connectomeviewer.org/users/download>`_, download the recent sourcefile (ending *.tar.gz*)

* Change to the folder where you downloaded the source, and extract it (update the version)::

    tar xzf ConnectomeViewer-0.X.X.tar.gz

* Finally, you can install ConnectomeViewer with::

    cd ConnectomeViewer-0.X.X
    sudo python setup.py install

* Start the ConnectomeViewer with::

    connectomeviewer -v
  
Now the ConnectomeViewer application should start up. The first start-up is usually slow.
If you encounter any problems, please send an email to `info[at]connectomics.org <mailto:info[at]connectomics.org>`_!


Step-by-Step Guide for Installation on Windows
----------------------------------------------

.. note:: ConnectomeViewer should work for all Windows Versions (except Windows 7). It was tested with Windows XP.

* Install the recent EPD_ (License: Academic, Option: Just for me). It is free for academic purposes, see button at the bottom of the page.
* Then you need to open a Command prompt (*Start->Run: cmd*)
* Install PyMySQL for MySQL connectivity::

    easy_install PyMySQL

* Install ConnectomeViewer executable `(from the download page) <http://www.connectomeviewer.org/users/download>`_
* Either select the ConnectomeViewer from the Startmenu (Connectome->ConnectomeViewer) or open a Command Prompt and type::

    connectomeviewer -v

If you encounter any problems, please send an email to `info AT connectomics.org <mailto:infoATconnectomics.org>`_!

.. include:: ../links_names.txt

Step-by-Step Guide for Installation on Other Platforms
------------------------------------------------------

If you managed to make the ConnectomeViewer work on your platform,
`please inform us <mailto:infoATconnectomics.org>`_ and we make the instructions available here.
