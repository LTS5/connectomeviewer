""" The class for the main data object holding all information about a loaded
    connectome file
"""
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
import os, os.path
import tempfile
from threading import Thread

# Enthought library imports
from enthought.traits.api import HasTraits, Instance, Any, Str, File, List, Bool, Property, cached_property

# ConnectomeViewer imports
from cmetadata import CMetadata
from cnetwork import CNetwork
from csurface import CSurface
from cvolume import CVolume
from ctimeserie import CTimeserie
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
    """This class basically allows you to create an instance for a
    loaded .cff file, and serves as the primary data source for scenes etc.
    
    The view shows all the relevant attributes.
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
    _workbenchwin = Instance('enthought.pyface.workbench.api.WorkbenchWindow')
    
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
        self._update_children()
        
    def _update_children(self):
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
            
            return 

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
            return [CVolume(obj=ele) for ele in self.obj.connectome_volume]
        
    def _get_connectome_track(self):
        if self.obj is None:
            return []
        else:
            return [CTrack(obj=ele) for ele in self.obj.connectome_track]
        
    def _get_connectome_timeserie(self):
        if self.obj is None:
            return []
        else:
            return [CTimeserie(obj=ele) for ele in self.obj.connectome_timeserie]

    def _get_connectome_data(self):
        if self.obj is None:
            return []
        else:
            return [CData(obj=ele) for ele in self.obj.connectome_data]
        
    def _get_connectome_script(self):
        if self.obj is None:
            return []
        else:
            return [CScript(obj=ele) for ele in self.obj.connectome_script]
        
    def _get_connectome_imagestack(self):
        if self.obj is None:
            return []
        else:
            return [CImagestack(obj=ele) for ele in self.obj.connectome_imagestack]

    
    ######################################################################
    # `object` interface.
    ######################################################################
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
        self._connectome_surface = None
        self._connectome_network = None
        
    def load_cfile(self, filepath, ismetacml = False):
        """ Load a given cfile as path and initializes the attributes """
        
        if not os.path.isfile(filepath):
            logger.error('Not existing file: %s' %filepath)
            return
        
        # set the fullpath to internal trait
        self.fullpathtofile = filepath
        # set the file name and make it visible in the TraitsUIView
        self.file_name = os.path.split(filepath)[1]
    
        if ismetacml:
            self.obj = cfflib.load_from_meta_cml(filepath)
        else:
            self.obj = cfflib.load_from_cff(filepath)
            
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

    
        
        
        