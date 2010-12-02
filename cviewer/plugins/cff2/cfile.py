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
from enthought.traits.api import HasTraits, Instance, Any, Str, File, List, Bool

# ConnectomeViewer imports

from cnetwork import CNetwork

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
    
        
    ######################################################################
    # `object` interface.
    ######################################################################
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
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
        self.__init__(connectome_meta=a.connectome_meta,
                        connectome_network=a.connectome_network,
                        connectome_surface=a.connectome_surface,
                        connectome_volume=a.connectome_volume,
                        connectome_track=a.connectome_track,
                        connectome_timeserie=a.connectome_timeserie,
                        connectome_data=a.connectome_data,
                        connectome_script=a.connectome_script,
                        connectome_imagestack=a.connectome_imagestack)
        
        #self.connectome_network = a.get_connectome_network()
    
#        # open the .cff file with Zip
#        from zipfile import ZipFile, ZIP_DEFLATED
#        self._filezip = ZipFile(self.fullpathtofile, 'r', ZIP_DEFLATED)
#        metadatastring = self._filezip.read('meta.cml')
#        
#        print "we got metadata", metadatastring
#                