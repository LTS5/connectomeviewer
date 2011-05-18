.. _installation:

==========================
Download and Installation
==========================

.. warning:: This content is soon deprecated and replaced by an easier installation procedure using Neuro Debian.

Step-by-Step Guide for Installation on Ubuntu/Debian
----------------------------------------------------

The Python Version 2.6 is needed minimally.
Add the NeuroDebian repository to your system. The steps are explained here::

	firefox http://neuro.debian.net/

There are some dependencies to install (the exact package names might change slightly for different Ubuntu/Debian versions)::

    sudo apt-get update

    sudo apt-get install python-wxgtk2.8 python-envisagecore python-envisageplugins python-traitsbackendwx python-traitsgui python-enthoughtbase python-wxtools python-wxversion
    sudo apt-get install python-nibabel python-cfflib python-numpy python-scipy python-chaco mayavi2 ipython nipy-suite python-matplotlib python-qscintilla2

Download the `Connectome Viewer source code <http://www.cmtk.org/users/download>`_, extract and install it::

    tar xzf LTS5-....tar.gz
    cd LTS5-connectomeviewer-..../
    sudo python setup.py install

You should now be able to start with::

    connectomeviewer -v

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

Installation on Other Platforms
-------------------------------

.. note:: Currently, the support for a Windows installer has been dropped. But you can install `VirtualBox <http://www.virtualbox.org/wiki/Downloads>`_ and a recent `Ubuntu <http://www.ubuntu.com/desktop/get-ubuntu/download>`_ and then carry out the steps above.
