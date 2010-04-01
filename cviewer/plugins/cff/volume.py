""" The implementation of the interface for Connectome Volumes """
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

# Enthought library imports
from enthought.traits.api import HasTraits, Str, Any, Bool, implements, DelegatesTo

# ConnectomeViewer imports
from interfaces.i_volume import IVolume

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class Volume(HasTraits):
    """ The implementation of the interface for Connectome Volumes. """

    implements(IVolume)

    # volume name
    volumename = Str
      
    # the path to the file within the cfile
    tmpfname = Str
    
    # fileformat, take better Enum Trait? (only also if in XSD)
    # allowed values: nifti
    fileformat = Str

    # is this a segmentation volume ?
    segmentation = Bool

    # description of the volume
    description = Str
    
    # the Nifti volume, as private traits
    volume = Any
    
    # the path to the temporary created file
    _tmpvolfile = Any
    
    # Reference to the parent network file
    _networkref = Any
    
    # filezip of cfile
    _filezip = DelegatesTo('_networkref')
    
    def __init__(self, volumename, networkref, segmentation = False, fileformat = 'nifti', \
                 src = '', description = ''):
        """ Initializes the Volume and initialize the traits
        
        Parameters
        ----------
        volumename : string
            the name of the volume
        fileformat : string
            the file format (default = 'nifti')
        src : string
            the path to the volume file within the cfile
        description : string
            the description of the content of the volume
            
        """
        if volumename is None:
            return
        
        self.volumename = volumename
        self._networkref = networkref
        self.fileformat = fileformat
        self.description = description
        self.segmentation = segmentation
        
        if len(src) != 0:
            # store the tmp path name for later usage (trackvis)
            self.tmpfname = src


    def load_volume(self):
        """ Extract the volume data from the cfile and store it
        in the trait volume """
        import os
        
        # create temporary file
        self._tmpvolfile = self.load_volume_to_file()
        
        # load the volume to memory
        self.volume = self._load_nifti_volume(self._tmpvolfile)
        
        
    def close_volume(self):
        """ This closes the volume thereby removing the temporary
        volume file
        """
        
        self.volume = None
        
        # unlink the file
        os.unlink(self._tmpvolfile)
        
        
    def load_volume_to_file(self):
        """ Extract the volume data from the cfile and creates
        a temporary file for usage
        
        Returns
        -------
        fname : string
            Full path to the temporary extracted volume file
            Needs to be removed/unlinked by the client.
            
        """
        import tempfile
        import os
        
        # check if file is compressed: .nii.gz
        if self.tmpfname.endswith('.gz'):
            import gzip
            srcfile_compressed = tempfile.mkstemp(prefix='compressed', suffix = '.nii.gz')
            os.write(srcfile_compressed[0],self._filezip.read(self.tmpfname))
            os.close(srcfile_compressed[0])
            f = gzip.open(srcfile_compressed[1], 'rb')
            srcfile=tempfile.mkstemp(prefix='vol', suffix='.nii')
            file_content = f.read()
            f.close()
            os.write(srcfile[0],file_content)
            os.close(srcfile[0])            
            os.unlink(srcfile_compressed[1])

        else:
            # extract the file to a tempfile
            srcfile=tempfile.mkstemp(prefix='vol', suffix='.nii')
            # but we need to write it binary here
            os.write(srcfile[0],self._filezip.read(self.tmpfname))
            os.close(srcfile[0])
        
        return srcfile[1]
        

    def _load_nifti_volume(self, fname):
        """ Return a loaded NiftiImage given the absolute path to the file
        
        Parameters
        ----------
        fname : string
            full path to a temporary nifti volume file
        
        """
        from cviewer.io.nipy.api import load_image
        return load_image(fname)

    def render_vol(self):
        """ Render the volume in the current scene3d """
        
        if self.volume is None:
            self.load_volume()
        
        from cviewer.visualization.render_volume import render_vol
        
        # get the data array
        
        if len(self.volume._data.shape) == 4:
            data = self.volume._data[0,:,:,:] # we pick the first image in a 4d array
        elif len(self.volume._data.shape) == 3:
            data = self.volume._data
            
        logger.debug('Dimension of the Volume data: %s' % str(data.shape))
        
        render_vol(data)
        
    def vol_vis(self):
        """ Invokes the Volume Visualizer by Gael Varoquaux """
        
        if self.volume is None:
            self.load_volume()
        
        from cviewer.visualization.volume.thread_volslice import ThreadedVolumeSlicer

        logger.debug('Invoke Volume Slicer...')

        action = ThreadedVolumeSlicer(fname = self._tmpvolfile)
        action.start()
        


#### EOF ######################################################################
