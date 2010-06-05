""" The implementation of the interface for Surface Container """
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
from enthought.traits.api import HasTraits, implements, Str, Any, List, DelegatesTo, Bool

# ConnectomeViewer imports
from interfaces.i_surface_container import ISurfaceContainer
from interfaces.i_surface import ISurface
from surface import Surface
import cviewer.io.gifti.gifti as gifti

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class SurfaceContainer(HasTraits):
    """ The implementation for Connectome Surfaces Container. """

    implements(ISurfaceContainer)

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
        self.name = name
        self.fileformat = fileformat
        self._surfacepath = src
        
        
    def load_surface_container(self):
        """ Loads the surface container from the filezip of cfile and sets
        all traits appropriately (surfaces and labels)
        
        """
        import os, os.path
        if self.container_loaded:
            logger.info('The SurfaceContainer is already loaded: %s' % self.name)
            return
        
        import tempfile
        
        if not len(self._surfacepath) == 0:
            
            if os.path.isfile(self._surfacepath):
                # load surface file
                logger.info('Loading SurfaceContainer: %s' % self.name)
                self._surface = self._load_gifti_surface(self._surfacepath)
                logger.info('Done.')
            else:
                # extract from zip first
                # create temporary file for surface
                srcfile=tempfile.mkstemp(prefix='surf', suffix='.gii')
                os.write(srcfile[0],self._filezip.read(self._surfacepath))
                # the one million dollar line
                os.close(srcfile[0])
                # load surface file
                logger.info('Loading SurfaceContainer: %s' % self.name)
                self._surface = self._load_gifti_surface(srcfile[1])
                logger.info('Done.')
                
                # close and remove temporary files since the are not used anymore
                try:
                    logger.debug('Unlink: %s' % srcfile[1])
                    os.unlink(srcfile[1])
                except os.error:
                    logger.error('An exception occured while trying to unlink the file '+srcfile[1])
    
            # add all the structures that can be found
            self._add_structures()


        # now we try to load the label information        
        if not len(self._labelpath) == 0:

            if os.path.isfile(self._labelpath):
                # load label file
                self._label = self._load_gifti_surface(self._labelpath)
            else:
                # first extract from zip
                # extract the label file and create temporary file for label gifti file
                labelfile=tempfile.mkstemp(prefix='label', suffix='.gii')
                os.write(labelfile[0],self._filezip.read(self._labelpath))
                # the one million dollar line
                os.close(labelfile[0])
                self._label = self._load_gifti_surface(labelfile[1])
            
                # unlink the label files
                try:
                    logger.debug('Unlink: %s' % labelfile[1])
                    os.unlink(labelfile[1])
                except os.error:
                    logger.error('An exception occured while trying to unlink the file '+labelfile[1])
        
        container_loaded = True

    def _add_structures(self):
        """ Finds all the actual surfaces for this surface container """

        # loop over all POINTSET array existing in the _surface gifti
        # hidden assumption: for every POINTSET there is corresponding TRIANGLESET
        # in the gifti file
        logger.debug('Searching in SurfaceContainer for structures ...')
        for array in self._surface.getArraysFromIntent(gifti.GiftiIntentCode.NIFTI_INTENT_POINTSET):
            
            # TODO: improve: Gifti file might have only Primary structure!
            # DO: only take secondary structure names for surfaces!
            
            #if array.get_meta_as_dict().has_key('AnatomicalStructurePrimary') and \
            #    array.get_meta_as_dict().has_key('AnatomicalStructureSecondary'):
            
            if array.get_meta_as_dict().has_key('AnatomicalStructureSecondary'):
                
                prim = array.get_meta_as_dict()['AnatomicalStructurePrimary']
                sec = array.get_meta_as_dict()['AnatomicalStructureSecondary']
                
                if array.get_meta_as_dict().has_key('Description'):
                    desc = array.get_meta_as_dict()['Description']
                    
                logger.debug('Found a structure in the Gifti file as surface:')
                logger.debug('Primary: '+prim+' Secondary: '+sec)
                
                self.add_surface(Surface(primary=prim, secondary=sec, description=desc))

    def _load_gifti_surface(self, fname):
        """ Return a loaded GiftiImage given the absolute path to the file
        
        Parameters
        ----------
        fname : string
            full path to the gifti surface file
        
        """
        return gifti.loadImage(fname)
        
    def get_gifti_image(self):
        """ Get the Gifti Image """
        return self._surface
        
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
        
        self._labelid = labelid
        self._labelpath = src
        
        
    def add_surface(self, surface):
        """ Adds a particular surface to this surface container
        
        Parameters
        ----------
        surface : `Surface` instance
        
        """
        # sets a reference from the surface to this container
        # to allow retrieving of its geometry directly
        surface._set_container(self)
        self.surfaces.append(surface)
        logger.debug('Added surface to SurfaceContainer List')

    def get_surface_secondary(self, secondary):
        """ Return a surface from the container matching the secondary string
        
        Parameters
        ----------
        secondary : string
            the name of the secondary anatomical structure
        
        """
        for surf in self.surfaces:
            if surf.secondary == secondary:
                return surf
        

    def get_label(self):
        """ Returns an array giving the intensityvalues for each verticesid """
        for array in self._label.getArraysFromIntent(gifti.GiftiIntentCode.NIFTI_INTENT_LABEL):
            
            if array.get_meta_as_dict().has_key('labelid'):
                
                labelid_fromfile = array.get_meta_as_dict()['labelid']
                
                if self._labelid == labelid_fromfile:
                    # the labelid given in the meta.xml and the
                    # Gifti file match!
                    logger.debug('SurfaceContainer: Took Labelset: '+ self._labelid)
                    return array.data
                
        logger.error('Found no matching of labelid in the meta.xml and given Gifti label file!')
        logger.error('Can\'t label the surface!')
                

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
        daV = None
        daF = None
        
        if primary != '':
            
            if secondary != '':
                
                # loop pointset arrays
                for array in self._surface.getArraysFromIntent(gifti.GiftiIntentCode.NIFTI_INTENT_POINTSET):
                    
                    # TODO: it might be enough to say that only the primary structure match is enough!
                    # check if primary and secondary match
                    # if so, get this data array, loop to find triangle set and break
                    if array.get_meta_as_dict()['AnatomicalStructurePrimary'] == primary and \
                        array.get_meta_as_dict()['AnatomicalStructureSecondary'] == secondary :
                        # loop triangleset array
                        for array2 in self._surface.getArraysFromIntent(gifti.GiftiIntentCode.NIFTI_INTENT_TRIANGLE):
                            if array2.get_meta_as_dict()['AnatomicalStructurePrimary'] == primary and \
                                array2.get_meta_as_dict()['AnatomicalStructureSecondary'] == secondary:
                                daF = array2.data
                                break
                            
                        daV = array.data
                        break
     
        if not daV is None and not daF is None:
            return (daV, daF)
        else:
            return


#### EOF ######################################################################

