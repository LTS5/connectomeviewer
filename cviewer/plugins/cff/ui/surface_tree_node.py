""" Specify the SurfaceNode with its action, context-menus """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os

# Enthought library imports
from enthought.traits.api import Instance, Str, Any
from enthought.traits.ui.api import TreeNode
from enthought.traits.ui.menu import Menu, Action, Separator

# ConnectomeViewer imports
from cviewer.plugins.cff.surface import Surface

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class SurfaceTreeNode(TreeNode):
    
    # The object that contains the container ;^)
    parent = Any

    # the network associated with this node
    node_for=[Surface]

    # a default icons
    # Name of group item icon
    icon_group = Str('surface.png')
    # Name of leaf item icon
    icon_item=Str('surface.png')
    # Name of opened group item icon
    icon_open=Str('surface.png')
    
    # labels
    label='secondary'

    ###
    # Private Traits
    
    _SurfaceRender = Instance(Action,
                               kw={'name': 'Render network using this surface for layout.', 
                                   'action': 'object._layout_3DView',
                                    'tooltip':'Render the network using this surface',
                                    'enabled_when':'object.active == True'}, )

    # the menu shown after right-click
    menu = Instance(Menu, transient=True)
    
    ######################################################################
    # Non-public interface
    ######################################################################

    def _menu_default(self):
        """ Standard menus for network nodes """
        
        menu_actions = [Separator(), self._SurfaceRender, Separator(), ]
        
        return Menu( *menu_actions)
        
