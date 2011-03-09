""" The Connectome File View as a Tree Structure """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library import
import os, os.path

# Enthought library imports
from enthought.traits.api import Instance, HasTraits, Any, Delegate, List, Either, Property
from enthought.pyface.image_cache import ImageCache
from enthought.traits.ui.api import (Item, TreeEditor, TreeNode, View, Handler, UIInfo)
from enthought.pyface.image_resource import ImageResource
from enthought.pyface.api import FileDialog, OK
from enthought.pyface.action.api import Action

# ConnectomeViewer imports
from cviewer.plugins.cff2.cnetwork import CNetwork
from cviewer.plugins.cff2.cfile import CFile
from cviewer.plugins.cff2.csurface import CSurface


from cnetwork_tree_node import CNetworkTreeNode
from csurface_tree_node import CSurfaceTreeNode
from cfile_tree_node import CFileTreeNode

from cdata_tree_node import CDataTreeNode
from cimagestack_tree_node import CImagestackTreeNode
from cscript_tree_node import CScriptTreeNode
from ctimeseries_tree_node import CTimeseriesTreeNode
from cvolume_tree_node import CVolumeTreeNode
from ctrack_tree_node import CTrackTreeNode
from csurface_darray_tree_node import CSurfaceDarrayTreeNode

from cviewer.action.common import IMAGE_PATH


# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CFFViewHandler(Handler):
    """ A handler for the CFFView object """

    def _on_dclick(self, object):
        """ Called when a node in the tree  is double-clicked.
        """
        
        if isinstance(object, CFile):
            pass
                
        else:
            if not object.loaded:
                object.load()
            else:
                object.close()
        
#        if isinstance(object, CNetwork):
#            logger.info("Network doubleclicked")
#        else:
#            pass

##############################################################################
# CFFView class.
##############################################################################
class CFFView(HasTraits):
    """ A view displaying the cfile's object tree. """

    # The connectome file we are viewing
    cfile = Instance(CFile, allow_none=True)
   
    # Path used to search for images
    _image_path = [IMAGE_PATH, ]

    # The icon of the dialog
    icon = ImageResource('favicon.ico', search_path=_image_path)

    # Nodes on the tree
    nodes = Any

    # TreeEditor 
    tree_editor = Instance(TreeEditor)

    # render actions
    #renderactions = List(Action)

#    childrenlist = Property(List, depends_on = ['cfile'] )
#    
#    def _get_childrenlist(self):
#        print "get childrenlist"
#        return self.cfile.connectome_network

    ###########################################################################
    # `object` interface.
    ###########################################################################
    def __init__(self, **traits):
        super(CFFView, self).__init__(**traits)

    ###########################################################################
    # `HasTraits` interface.
    ###########################################################################
    def default_traits_view(self):
        """ The Connectome File Tree View
        """
       
        view = View(Item(name='cfile',
                               id='cfile',
                               editor=self.tree_editor,
                               resizable=True,
                               show_label=False),
                    id='cviewer.plugins.cff2.cfile',
                    help=False,
                    resizable=True,
                    scrollable=True,
                    undo=False,
                    revert=False,
                    ok=False,
                    cancel=False,
                    icon=self.icon,
                    title = 'Connectome File View',
                    handler=CFFViewHandler)
        
        return view
    
    def _nodes_default(self):
        """ The default value of the cached nodes list.
        """

        # Now setup the view.
        nodes = [
                 CFileTreeNode(
                          node_for=[CFile],
                          children='children',
                          icon_path=IMAGE_PATH,
                          ),
                 CNetworkTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 CSurfaceTreeNode(
                          children='children',
                          icon_path=IMAGE_PATH,
                          auto_open=True,
                          ),
                 CSurfaceDarrayTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,           
                          ),
                 CDataTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 CScriptTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 CVolumeTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 CTimeseriesTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 CImagestackTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 CTrackTreeNode(
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 ]
        
        return nodes

    def _tree_editor_default(self):
        return TreeEditor(editable=False,
                                 hide_root=False,
                                 on_dclick='handler._on_dclick',
                                 orientation='vertical',
                                 nodes=self.nodes
                        )
    