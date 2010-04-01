#!/usr/bin/env python

# TODO
# - view for metadata for write
# - treeview for networks, surfaces?
# - console output for validation
# - multiple selects for graphml files
# - script to write meta.xml

from enthought.traits.api import HasTraits, Str, Directory, List, Instance
from enthought.traits.ui.api \
    import View, Item, Group, spring


from cviewer.plugins.cff.cfile import CFile
from cviewer.plugins.cff.interfaces.i_network import INetwork

meta_view = View(
        
        height = 600
    )

class CFFCreator(CFile):
    """ An class to represent a newly created Connectome File object
    
    """
    
    # additional attributes
    
    filename = Str
    
    save_path = Directory
    
    networks = List(Instance(INetwork))
    
    # initialize empty
    
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
        from cviewer.plugins.cff.meta import Meta
        from cviewer.plugins.cff.network import Network
        
        self.metadata = Meta(view = meta_view)
        mynet = Network(name = 'Sample Network')
        self.networks.append(mynet)
    
    
    infogroup = Group(
        Item( 'name', label = 'Connectome File Name'),
        Item( 'filename', label = 'Output Filename'),
        Item( 'save_path', label = 'Output Directory'),
        Item( 'metadata', label = 'Metadata'),
        Item( 'networks', label = 'Networks', style = 'custom')
    )


    
    traits_view = View(
        infogroup,
        title = 'Connectome File Creator',
        width = 600,
        height = 450,
        resizable = True,
        buttons = ['Validate', 'Save', 'Cancel']
    )

    # additional traits

    # view for the cf, title
    
    # different view for the networks/surfaces etc.?

    # validation methods to check cf
    

    traits_view

if __name__ == '__main__':
    
    cffcreator = CFFCreator()
    cffcreator.configure_traits()