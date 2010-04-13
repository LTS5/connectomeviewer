""" Connectome Track File """

# Enthought library imports
from enthought.traits.api import HasTraits, Instance, Any, Str, File, List, Bool, Enum

class CTrack(HasTraits):
    """ Representation of a Connectome Track """

    # name of the track
    name = Str
    
    # relative path to the source file within the connectome archive
    src = Str

    # file format
    fileformat = Enum('trackvis')
    
    # description
    description = Str
    
    # is the data loaded in memory
    is_loaded = Bool(False)
    
    # reference to connectome file archive
    _cfilesrc = File
    
    def __init__(self, **traits):
        super(CTrack, self).__init__(**traits)