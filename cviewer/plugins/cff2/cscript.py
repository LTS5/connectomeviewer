""" The ConnectomeViewer wrapper for a cfflib object """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os

# Enthought library imports
from enthought.traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo, Property, Code
from enthought.traits.ui.api import View, Item, auto_close_message, message

import cfflib

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

from cbase import CBase

class CScript(CBase):
    """ The implementation of the Connectome CScript """
    
    obj = Instance(cfflib.CScript)

    view = View(
            Item( 'code',  style = 'readonly' ),
            resizable = True,
            width     = 0.75,
            height    = 0.75
        )
    
    def __init__(self, **traits):
        super(CScript, self).__init__(**traits)
                
    def open_file(self):
        """ Opens the file in an editor """
        if not self.loaded:
            self.load()

        # call a plugin  action to perform the open action
        from cviewer.plugins.cff2.actions.actions import OpenFile
        
        