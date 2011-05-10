""" Specify the NetworkNode with its action, context-menus """
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
from cviewer.plugins.cff.network import Network

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class NetworkTreeNode(TreeNode):
    
    # The object that contains the container ;^)
    parent = Any

    # the network associated with this node
    node_for=[Network]

    # a default icons
    # Name of group item icon
    icon_group = Str('home.png')
    # Name of leaf item icon
    icon_item=Str('home.png')
    # Name of opened group item icon
    icon_open=Str('home.png')
    
    # labels
    label='name'

    ###
    # Private Traits
    
    # activate / deactivate logic
    # if the node is activated, this means that there exists a
    # corresponding RenderManager instance
    
    _ActivateAction = Instance(Action,  
                               kw={'name': 'Activate / Deactivate', 
                                   'action': 'object._de_activate',
                                   'tooltip': 'Activate or Deactive the Network'}, )
    
    _ChangeParameters = Instance(Action,
                                 kw={'name': 'Edge Parameters',
                                     'action': 'object._edge_parameters',
                                     'tooltip': 'Thresholding and Change Attributes',
                                     'enabled_when' : 'object.active == True'}, )
    
    _RenderMatrixAction = Instance(Action,
                               kw={'name': 'Matrix Viewer', 
                                   'action': 'object._render_matrix',
                                    'tooltip':'View the connectivity matrices',
                                    'enabled_when':'object.active == True'}, )

    _SelectAllNodes = Instance(Action,
                               kw={'name': 'Select All Nodes and Edges', 
                                   'action': 'object.select_all',
                                    'tooltip':'Select all nodes of the current network',
                                    'enabled_when':'object.active == True and not object.rendermanager.nodes is None'}, )
    
    _UnSelectAllNodes = Instance(Action,
                               kw={'name': 'Unselect All Nodes and Edges', 
                                   'action': 'object.unselect_all',
                                    'tooltip':'Unselect all nodes of the current network',
                                    'enabled_when':'object.active == True and not object.rendermanager.nodes is None'}, )

    _ToggleSurfaceVisibility = Instance(Action,
                               kw={'name': 'Toggle Surface Visibility', 
                                   'action': 'object.toggle_surface',
                                    'tooltip':'Toggle the surface visibility for all selected nodes',
                                    'enabled_when':'object.active == True and not object.rendermanager.surfsrc is None'}, )

    _TrackVisLaunchAction = Instance(Action,
                               kw={'name': 'TrackVis On Selection', 
                                   'action': 'object._trackvis_launch',
                                    'tooltip':'Launch Trackvis on selected nodes',
                                    'enabled_when':'object.active == True and len(object.tracks) != 0'}, )

    # the menu shown after right-click
    menu = Instance(Menu, transient=True)
    
    def get_children(self, object):
        """ Get the object's children. """

        # Collate the window's views into categories.
        return object.surfaces + object.volumes + object.tracks
        
    ######################################################################
    # Non-public interface
    ######################################################################

    def _menu_default(self):
        """ Standard menus for network nodes """
        
        menu_actions = [Separator(), \
                        self._SelectAllNodes, self._UnSelectAllNodes, self._ChangeParameters, \
                        Separator(), \
                        self._ToggleSurfaceVisibility, Separator(), \
                        self._RenderMatrixAction, self._TrackVisLaunchAction, \
                        Separator(), self._ActivateAction]
        
        return Menu( *menu_actions)
        
