""" The interface for Connectome Surfaces Container. """
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
from enthought.traits.api import Interface, Str, Any, List, Bool, DelegatesTo

# Local imports
from i_surface import ISurface

class ISurfaceContainer(Interface):
    """ The interface for Connectome Surfaces Container. """

    # the name of the surface container
    name = Str
    
    # fileformat, take better Enum Trait? (only also if in XSD)
    # allowed values: gifti (, asc)
    fileformat = Str
    
    # is this surface container already loaded?
    container_loaded = Bool(False)
    
    # the surfaces this object contains
    surfaces = List(ISurface)
    
    # is this surface container related to an atlas?
    is_atlas = Bool
    
    # the local path to the Gifti file in the cfile with the surfaces
    _surfacepath = Str
    
    # the Gifti file as private trait
    _surface = Any

    # the lxml node
    _lxml_node = Any
    
    # the lxml node prefix
    _lxml_node_prefix = Str
    
    # Label handling
    # --------------
    
    # the local path to the label file in the cfile
    _labelpath = Str
    
    # the Label Gifti file as private trait
    _label = Any
    
    # the labelid for the _label file
    _labelid = Str
    
    # network this surface container belongs to
    _networkref = Any
    
    # filezip of the cfile
    _filezip = DelegatesTo('_networkref')
    
    def __init__(self, name, src = '', fileformat = 'gifti'):
        """ Initializes the SurfaceContainer and initialize the traits
        
        Parameters
        ----------
        name : string
            the name of the surface container
        src :  string
            the local path to Gifti surface in the cfile or a absolute path
        fileformat : string
            the file format (default = 'gifti')
        
        """

    def load_surface_container(self):
        """ Loads the surface container from the filezip of cfile and sets
        all traits appropriately (surfaces and labels)
        
        """

    def _add_structures(self):
        """ Finds all the actual surfaces for this surface container """

    def _load_gifti_surface(self, fname):
        """ Return a loaded GiftiImage given the absolute path to the file
        
        Parameters
        ----------
        fname : string
            full path to the gifti surface file
        
        """
        
    def get_gifti_image(self):
        """ Get the Gifti Image """
        
    def _add_label(self, labelid, src = '', fileformat = 'gifti'):
        """ Add a label file to this SurfaceContainer
        
        Parameters
        ----------
        labelid : string
            the labelid given in the meta.xml
        src : string
            the local path to the gifti label file in the cfile or a abosolute path
        fileformat : string
            the fileformat for the given file, default = gifti
        
        """

    def add_surface(self, surface):
        """ Adds a particular surface to this surface container
        
        Parameters
        ----------
        surface : `Surface` instance
        
        """

    def get_surface_secondary(self, secondary):
        """ Return a surface from the container matching the secondary string
        
        Parameters
        ----------
        secondary : string
            the name of the secondary anatomical structure
        
        """

    def get_label(self):
        """ Returns an array giving the intensityvalues for each verticesid """

    def get_geometry(self, primary, secondary):
        """ Get geometry information, pointset daV and triangleset daF,
        given a primary structure (e.g. CortexLeft) and secondary structure (e.g. Pial)
        
        Parameters
        ----------
        primary : string
            name of the primary structure
        secondary : string
            name of the secondary structure
            
        Returns
        -------
        daV : array_like
            array of vertices locations
        daF : array_like
            array of faces

        """
        
#### EOF ######################################################################
