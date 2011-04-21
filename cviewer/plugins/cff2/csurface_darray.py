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

import cfflib
try:
    from nibabel.nifti1 import intent_codes
except ImportError:
    print("Please install Nibabel >= 1.1.0")

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)


class CSurfaceDarray(HasTraits):
    """ The implementation of the Connectome Surface data array """
    
    def __init__(self, darray, **traits):
        super(CSurfaceDarray, self).__init__(**traits)
        
        self.data = darray
        
        if not self.data.meta is None:
            getdict = self.data.get_metadata()
            prim = ''
            if getdict.has_key('AnatomicalStructurePrimary'):
                prim = getdict['AnatomicalStructurePrimary']
            sec = ''
            if getdict.has_key('AnatomicalStructureSecondary'):
                sec = getdict['AnatomicalStructureSecondary']
                
            # name resolution
            if prim == '':
                if sec == '':
                    dname = 'Data arrays (%s)' % str(intent_codes.label[self.data.intent])
                else:
                    dname = '%s (%s)' % (sec, str(intent_codes.label[self.data.intent]))
            else:
                if sec == '':
                    dname = '%s (%s)' % (prim, str(intent_codes.label[self.data.intent]))
                else:
                    dname = '%s / %s (%s)' % (prim, sec, str(intent_codes.label[self.data.intent]))
        else:
            dname = 'Data arrays (%s)' % str(intent_codes.label[self.data.intent])
            
        self.dname = dname
        # attach it to parent?
        
