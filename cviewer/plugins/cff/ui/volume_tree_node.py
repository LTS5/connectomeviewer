""" Specify the VolumeNode with its action, context-menus """
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

# Standard library imports
import os

# Enthought library imports
from enthought.traits.api import Instance, Str, Any
from enthought.traits.ui.api import TreeNode
from enthought.traits.ui.menu import Menu, Action, Separator

# ConnectomeViewer imports
from cviewer.plugins.cff.volume import Volume

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class VolumeTreeNode(TreeNode):
    
    # The object that contains the container ;^)
    parent = Any

    # the network associated with this node
    node_for=[Volume]

    # a default icons
    # Name of group item icon
    icon_group = Str('volume.png')
    # Name of leaf item icon
    icon_item=Str('volume.png')
    # Name of opened group item icon
    icon_open=Str('volume.png')
    
    # labels
    label='volumename'

    ###
    # Private Traits
    
    _VolumeRender = Instance(Action,
                               kw={'name': 'Volume Isosurface', 
                                   'action': 'object.render_vol',
                                    'tooltip':'Renders volume in the current window.',
                                    'enabled_when':'object._networkref.active == True'}, )

    _VolumeVisualizer = Instance(Action,
                               kw={'name': 'Volume Slicer', 
                                   'action': 'object.vol_vis',
                                    'tooltip':'Invokes a simple Volume Slicer by Gael Varoquaux.',
                                    #'enabled_when':'object._networkref.active == True'
                                    },
                               )

    # the menu shown after right-click
    menu = Instance(Menu, transient=True)
    
    ######################################################################
    # Non-public interface
    ######################################################################

    def _menu_default(self):
        """ Standard menus for network nodes """
        
        menu_actions = [Separator(), self._VolumeVisualizer, \
                        Separator(), ]
        
        return Menu( *menu_actions)
        
