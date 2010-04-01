""" The interface for the Trackfiles. """
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

# Enthought library imports.
from enthought.traits.api import Interface, Str, DelegatesTo, Any, Bool

class ITrackfile(Interface):
    """ The interface for a Trackfile (.trk) """

    # trackfilename
    trkname = Str
          
    # source path name within the cfile
    tmpfname = Str
    
    # fileformat, only supports trackvis .trk so far
    fileformat = Str
    
    # description of the trackfile
    description = Str
    
    # trackvis header
    hdr = Any
    
    # trackvis fiber data (streamlines)
    streams = Any
    
    # data loaded
    data_loaded = Bool(False)
    
    # filezip with delegate
    _filezip = DelegatesTo('_networkref')
    
    # reference to parent network
    _networkref = Any
    
    def __init__(self, trkname, tmpfname, networkref, \
                 fileformat = 'trk', description = ''):
        """ Initializes by settting all the traits
        
        Parameters
        ----------
        trkname : string
            name of the trackfile
        tmpfname : string
            path to trackfile in the cfile
        networkref : INetwork object
            reference to the parent network for this trackfile
        fileformat : string
            the file format (default = 'nifti')
        description : string
            the description of the content of the trackfile
        
        """

    def load_trackfile(self):
        """ Loads the trackfile data and header using DiPy IO
        
        Returns
        -------
        Boolean: Successfull or not
        """
        
    def load_trackfile_to_file(self):
        """ Extracts the trackfile from the cfile and creates
        it as temporary file for usage
        
        """
    def render_tracks(self):
        """ Renders the tracks in a separate window using DiPy Fos """

#### EOF ######################################################################
