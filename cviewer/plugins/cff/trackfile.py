""" The implementation of the interface for the Trackfiles """
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
from enthought.traits.api import HasTraits, Str, implements, Any, DelegatesTo, Bool

# ConnectemeViewer imports
from interfaces.i_trackfile import ITrackfile

class Trackfile(HasTraits):
    """ The implementation of the interface for a Trackfile (.trk) """

    implements(ITrackfile)

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
        self.trkname = trkname
        self.tmpfname = tmpfname
        self.fileformat = fileformat
        self.description = description
        self._networkref = networkref
        
    def load_trackfile(self):
        """ Loads the trackfile data and header using DiPy IO
        
        Returns
        -------
        Boolean: Successfull or not
        """
        from cviewer.plugins.dipy.io import trackvis as tv
        
        fname = self.load_trackfile_to_file()
        try:
            self.streams, self.hdr = tv.read(fname)
        except HeaderError:
            return False
        
        self.data_loaded = True
        return True
        
    def load_trackfile_to_file(self):
        """ Extracts the trackfile from the cfile and creates
        it as temporary file for usage
        
        """
        import tempfile
        import os
        
        # create a temporary file
        srcfile=tempfile.mkstemp(prefix='track', suffix='.trk')

        # only one trackfile per network is allowed
        os.write(srcfile[0],self._filezip.read(self.tmpfname))
        os.close(srcfile[0])
        
        return srcfile[1]
        
    def render_tracks(self):
        """ Renders the tracks in a separate window using DiPy Fos """
        from cviewer.plugins.dipy.viz import fos
        if not self.data_loaded:
            self.load_trackfile()
        # copy tracks for visualization
        T = [i[0] for i in self.streams]
        r=fos.ren()
        fos.add(r,fos.line(T,fos.white,opacity=0.1))
        fos.show(r)

#### EOF ######################################################################
