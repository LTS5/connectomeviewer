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
      List, Instance, DelegatesTo, Property
from enthought.traits.ui.api import View, Item, auto_close_message, message

# ConnectomeViewer imports

from cviewer.visualization.render_manager import RenderManager
from cviewer.sources.datasource_manager import DatasourceManager
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.plugins.cff.ui.edge_parameters_view import EdgeParameters

import cfflib

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

from cbase import CBase

class CNetwork(CBase):
    """ The implementation of the Connectome Networks """
    
    # the render manager of this network
    rendermanager = Instance(RenderManager)
    
    # DatasourceManager Instance of this network
    datasourcemanager = Instance(DatasourceManager)
    
    # the cfflib CNetwork object
    obj = Instance(cfflib.CNetwork)
    
    graph = Property(Any, depends_on = [ 'obj' ])

    # edge parameters for visualization
    _edge_para = Instance(EdgeParameters)
    
    def _get_graph(self):
        return self.obj.data

#    def invoke_matrix_viewer(self):
#        """ Invoke the Conectome Matrix Viewer """
#        from cviewer.visualization.matrix.cmatrix_viewer import CMatrixViewer
#        self.cmatrix_viewer = CMatrixViewer(self.network)
#        self.cmatrix_viewer.edit_traits()

    def __init__(self, **traits):
        super(CNetwork, self).__init__(**traits)
        
        
        
    