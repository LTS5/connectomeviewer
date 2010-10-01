""" The implementation of the 'INetwork' interface. """
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
import os

# Enthought library imports
from enthought.traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo
from enthought.traits.ui.api import View, Item, auto_close_message, message

# ConnectomeViewer imports

from cviewer.visualization.render_manager import RenderManager
from cviewer.sources.datasource_manager import DatasourceManager

from cviewer.plugins.ui.preference_manager import preference_manager

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CNetwork(HasTraits):
    """ The implementation of the Connectome Networks """

    # network name as seen in the TreeView
    name = Str
    
    # the render manager of this network
    rendermanager = Instance(RenderManager)
    
    # DatasourceManager Instance of this network
    datasourcemanager = Instance(DatasourceManager)
    

    def __init__(self, name):
        
        pass