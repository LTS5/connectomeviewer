""" Helper class to load CFF files """

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

# Standard library imports
import os.path

# Enthought library imports
from enthought.io.api import File
from enthought.traits.api import Str, Bool
from enthought.pyface.action.api import Action
from enthought.pyface.api import FileDialog, OK
from enthought.pyface.image_resource import ImageResource

# ConnectomeViewer imports
from common import IMAGE_PATH
from cviewer.plugins.ui.preference_manager import preference_manager

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

######################################################################
# `OpenCFFFile` class.
######################################################################
class OpenCFile(Action):
    """ An action that opens a data file depending on the supported
    extensions.  """

    name        = "Open CFF File"
    description = "Open the File Dialog where you can select a .cff file"
    tooltip     = "Open a CFF file"
    path        = Str("MenuBar/File/LoadDataMenu")
    image       = ImageResource("cff-open.png", search_path=[IMAGE_PATH])

    # Is the action enabled?
    enabled = Bool(True)

    # Is the action visible?
    visible = Bool(True)

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event, cfile=None):
        """ Performs the action. """
        
        logger.info('Performing open connectome file action')
        
        # helper variable to use this function not only in the menubar
        exec_as_funct = True
        
        if cfile is None:
            # get the instance of the current CFile
            # with the help of the Service Registry
            cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
            exec_as_funct = False
    
        wildcard = "Connectome File Format v2.0 (*.cff)|*.cff|" \
                    "All files (*.*)|*.*"
        dlg = FileDialog(wildcard=wildcard,title="Choose a Connectome File",\
                         resizeable=False, \
                         default_directory=preference_manager.cviewerui.cffpath,)
        
        if dlg.open() == OK:
            
            if not os.path.isfile(dlg.path):
                logger.error("File '%s' does not exist!"%dlg.path)
                return
            
            # if file exists and has .cff ending
            if os.path.exists(dlg.paths[0]) and (dlg.paths[0]).endswith('.cff'):
                
                # close the cfile if one is currently loaded
                if cfile._data_loaded:
                    cfile.close_cfile(close_scenes=True)
                
                if not exec_as_funct:
                    # setup statusbar
                    self.window.status_bar_manager.message='Loading Connectome File ...'
                
                # load cfile data
                cfile.load_cfile(dlg.paths[0])
                
            else:
                logger.info('Could not load file: '+ dlg.paths)
