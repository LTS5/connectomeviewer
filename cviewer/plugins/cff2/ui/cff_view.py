""" The Connectome File View as a Tree Structure """
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
from cviewer.plugins.cff2.cnetwork import CNetwork
from cviewer.plugins.cff2.cfile import CFile
from cnetwork_tree_node import CNetworkTreeNode

from cviewer.action.common import IMAGE_PATH

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CFFViewHandler(Handler):
    """ A handler for the CFFView object """

    def _on_dclick(self, object):
        """ Called when a node in the tree  is double-clicked.
        """
                
        if isinstance(object, CNetwork):
            logger.info("Network doubleclicked")
        else:
            pass

##############################################################################
# CFFView class.
##############################################################################
class CFF2View(HasTraits):
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
                    id='cviewer.plugins.cff2.cfile',
                    help=False,
                    resizable=True,
                    scrollable=True,
                    undo=False,
                    revert=False,
                    ok=False,
                    cancel=False,
                    icon=self.icon,
                    title = 'Connectome File View 2',
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
                          )
                 ]
        
        return nodes

    def _tree_editor_default(self):
        return TreeEditor(editable=False,
                                 hide_root=False,
                                 on_dclick='handler._on_dclick',
                                 orientation='vertical',
                                 nodes=self.nodes
                        )
    