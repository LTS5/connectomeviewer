""" Module for starting ConnectomeDatabase download interface

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# Standard library imports.
import os, os.path
import sys

# Enthought library imports.
from enthought.pyface.api import OK
from enthought.pyface.wizard.api import ChainedWizard, Wizard, WizardPage
from enthought.traits.api import Color, HasTraits, Int, Str, Password, Directory, Tuple
from enthought.traits.ui.api import View, Item

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

def start_interface(window):
    """ Starts the interface to download cfiles """
    
    # ConnectomeViewer imports
    from cviewer.plugins.cdb.cfile_editor import CFileContainer
    from cviewer.plugins.cdb.connection_details import ConnectionDetails
    from cviewer.plugins.ui.preference_manager import preference_manager
    
    heading="ConnectomeDatabase",
    subheading="Select the files you want to download.",

    mycdb = preference_manager.cdb
    details = ConnectionDetails()
    details.set(host = mycdb.host, user=mycdb.user, passwd=mycdb.passwd,\
                            db = mycdb.db, savepath = mycdb.savepath)

    if not details._allfull():
        # message box: not all field filled
        window.error('Please fill out all the database preferences!')
        return

    cfilecont = CFileContainer()
    retval = cfilecont._establish_connection(myhost = details.host,\
                               myuser = details.user,\
                               mypasswd = details.passwd,\
                               mydb = details.db)
    
    if not retval:
        # message box: could not establish database connection
        window.error('Could not establish a connection to the database.\nPlease update your settings. ')
        return
    
    # setting the cfiles
    cfilecont._set_cfiles_list()
    cfilecont.set(savepath = details.savepath)
    
    ui = cfilecont.edit_traits()
