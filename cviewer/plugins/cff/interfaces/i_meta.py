""" The interface for Connectome File Metadata. """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports.
from enthought.traits.api import Interface, Str, Int

class IMeta(Interface):
    """ The interface for Connectome File Metadata. """

    # Connectome File Format Version
    version = Str
    
    # Generator
    generator = Str
    
    # Initial creator
    initial_creator = Str
    
    # Creation date
    creation_date = Str
    
    # Modification Date
    modification_date = Str
    
    # the name of the cfile
    name = Str
    
    # Data about which species
    species = Str
    
    # Legal notice
    legal_notice = Str
    
    # A reference, e.g. where the data was used
    reference = Str
    
    # URL with more information or data source
    url = Str
    
    # Description of the content of the File
    description = Str
    
    # Nr of networks contained in this File
    nr_of_networks = Int
            

    def parse_meta_xml(self):
        """ Parses the given file and returns True if all Traits are set.

        Returns a Boolean

        """
    


#### EOF ######################################################################
