""" The ConnectomeViewer wrapper for a cfflib object """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os
import subprocess

# Enthought library imports
from enthought.traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo, Property
from enthought.traits.ui.api import View, Item, auto_close_message, message

import cfflib
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.plugins.cff2.trackvis.main import ThreadedTrackvis
# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

from cbase import CBase

class CTrack(CBase):
    """ The implementation of the Connectome Track """
    
    obj = Instance(cfflib.CTrack)
    
    
    def __init__(self, **traits):
        super(CTrack, self).__init__(**traits)
    
    def get_fibdata(self):
        """ Return a Numpy Object of TrackVis fibers """
        if not self.loaded:
            self.load()
         
        if self.obj.get_fileformat() == 'TrackVis':
            return self.obj.get_fibers_as_numpy()
        else:
            return None
    
    
    def launch_trackvis(self, volumefname = None):
        """ Launches TrackVis externally """
        logger.info("Launch TrackVis...")
        
        if not self.loaded:
            self.load()
        
        pref = preference_manager.preferences
        
        tvp = pref.get('cviewer.plugins.ui.trackvispath')
        if tvp is None:
            logger.error("Path to TrackVis executable does not exist. Please update preferences.")
            return
        
        tvpath = os.path.join(tvp,'trackvis')
        trackfname = self.obj.tmpsrc
        
        tvt = ThreadedTrackvis(tvpath, trackfname)
        tvt.start()
    
        
    def render_tracks(self):
        """ Renders the tracks in a separate window using DiPy Fos """
        from cviewer.libs.dipy.viz import fos
        if not self.loaded:
            self.load()
            
        # copy tracks for visualization
        T = [i[0] for i in self.obj.data[0]]
        r=fos.ren()
        fos.add(r,fos.line(T,fos.white,opacity=0.8))
        fos.show(r)
        