""" The Connectome File View as a Tree Structure """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library import
import os, os.path

# Enthought library imports
from enthought.traits.api import Instance, HasTraits, Any, Delegate, List, Either
from enthought.pyface.image_cache import ImageCache
from enthought.traits.ui.api import (Item, TreeEditor, TreeNode, View, Handler, UIInfo)
from enthought.pyface.image_resource import ImageResource
from enthought.pyface.api import FileDialog, OK
from enthought.pyface.action.api import Action

# ConnectomeViewer imports
from cviewer.plugins.cff.surface import Surface
from cviewer.plugins.cff.surface_container import SurfaceContainer
from cviewer.plugins.cff.cfile import CFile
from cviewer.plugins.cff.network import Network
from cviewer.plugins.cff.volume import Volume
from cviewer.plugins.cff.trackfile import Trackfile
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.plugins.cff.ui.network_tree_node import NetworkTreeNode
from cviewer.plugins.cff.ui.surface_tree_node import SurfaceTreeNode
from cviewer.plugins.cff.ui.volume_tree_node import VolumeTreeNode
from cviewer.plugins.cff.ui.trackfile_tree_node import TrackfileTreeNode
from cviewer.action.common import IMAGE_PATH

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)


class CFFViewHandler(Handler):
    """ A handler for the CFFView object """

    def _on_dclick(self, object):
        """ Called when a node in the tree  is double-clicked.
        """
        if isinstance(object, Surface):
            
            # security test if network already activated
            if object._container._networkref.active:
                # quite hackish
                tmpwin = object._container._networkref._parentcfile._workbenchwin
                tmpwin.status_bar_manager.message='Rendering 3D View ...'
                object._layout_3DView()
                
        elif isinstance(object, Network):
            if not object.active:
                tmpwin = object._parentcfile._workbenchwin
                tmpwin.status_bar_manager.message='Activating network ...'
                object._de_activate()
            else:
                object._de_activate()
            
        elif isinstance(object, CFile):
            if not object.metadata is None:
                object.metadata.edit_traits()
            else:
                # otherwise open the dialog to load a cfile
                from cviewer.action.load_cff import OpenCFile
                actio = OpenCFile()
                actio.perform(event=None, cfile=object)
        else:
            pass

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
                    id='cviewer.plugins.cff.cfile',
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
        nodes = [TreeNode(node_for=[CFile],
                          children='networks',
                          icon_path=IMAGE_PATH,
                          icon_item='cff.png',
                          icon_open='cff.png',
                          icon_group='cff.png',
                          label='name',
                          auto_open=True,
                          copy=False,
                          delete=False,
                          rename=False,
                          ),
                 NetworkTreeNode(
                          children='surfaces',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          ),
                 TrackfileTreeNode(node_for=[Trackfile],
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          copy=False,
                          delete=False,
                          rename=False,
                          ),
                 VolumeTreeNode(node_for=[Volume],
                          children='',
                          icon_path=IMAGE_PATH,
                          auto_open=False,
                          copy=False,
                          delete=False,
                          rename=False,
                          ),
                 TreeNode(node_for=[SurfaceContainer],
                          children='surfaces',
                          label='name',
                          icon_path=IMAGE_PATH,
                          icon_item='surfacecontainer.png',
                          icon_open='surfacecontainer.png',
                          icon_group='surfacecontainer.png',
                          auto_open=True,
                          copy=False,
                          delete=False,
                          rename=False,
                          ),
                 SurfaceTreeNode(node_for=[Surface],
                          children='',
                          label='secondary',
                          icon_path=IMAGE_PATH,
                          #icon_item='surface.png',
                          #icon_open='surface.png',
                          auto_open=True,
                          copy=False,
                          delete=False,
                          rename=False,
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
    