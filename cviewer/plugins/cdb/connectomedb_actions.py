""" Actions for the ConnectomeDatabase

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

# Enthought library imports
from enthought.pyface.image_resource import ImageResource
from enthought.traits.api import Bool
from enthought.pyface.action.api import Action

# ConnectomeViewer imports
from cviewer.action.common import IMAGE_PATH

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class StartDownloadInterface(Action):
    """ Starts the ConnectomeDatabase Download Interface """

    name        = "ConnectomeDB"
    tooltip     = "ConnectomeDB Download Interface"
    description = "Open the dialog of the ConnectomeDatabase for downloading connectome files"
    image       = ImageResource("cdb.png", search_path=[IMAGE_PATH])

    # Is the action enabled?
    enabled = Bool(True)

    # Is the action visible?
    visible = Bool(True)

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event):
        """ Performs the action. """
        
        from connectomedb_interface import start_interface
        start_interface(window=self.window)
