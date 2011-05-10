""" The interface for Connectome Volumes. """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports.
from enthought.traits.api import Interface, Str, Any, Bool, DelegatesTo

class IVolume(Interface):
    """ The interface for Connectome Volumes. """
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

    def load_volume(self):
        """ Extract the volume data from the cfile and store it
        in the trait volume """

    def close_volume(self):
        """ This closes the volume thereby removing the temporary
        volume file
        """

    def load_volume_to_file(self):
        """ Extract the volume data from the cfile and creates
        a temporary file for usage
        
        Returns
        -------
        fname : string
            Full path to the temporary extracted volume file
            Needs to be removed/unlinked by the client.
            
        """

    def _load_nifti_volume(self, fname):
        """ Return a loaded NiftiImage given the absolute path to the file
        
        Parameters
        ----------
        fname : string
            full path to a temporary nifti volume file
        
        """

#### EOF ######################################################################
