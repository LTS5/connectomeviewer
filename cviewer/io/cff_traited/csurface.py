""" Connectome Surface File """

# Enthought library imports
from enthought.traits.api import HasTraits, Str, File, Bool, Enum

class CSurface(HasTraits):
    """ Representation of a Connectome Surface """

    # name of the surface
    name = Str
    
    # relative path to the source file within the connectome archive
    src = Str

    # file format
    fileformat = Enum('gifti')
    
    # data type
    dtype = Enum('surfaceset', 'label', 'surfaceset+label')
    
    # description
    description = Str
    
    # is the data loaded in memory
    is_loaded = Bool(False)
    
    # reference to connectome file archive
    _cfilesrc = File
    
    def __init__(self, **traits):
        super(CSurface, self).__init__(**traits)