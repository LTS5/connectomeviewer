""" The ConnectomeViewer wrapper for a cfflib object """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os

# Enthought library imports
from traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo, Property
from traitsui.api import View, Item, auto_close_message, message

import cfflib

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

from cbase import CBase

class CVolume(CBase):
    """ The implementation of the Connectome Volume """
    
    obj = Instance(cfflib.CVolume)

    def vol_vis(self):
        """ Invokes the Volume Visualizer by Gael Varoquaux """
        
        if not self.loaded:
            self.load()
        
        from cviewer.visualization.volume.thread_volslice import ThreadedVolumeSlicer

        logger.debug('Invoke Volume Slicer...')

        action = ThreadedVolumeSlicer(fname = self.obj.tmpsrc)
        action.start()
        
    def __init__(self, **traits):
        super(CVolume, self).__init__(**traits)
        