.. _installation:

==========================
Download and Installation
==========================

Step-by-Step Guide for Installation on Ubuntu/Debian
----------------------------------------------------

The Python Version 2.6 is needed minimally.
Add the NeuroDebian repository to your system. The steps are explained here::

	firefox http://neuro.debian.net/

The Connectome Viewer with all dependencies is available from Neuro Debian::

    sudo apt-get update
    sudo apt-get install connectomeviewer

You should now be able to start with::

    connectomeviewer -v

After startup, you can adapt the view widgets according to your needs and screen size. You can now
download `example connectome files <http://connectomeviewer.org/viewer/datasets>`_. After loading a file, double-click on an item (e.g. a network)
in the Connectome File View to load the data in memory. Start to explore the Code Oracle scripts (see Menu bar)
for visualization and analysis task. Created scripts can be saved (Ctrl-S) and run (Ctrl-R) in the embedded
Python Shell. You might want to change preferences in menu Tools->Preference (e.g. use Ipython, don't prompt on exit).

Do not miss the `Mayavi2 documentation page <http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/>`_ that details the many visualization capabilities also available in
Connectome Viewer. Furthermore, check out the `Neuroimaging in Python project <http://nipy.org>`_ for more neuroimaging package and documentation.

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

.. note:: You can install `VirtualBox <http://www.virtualbox.org/wiki/Downloads>`_ and a recent `Neuro Debian virtual machine <http://neuro.debian.net/vm.html>`_ and then carry out the steps above.
