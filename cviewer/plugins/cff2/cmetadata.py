""" The ConnectomeViewer wrapper for a cfflib object """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os
import subprocess

# Enthought library imports
from traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo, Property
from traitsui.api import View, Item, auto_close_message, message

import cfflib
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.plugins.cff2.trackvis.main import ThreadedTrackvis
# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

from cbase import CBase

class CMetadata(CBase):
    """ The implementation of the Connectome Metadata """
    
    obj = Instance(cfflib.CMetadata)
    
    def __init__(self, **traits):
        super(CMetadata, self).__init__(**traits)
        