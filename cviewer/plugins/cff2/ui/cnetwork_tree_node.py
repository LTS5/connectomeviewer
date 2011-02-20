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
from cviewer.plugins.cff2.cnetwork import CNetwork

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CNetworkTreeNode(TreeNode):
    
    # The object that contains the container ;^)
    parent = Any

    # the network associated with this node
    node_for=[CNetwork]

    # a default icons
    # Name of group item icon
    icon_group = Str('home.png')
    # Name of leaf item icon
    icon_item=Str('home.png')
    # Name of opened group item icon
    icon_open=Str('home.png')
    
    # labels
    label='dname'

    ###
    # Private Traits
    
    # activate / deactivate logic
    # if the node is activated, this means that there exists a
    # corresponding RenderManager instance
    
    _ShowName = Instance(Action,  
                               kw={'name': 'Show name', 
                                   'action': 'object.show_name',
                                   'tooltip': 'Shows the network name'}, )
    
    _ChangeParameters = Instance(Action,
                                 kw={'name': 'Edge Parameters',
                                     'action': 'object._edge_parameters',
                                     'tooltip': 'Thresholding and Change Attributes',
                                     'enabled_when' : 'object.loaded == True'}, )
    
    _RenderMatrixAction = Instance(Action,
                               kw={'name': 'Connectome Matrix Viewer', 
                                   'action': 'object.invoke_matrix_viewer',
                                    'tooltip':'View the connectivity matrices',
                                    'enabled_when':'object.loaded == True'}, )
    
    # the menu shown after right-click
    menu = Instance(Menu, transient=True)
    
    def get_children(self, object):
        """ Get the object's children. """
        pass
        # Collate the window's views into categories.
        #return object.surfaces + object.volumes + object.tracks
        
    ######################################################################
    # Non-public interface
    ######################################################################

    def _menu_default(self):
        """ Standard menus for network nodes """
        
        menu_actions = [Separator(), \
                        self._RenderMatrixAction]
        
        return Menu( *menu_actions)
        
