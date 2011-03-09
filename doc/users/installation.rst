.. _installation:

==========================
Download and Installation
==========================

.. note:: Download the `source code <http://www.connectomeviewer.org/users/download>`_ used for the installation. Please register following `this link. <http://www.connectomeviewer.org/users/register>`_
          The ConnectomeViewer is currently BETA and released to the public. 32bit and 64bit architectures are supported.


Step-by-Step Guide for Installation on Linux (Ubuntu, Fedora)
-------------------------------------------------------------

The Python Version 2.6 is needed. The installation procedure including installation of required packages,
downloading of sources, and compilation is automated by a sh-script.
Take first a look at the respective scripts in order to understand what they do.

* Download the `installation script <https://github.com/LTS5/connectomeviewer/tree/master/scripts>`_ (Ubuntu / Fedora 11 or higher supported)

* You need to make the installation file executable::

    chmod +x install_cviewer_ubuntu.sh
    
* Start the installation and compilation (choose the appropriate script) ::

    sh ./install_cviewer_ubuntu10_10.sh

* Start the ConnectomeViewer in verbose mode::

    connectomeviewer.py -v

* If there are errors during the script execution, generate a log file and send it together with the startup logfile via email to `info[at]connectomics.org <mailto:info[at]connectomics.org>`_::

    sh ./install_cviewer_ubuntu.sh > logfile.txt

.. On first startup, a directory is created in your home folder (*$HOME/.enthought/ch.connectome.viewer*) to store the logfile and window settings. If the first startup was as root, you do not have write permission in this folder, leading to a *Permission Error*. Simply remove this folder (*sudo rm -rf $HOME/.enthought/ch.connectome.viewer/*) and start ConnectomeViewer again as user.

.. Step-by-Step Guide for Installation on Windows
.. ----------------------------------------------
.. * Install the recent EPD_ (License: Academic, Option: Install for all users). It is free for academic purposes, see button at the bottom of the page.
.. * To access HDF5 files, we need H5Py, you `install it from here <http://code.google.com/p/h5py/downloads/list>`_
.. * Install ConnectomeViewer executable `(from the download page) <http://www.connectomeviewer.org/users/download>`_
.. * Either select the ConnectomeViewer from the Startmenu (Connectome->ConnectomeViewer) or open a Command Prompt and type::
..     connectomeviewer.py -v
.. * You can `download example datasets <http://connectomeviewer.org/viewer/datasets>`_. Make sure that they end with *.cff* (however they are regular ZIP archives).
.. If you encounter any problems, please send an email to `info AT connectomics.org <mailto:infoATconnectomics.org>`_!

.. include:: ../links_names.txt

Step-by-Step Guide for Installation on Other Platforms
------------------------------------------------------

.. note:: Currently, the support for a Windows installer has been dropped. But you can install `VirtualBox <http://www.virtualbox.org/wiki/Downloads>`_ and a recent `Ubuntu <http://www.ubuntu.com/desktop/get-ubuntu/download>`_ and then carry out the steps above.

If you managed to make the ConnectomeViewer work on your platform,
`please inform us <mailto:infoATconnectomics.org>`_ and we make the instructions available here.

Step-by-Step Guide for Installation on Linux by Hand
----------------------------------------------------

Dependencies

+------------+------------+
| Package   | Minimal Version |
+============+============+
| NetworkX | |
+------------+------------+
| Cython | |
+------------+------------+
| ETS | https://svn.enthought.com/enthought/wiki/Build/ETS_3.0.0b1/Py2.5/Generic_Any_Any
+------------+------------+
| Scipy | |
+------------+------------+
| Numpy | |
+------------+------------+
