""" The ConnectomeViewer wrapper for a cfflib object """
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
    
    code = Code

    view = View(
            Item( 'code',  style = 'readonly' ),
            resizable = True,
            width     = 0.75,
            height    = 0.75
        )
    
    def __init__(self, **traits):
        super(CScript, self).__init__(**traits)
        
    def print_file(self):
        """ Prints the file content """
        if not self.loaded:
            self.load()
        self.obj.data.seek(0)
        return self.obj.data.read()
        
    def open_file(self):
        """ Opens the file in an editor """
        if not self.loaded:
            self.load()

        self.code = self.print_file()
        self.configure_traits()
        