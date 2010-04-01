""" The interface for Connectome Surfaces. """
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
from enthought.traits.api import Interface, Str, Any

class ISurface(Interface):
    """ The interface for Connectome Surfaces. """

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
        
    def _layout_3DView(self):
        """ Layouts the 3D View with this surface """


    def _set_container(self, surfcontainer):
        """ Sets the SurfaceContainer of this surface
        
        Parameters
        ----------
        surfcontainer : in
            adding a `SurfaceContainer` instance
            
        """
      
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
        
#### EOF ######################################################################
