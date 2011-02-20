""" The ConnectomeViewer wrapper for a cfflib object """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

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
from cviewer.plugins.cff2.csurface_darray import CSurfaceDarray

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

from cbase import CBase

class CSurface(CBase):
    """ The implementation of the Connectome Surface """
    
    obj = Instance(cfflib.CSurface)
    
    darrays = List

    children = Property(depends_on = ['darrays'])
    
    def load(self):
        super(CSurface, self).load()
        # update darrays
        self.darrays = [CSurfaceDarray(ele) for ele in self.obj.data.darrays]
    
    def close(self):
        super(CSurface, self).close()
        self.darrays = []
    
    def _get_children(self):
        return self.darrays
    
    def __init__(self, **traits):
        super(CSurface, self).__init__(**traits)
        
        self.darrays = []
