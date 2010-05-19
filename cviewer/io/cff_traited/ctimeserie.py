""" Connectome Timeserie File """

# Enthought library imports
from enthought.traits.api import HasTraits, Instance, Any, Str, File, List, Bool, Enum

class CTimeserie(HasTraits):
    """ Representation of a Connectome Timeserie """

    # name of the timeserie
    name = Str
    
    # relative path to the source file within the connectome archive
    src = Str

    # file format
    fileformat = Enum('hdf5')
    
    # description
    description = Str
    
    # is the data loaded in memory
    is_loaded = Bool(False)
    
    # reference to connectome file archive
    _cfilesrc = File
    
    def __init__(self, **traits):
        super(CTimeserie, self).__init__(**traits)