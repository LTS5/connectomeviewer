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

* Download the `installation script <http://github.com/unidesigner/connectomeviewer/raw/development/scripts/install_cviewer.sh>`_ (Ubuntu 9.10 or higher / Fedora 11 or higher supported)

* You need to make the installation file executable::

    chmod +x install_cviewer.sh
    
* Start the installation and compilation ::

    sh ./install_cviewer.sh

* Start the ConnectomeViewer in verbose mode::

    connectomeviewer -v

* If there are errors during the script execution, generate a log file and send it together with the startup logfile via email to `info[at]connectomics.org <mailto:info[at]connectomics.org>`_::

    sh ./install_cviewer.sh > logfile.txt

.. On first startup, a directory is created in your home folder (*$HOME/.enthought/ch.connectome.viewer*) to store the logfile and window settings. If the first startup was as root, you do not have write permission in this folder, leading to a *Permission Error*. Simply remove this folder (*sudo rm -rf $HOME/.enthought/ch.connectome.viewer/*) and start ConnectomeViewer again as user.

Step-by-Step Guide for Installation on Windows
----------------------------------------------

.. note:: ConnectomeViewer should work for all Windows Versions (except Windows 7). It was tested with Windows XP.

* Install the recent EPD_ (License: Academic, Option: Install for all users). It is free for academic purposes, see button at the bottom of the page.
* To access HDF5 files, we need H5Py, you `install it from here <http://code.google.com/p/h5py/downloads/list>`_
* Install ConnectomeViewer executable `(from the download page) <http://www.connectomeviewer.org/users/download>`_
* Either select the ConnectomeViewer from the Startmenu (Connectome->ConnectomeViewer) or open a Command Prompt and type::

    connectomeviewer -v

* You can `download example datasets <http://connectomeviewer.org/viewer/datasets>`_. Make sure that they end with *.cff* (however they are regular ZIP archives).

If you encounter any problems, please send an email to `info AT connectomics.org <mailto:infoATconnectomics.org>`_!

.. include:: ../links_names.txt

Step-by-Step Guide for Installation on Other Platforms
------------------------------------------------------

If you managed to make the ConnectomeViewer work on your platform,
`please inform us <mailto:infoATconnectomics.org>`_ and we make the instructions available here.
