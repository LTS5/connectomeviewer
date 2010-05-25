""" Connectome Metadata """

# Enthought library imports
from enthought.traits.api import HasTraits, Str, implements, Int
from enthought.traits.ui.api import View, Item, Group, spring

class CMetadata(HasTraits):
    """ The connectome metadata object """

    # Connectome File Format Version
    version = Str
    
    # Generator
    generator = Str
    
    # Initial creator
    initial_creator = Str
    
    # Institution
    institution = Str
    
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
    
    # Additional data defined as metadata
    data = DictStrAny
    
    # Group
    infogroup = Group(
      Item( 'version', style = 'readonly', label = 'CFF Version:'),
      Item( 'generator', style = 'readonly', label = 'Generator:'),
      Item( 'name', style = 'readonly', label = 'Short Name:'),
      Item( 'initial_creator', style = 'readonly', label = 'Initial Creator:'),
      Item( 'institution', style = 'readonly', label = 'Institution:'),
      Item( 'creation_date', style = 'readonly', label = 'Creation Date'),
      Item( 'modification_date', style = 'readonly', label = 'Modification Date'),
      Item( '_'),
      Item( 'species', style = 'readonly', label = 'Species:'),
      Item( 'legal_notice', style = 'readonly', label = 'Legal Notice:'),
      Item( 'reference', style = 'readonly', label = 'Reference:'),
      Item( 'url', style = 'readonly', label = 'URL:'),
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

    def __init__(self, xmlstring):
        """ Parses the xml string """
        if not xmlstring is None:
            # invoke the parser
            
            # validate the xml file with the connectome.xsd schema
            from validator import validate_metaxml
            
            if validate_metaxml(xmlstring):
                self.parse_meta_xml(xmlstring)
            else:
                raise RuntimeError('Can not validate meta.xml from connectome file.')

    def parse_meta_xml(self, xmlstring):
        """ Parses the given file and returns True if all Traits are set.

        Parameters
        ----------
        xmlstring: StringIO
            String object or string to parse metadata

        """
        from lxml import etree
        tree = etree.fromstring(xmlstring)
        
        # first child, loop through the meta-info
        nsprefix = "{%s}" % tree.nsmap[None]
        
        # Parse the KEYs
        for child in tree.iterchildren():
            if child.tag == (nsprefix+'connectome-meta'):
                self.version = child.attrib['version']
                for mchildren in child.iterchildren():
                    if mchildren.tag == (nsprefix + 'generator'):
                        if not mchildren.text is None:
                            self.generator = mchildren.text
                    elif mchildren.tag == (nsprefix + 'initial-creator'):
                        if not mchildren.text is None:
                            self.initial_creator = mchildren.text
                    elif mchildren.tag == (nsprefix + 'institution'):
                        if not mchildren.text is None:
                            self.institution = mchildren.text
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
                    elif mchildren.tag == (nsprefix + 'reference'):
                        if not mchildren.text is None:
                            self.reference = mchildren.text
                    elif mchildren.tag == (nsprefix + 'url'):
                        if not mchildren.text is None:
                            self.url = mchildren.text
                    elif mchildren.tag == (nsprefix + 'description'):
                        if not mchildren.text is None:
                            self.description = mchildren.text
                    elif mchildren.tag == (nsprefix + 'metadata'):
                        for dchild in mchildren.iterchildren():
                            if dchild.tag == (nsprefix + 'data'):
                                if not dchild.text is None:
                                    self.data[dchild.attrib['key']] = dchild.text
                    else:
                        print 'Tag %s not valid in meta.xml!' % mchildren.tag
        