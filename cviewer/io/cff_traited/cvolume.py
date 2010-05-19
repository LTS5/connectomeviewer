""" Connectome Volume File """

# Enthought library imports
from enthought.traits.api import HasTraits, Str, File, Bool, Enum

class CVolume(HasTraits):
    """ Representation of a Connectome Volume """

    # name of the volume
    name = Str
    
    # relative path to the source file within the connectome archive
    src = Str

    # file format
    fileformat = Enum('nifti')
    
    # data type
    dtype = Enum('segmentation', 'T1-weighted', 'T2-weighted', 'PD-weighted', \
                 'fMRI', 'probabilitymap', 'MD', 'FA', 'LD', 'TD')
    
    # description
    description = Str
    
    # is the data loaded in memory
    is_loaded = Bool(False)
    
    # reference to connectome file archive
    _cfilesrc = File
    
    def __init__(self, **traits):
        super(CVolume, self).__init__(**traits)