""" The implementation of the interface for surfaces using Gifti format. """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports.
from enthought.traits.api import HasTraits, implements, Str, Any

# ConnectomeViewer imports
from interfaces.i_surface import ISurface

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class Surface(HasTraits):
    """ The implementation for Connectome Surfaces. """

    implements(ISurface)

    # name
    name = Str
  
    # AnatomicalStructurePrimary
    primary = Str
    
    # AnatomicalStructureSecondary
    secondary = Str

    # the information from the metatags of the gifti file
    description = Str
    
    # the surfacecontainer this surface belongs to (parent)
    _container = Any
        
    def __init__(self, primary, secondary, description = ''):
        """ Initializes the Surface and initialize the traits
        
        Parameters
        ----------
        primary : string
            the primary anatomical structure
        secondary : string
            the secondary anatomical structure
        description : string
            the description of the content of the surface
        
        """
        self.primary = primary
        self.name = secondary
        self.secondary = secondary
        self.description = description
        
    def _layout_3DView(self):
        """ Layouts the 3D View with this surface """

        # what index has the surface container?
        surfcontid = self._container._networkref.surfaces.index(self._container)
        
        # what index has this surface?
        surfid = self._container.surfaces.index(self)
        
        logger.debug('SurfaceContainer evaluated: %s and surface id: %s' % (surfcontid, surfid))
        
        # invoke the layouting
        self._container._networkref.rendermanager.layout_3dview(surfcontid, surfid)
        

    def _set_container(self, surfcontainer):
        """ Sets the SurfaceContainer of this surface
        
        Parameters
        ----------
        surfcontainer : in
            adding a `SurfaceContainer` instance
            
        """
        self._container = surfcontainer
      
    def get_geometry(self):
        """ Gets the geometry information of this surface
        
        Returns
        -------
        daV : array_like
            a set of vertices
        daF : array_like
            a set of faces (triangles)
        labelarr : array_like
            array with label intensity values
            
        """
        daV, daF = self._container.get_geometry(primary=self.primary, secondary=self.secondary)
        
        labelarr = self._container.get_label()
        
        return (daV, daF, labelarr)


#### EOF ######################################################################
