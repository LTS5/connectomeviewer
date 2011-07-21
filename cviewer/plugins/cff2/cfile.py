""" The class for the main data object holding all information about a loaded
    connectome file
"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os, os.path
import tempfile
from threading import Thread

# Enthought library imports
from traits.api import HasTraits, Instance, Any, Str, File, List, Bool, Property, cached_property

# ConnectomeViewer imports
from cmetadata import CMetadata
from cnetwork import CNetwork
from csurface import CSurface
from cvolume import CVolume
from ctimeseries import CTimeseries
from cdata import CData
from cscript import CScript
from cimagestack import CImagestack   
from ctrack import CTrack
import cfflib

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

######################################################################
# CFile` class.
######################################################################
class CFile(HasTraits):
    """This is a wrapper class for a connectome file
    """

    # The name of the selected cff file
    file_name = File
    
    # Full pathname to file including filename
    fullpathtofile = Str
    
    # standard name
    dname = Str('Connectome File')

    obj = Instance(cfflib.connectome)

    #######################################
    # Private traits.
    #######################################
    
    # zipfile
    _filezip = Any
    
    # data loaded
    _data_loaded = Bool(False)
    
    # reference to the current window for statusbar updates
    # not nice MVC-style
    _workbenchwin = Instance('pyface.workbench.api.WorkbenchWindow')
    
    connectome_meta = Property(Any)
    connectome_network = Property(List)
    connectome_surface = Property(List)
    connectome_volume = Property(List)
    connectome_track = Property(List)
    connectome_timeserie = Property(List)
    connectome_data = Property(List)
    connectome_script = Property(List)
    connectome_imagestack = Property(List)
    
    children = List
    
    def _obj_changed(self):
        self.update_children()
    
    def update_children(self):
        # remove all internal structures
        self._connectome_surface = None
        self._connectome_network = None
        self._connectome_volume = None
        self._connectome_track = None
        self._connectome_timeserie = None
        self._connectome_data = None
        self._connectome_script = None
        self._connectome_imagestack = None

        self.children = self.connectome_network + self.connectome_surface + \
                self.connectome_volume + self.connectome_track + \
                self.connectome_timeserie + self.connectome_data + \
                self.connectome_script + self.connectome_imagestack
    
    def _get_connectome_meta(self):
        if self.obj is None:
            return None
        else:
            return CMetadata(obj=self.obj.connectome_meta)
        
    def _get_connectome_network(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_network is None:
                self._connectome_network = [CNetwork(obj=ele) for ele in self.obj.connectome_network]
                return self._connectome_network
            else:
                return self._connectome_network

    def _get_connectome_surface(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_surface is None:
                self._connectome_surface = [CSurface(obj=ele) for ele in self.obj.connectome_surface]
                return self._connectome_surface
            else:
                return self._connectome_surface
        
    def _get_connectome_volume(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_volume is None:
                self._connectome_volume = [CVolume(obj=ele) for ele in self.obj.connectome_volume]
                return self._connectome_volume
            else:
                return self._connectome_volume
        
    def _get_connectome_track(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_track is None:
                self._connectome_track = [CTrack(obj=ele) for ele in self.obj.connectome_track]
                return self._connectome_track
            else:
                return self._connectome_track
        
    def _get_connectome_timeserie(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_timeserie is None:
                self._connectome_timeserie = [CTimeseries(obj=ele) for ele in self.obj.connectome_timeseries]
                return self._connectome_timeserie
            else:
                return self._connectome_timeserie

    def _get_connectome_data(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_data is None:
                self._connectome_data = [CData(obj=ele) for ele in self.obj.connectome_data]
                return self._connectome_data
            else:
                return self._connectome_data
        
    def _get_connectome_script(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_script is None:
                self._connectome_script = [CScript(obj=ele) for ele in self.obj.connectome_script]
                return self._connectome_script
            else:
                return self._connectome_script
        
    def _get_connectome_imagestack(self):
        if self.obj is None:
            return []
        else:
            if self._connectome_imagestack is None:
                self._connectome_imagestack = [CImagestack(obj=ele) for ele in self.obj.connectome_imagestack]
                return self._connectome_imagestack
            else:
                return self._connectome_imagestack

    
    ######################################################################
    # `object` interface.
    ######################################################################
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
        self._connectome_surface = None
        self._connectome_network = None
        self._connectome_volume = None
        self._connectome_track = None
        self._connectome_timeserie = None
        self._connectome_data = None
        self._connectome_script = None
        self._connectome_imagestack = None
    
    
    def load_cfile(self, filepath, ismetacml = False):
        """ Load a given cfile as path and initializes the attributes """
        
        if not os.path.isfile(filepath):
            logger.error('Not existing file: %s' %filepath)
            return
        
        # set the fullpath to internal trait
        self.fullpathtofile = filepath
        # set the file name and make it visible in the TraitsUIView
        self.file_name = os.path.split(filepath)[1]
    
        self.obj = cfflib.load(filepath)
            
    def close_cfile(self):
#        if not self.obj.iszip:
#            logger.info("Save connectome file data.")
        pass
        # save all the objects
        # remove all the lists
        
    def show_surface(self):
        # invokes a code oracle
        # get loaded surfaces
        from cviewer.plugins.codeoracle.csurface_action import SurfaceParameter
        so = SurfaceParameter()
        so.edit_traits()

    
        
        
        