""" The ConnectomeViewer wrapper for a cfflib object """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports

# Enthought library imports
from enthought.traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo, Property
from enthought.traits.ui.api import View, Item, auto_close_message, message

import cfflib

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CBase(HasTraits):
    """ The implementation of the Connectome Base class """

    # network name as seen in the TreeView
    name = Property(Str, depends_on = [ 'obj' ])
    
    # display name
    dname = Str
    
    # loaded
    loaded = Bool(False)
    
    # data exposed
#    data = Property(Any, depends_on = ['obj'] )
    
    # active window
    window = Instance('enthought.pyface.workbench.api.WorkbenchWindow')
    
    # private traits
    ###########
    
    # parent cfile this networks belongs to
    _parentcfile = Any

    # filezip of cfile
    _filezip = DelegatesTo('_parentcfile')

    def _get_name(self):
        return self.obj.name
    
#    def _get_data(self):
#        if self.loaded:
#            return self.obj.data
#        else:
#            print "No data. Need to load the object first."
    
    def load(self):
        self.obj.load()
        self.loaded = True
        
    def close(self):
        if self.loaded:
            logger.debug("Save...")
            self.obj.save()
        logger.debug("Delete from memory...")
        del self.obj.data
        self.loaded = False
        logger.debug("Done.")

    def _dname_default(self):
        return self.name

    def _loaded_changed(self, value):
        if value:
            n = self.dname
            if ' [Active]' not in n:
                self.dname = "%s [Loaded]" % n
                
            if not self.window is None:
                #from enthought.pyface.timer.api import do_later
                from enthought.pyface.api import GUI
                GUI.invoke_later(self.window.status_bar_manager.set, message = '')
            
        else:
            self.dname = self.dname.replace(' [Loaded]', '')
    
    def __init__(self, **traits):
        super(CBase, self).__init__(**traits)
        