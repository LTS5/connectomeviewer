""" The implementation of the interface for Connectome File Metadata """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.traits.api import HasTraits, Str, implements, Int, Button
from enthought.traits.ui.api import View, Item, Group, spring

# ConnectomeViewer import
from interfaces.i_meta import IMeta

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class Meta(HasTraits):
    """ The implementation for Connectome File Metadata. """

    implements(IMeta)

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
    
    # helper button
    openurl = Button
    
    # Group
    infogroup = Group(
      Item( 'generator', style = 'readonly', label = 'Generator:'),
      Item( 'name', style = 'readonly', label = 'Short Name:'),
      Item( 'initial_creator', style = 'readonly', label = 'Initial Creator:'),
      Item( 'creation_date', style = 'readonly', label = 'Creation Date'),
      Item( 'modification_date', style = 'readonly', label = 'Modification Date'),
      Item( '_'),
      Item( 'species', style = 'readonly', label = 'Species:'),
      #Item( 'useatlas', style = 'readonly', label = 'Use Atlas:'),
      Item( 'legal_notice', style = 'readonly', label = 'Legal Notice:'),
      Item( 'reference', style = 'readonly', label = 'References:'),
      Item( 'url', style = 'readonly', label = 'URL:'),
      Item( name = 'openurl', label = 'Open URL ...', show_label = False, visible_when = "url != ''"),
      spring,
      Item( 'description', style = 'custom', show_label=False, springy=True,\
           tooltip='The description of the content of this Connectome File'),
    )
      
    traits_view = View(
        infogroup,
        title = 'Connectome File: Metadata',
        width = 500,
        height = 450,
        resizable = True,
        buttons = ['OK', ]
    )

    def __init__(self, filestring = None):
        """ Parses the file given as a xml string
        
        """
        if not filestring is None:
            # invoke the parser
            self.parse_meta_xml(filestring)

    def _openurl_changed(self):
        """ Button was clicked """
        from cviewer.action.help import browser_open
        browser_open(url=self.url, decorated = True)


    def parse_meta_xml(self, filestring):
        """ Parses the given file and returns True if all Traits are set.

        Parameters
        ----------
        filestring: StringIO
            String object to parse metadata

        """
        from lxml import etree
        tree = etree.fromstring(filestring)
        
        # first child, loop through the meta-info
        nsprefix = "{%s}" % tree.nsmap[None]
        
        # Parse the KEYs
        for child in tree.iterchildren():
            if child.tag == (nsprefix+'viewer-meta'):
                self.version = child.attrib['version']
                for mchildren in child.iterchildren():
                    if mchildren.tag == (nsprefix + 'generator'):
                      if not mchildren.text is None:
                        self.generator = mchildren.text
                    elif mchildren.tag == (nsprefix + 'initial-creator'):
                      if not mchildren.text is None:
                        self.initial_creator = mchildren.text
                    elif mchildren.tag == (nsprefix + 'creation-date'):
                      if not mchildren.text is None:
                        self.creation_date = mchildren.text
                    elif mchildren.tag == (nsprefix + 'modification-date'):
                      if not mchildren.text is None:
                        self.modification_date = mchildren.text
                    elif mchildren.tag == (nsprefix + 'name'):
                      if not mchildren.text is None:
                        self.name = mchildren.text
                    elif mchildren.tag == (nsprefix + 'species'):
                      if not mchildren.text is None:
                        self.species = mchildren.text
                    elif mchildren.tag == (nsprefix + 'legal-notice'):
                      if not mchildren.text is None:
                        self.legal_notice = mchildren.text
                    elif mchildren.tag == (nsprefix + 'references'):
                      if not mchildren.text is None:
                        self.reference = mchildren.text
                    elif mchildren.tag == (nsprefix + 'url'):
                      if not mchildren.text is None:
                        self.url = mchildren.text
                    elif mchildren.tag == (nsprefix + 'description'):
                      if not mchildren.text is None:
                        self.description = mchildren.text
                    elif mchildren.tag == (nsprefix + 'nr_of_networks'):
                      if not mchildren.text is None:
                        self.nr_of_networks = int(mchildren.text)
                    else:
                        logger.info('Tag '+ mchildren.tag + ' not found in meta.xml')
        
#############