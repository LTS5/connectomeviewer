""" Specify the CFile Treenode with its action, context-menus """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os

# Enthought library imports
from traits.api import Instance, Str, Any
from traitsui.api import TreeNode
from traitsui.menu import Menu, Action, Separator

# ConnectomeViewer imports
from cviewer.plugins.cff2.cfile import CFile

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CFileTreeNode(TreeNode):
    
    # The object that contains the container ;^)
    parent = Any

    # the network associated with this node
    node_for=[CFile]

    # a default icons
    # Name of group item icon
    icon_group = Str('cff.png')
    # Name of leaf item icon
    icon_item=Str('cff.png')
    # Name of opened group item icon
    icon_open=Str('cff.png')
    
    # labels
    label='dname'

    auto_open=True
    copy=False
    delete=False
    rename=False

    ###
    # Private Traits
    
    # activate / deactivate logic
    # if the node is activated, this means that there exists a
    # corresponding RenderManager instance
    
    _ShowName = Instance(Action,  
                               kw={'name': 'Show name', 
                                   'action': 'object.show_name',
                                   'tooltip': 'Shows the network name'}, )
    
    _ShowSurface = Instance(Action,  
                               kw={'name': 'Show surface', 
                                   'action': 'object.show_surface',
                                   'tooltip': 'Creates a surface to inspect'}, )
    
    
    # the menu shown after right-click
    menu = Instance(Menu, transient=True)
    
    def get_children(self, object):
        """ Get the object's children. """
        # Collate the window's views into categories.
        return object.children
    
        
    ######################################################################
    # Non-public interface
    ######################################################################

    def _menu_default(self):
        """ Standard menus for network nodes """
        
        menu_actions = [Separator()]
        return Menu( *menu_actions)
        
