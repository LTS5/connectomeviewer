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
from enthought.traits.api import HasTraits, Instance, Any, Str, File, List, Bool, Property

# ConnectomeViewer imports

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
class CFile(HasTraits, cfflib.connectome):
    """This class basically allows you to create an instance for a
    loaded .cff file, and serves as the primary data source for scenes etc.
    
    The view shows all the relevant attributes.
    """

    # The name of the selected cff file
    file_name = File
    
    # Full pathname to file including filename
    fullpathtofile = Str
    
    # standard name
    name = Str('Connectome File')

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
    
    connectome_meta = Instance(cfflib.CMetadata)
    connectome_network = List
    connectome_surface = List
    connectome_volume = List
    connectome_track = List
    connectome_timeserie = List
    connectome_data = List
    connectome_script = List
    connectome_imagestack = List
    
    children = Property(depends_on = ['connectome_network', 
                                      'connectome_surface',
                                      'connectome_volume',
                                      'connectome_track',
                                      'connectome_timeserie',
                                      'connectome_data',
                                      'connectome_script',
                                      'connectome_imagestack',
                                      ])
    
    def _get_children(self):
        return self.get_all()
        
    ######################################################################
    # `object` interface.
    ######################################################################
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
        self.connectome_network = []
        self.connectome_surface = []
        self.connectome_volume = []
        self.connectome_track = []
        self.connectome_timeserie = []
        self.connectome_data = []
        self.connectome_script = []
        self.connectome_imagestack = []
    
    def load_cfile(self, filepath):
        """ Load a given cfile as path and initializes the attributes """
        
        if not os.path.isfile(filepath):
            logger.error('Not existing file: %s' %filepath)
            return
        
        # set the fullpath to internal trait
        self.fullpathtofile = filepath
        # set the file name and make it visible in the TraitsUIView
        self.file_name = os.path.split(filepath)[1]
    
        a = cfflib.load_from_cff(filepath)
        
        self.connectome_meta = a.connectome_meta
        self.connectome_network=[CNetwork(obj=ele) for ele in a.connectome_network]
        self.connectome_surface=[CSurface(obj=ele) for ele in a.connectome_surface]
        self.connectome_volume =[CVolume(obj=ele) for ele in a.connectome_volume]
        self.connectome_track =[CTrack(obj=ele) for ele in a.connectome_track]
        self.connectome_timeserie =[CTimeserie(obj=ele) for ele in a.connectome_timeserie]
        self.connectome_data =[CData(obj=ele) for ele in a.connectome_data]
        self.connectome_script =[CScript(obj=ele) for ele in a.connectome_script]
        self.connectome_imagestack =[CImagestack(obj=ele) for ele in a.connectome_imagestack]
                 

    def close_cfile(self):
        pass
        # save all the objects
        # remove all the lists
        