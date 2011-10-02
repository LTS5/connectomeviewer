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

class CTimeseries(CBase):
    """ The implementation of the Connectome Timeseries """
    
    obj = Instance(cfflib.CTimeseries)

    def __init__(self, **traits):
        super(CTimeseries, self).__init__(**traits)
        