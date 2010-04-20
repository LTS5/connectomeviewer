""" ConnectomeFile

It contains Metadata, Networks, Surfaces, Volumes, Tracks

"""

# Enthought library imports
from enthought.traits.api import HasTraits, Instance, Any, Str, File, List, Bool, \
                                    DictStrAny

from cmetadata import CMetadata

class CFile(HasTraits):
    """ Representation of a Connectome File """
    
    #######################################
    # Public traits
    #######################################
    
    # Metadata derived from the meta.xml as cmetadata object
    metadata = Instance(CMetadata)
    
    # Dictionary of cnetwork instances keyed by name
    cnetworks = DictStrAny
    
    # Dictionary of cvolume instances keyed by name
    cvolumes = DictStrAny
    
    # Dictionary of csurface instances keyed by name
    csurfaces = DictStrAny
    
    # Dictionary of ctrack instances keyed by name
    ctracks = DictStrAny
    
    #######################################
    # Private traits
    #######################################
    
    # Reference to the source connectome file with ending .cff
    _src = File
    
    #######################################
    # Interface
    #######################################
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
        # Initialize private traits:
        
        # Initialize with source connectome file
        
    
    def add_cnetwork(self, cnetwork):
        """ Add connectome network """
        
        # add cnetwork to cnetworks
        pass
    
    def remove_cnetwork(self, cnetwork):
        """ Remove connectome network """
        
        # remove cnetwork from cnetworks
        pass
    
    def add_cvolume(self, cvolume):
        """ Add connectome volume """
        
        # add cvolume to cvolumes
        # add reference to self._src
        if self.cvolumes.has_key(cvolume.name):
            raise RuntimeError('A Connectome Volume with name %s already exists!' % cvolume.name)
        
        cvolume.set(_cfilesrc = self._src)
        
        if not cvolume.name is None and not cvolume.name == '':
            self.cvolumes[cvolume.name] = cvolume
        else:
            raise RuntimeError('Connectome Volume name is not valid!')
    
    def remove_cvolume(self, cvolume):
        """ Remove connectome volume """
        
        # remove cvolume from cvolumes
        pass
    
    def add_csurface(self, csurface):
        """ Add connectome surface """
        
        # add csurface to csurfaces
        # add reference to self._src
        if self.csurfaces.has_key(csurface.name):
            raise RuntimeError('A Connectome Surface with name %s already exists!' % csurface.name)
        
        # XXX: add src when None existing
        # XXX: fileformat? -> create csurface from gifti? from pointset & triangle array?
        
        csurface.set(_cfilesrc = self._src)
        
        if not csurface.name is None and not csurface.name == '':
            self.csurfaces[csurface.name] = csurface
        else:
            raise RuntimeError('Connectome Surface name is not valid!')
    
    def remove_csurface(self, csurface):
        """ Remove connectome surface """
        
        # remove csurface from csurfaces
        pass
    
    def add_ctrack(self, ctrack):
        """ Add connectome track """
        
        # add ctrack to ctracks
        # add reference to self._src
        if self.ctracks.has_key(ctrack.name):
            raise RuntimeError('A Connectome Track with name %s already exists!' % ctrack.name)
        
        ctrack.set(_cfilesrc = self._src)
        
        if not ctrack.name is None and not ctrack.name == '':
            self.ctracks[ctrack.name] = ctrack
        else:
            raise RuntimeError('Connectome Track name is not valid!')
    
    def remove_ctrack(self, ctrack):
        """ Remove connectome track """
        
        # remove data from memory, setting _cfilesrc to None
        # remove ctrack from ctracks
        
        pass
    
    def to_filename(self, filename):
        """ Saves the content of the connectome file to disk """
        # XXX to implement
        pass
    
    