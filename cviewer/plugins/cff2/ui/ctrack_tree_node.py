""" Specify the TreeNode object with its action, context-menus """
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
from cviewer.plugins.cff2.ctrack import CTrack

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CTrackTreeNode(TreeNode):
    
    # The object that contains the container ;^)
    parent = Any

    # the network associated with this node
    node_for=[CTrack]

    # a default icons
    # Name of group item icon
    icon_group = Str('trackvis.png')
    # Name of leaf item icon
    icon_item=Str('trackvis.png')
    # Name of opened group item icon
    icon_open=Str('trackvis.png')
    
    # labels
    label='dname'

    ###
    # Private Traits
    
    # activate / deactivate logic
    # if the node is activated, this means that there exists a
    # corresponding RenderManager instance
    
    _TracksRender = Instance(Action,
                               kw={'name': 'Show Tracks (fos)', 
                                   'action': 'object.render_tracks',
                                    'tooltip':'Show the rendered tracks',
                                    'enabled_when':'object.loaded == True'
                                    }, )
    
    _TrackVisLaunchAction = Instance(Action,
                               kw={'name': 'Launch TrackVis', 
                                   'action': 'object.launch_trackvis',
                                    'tooltip':'Launch TrackVis',
                                    'enabled_when':'object.loaded == True'}, )
    
    # the menu shown after right-click
    menu = Instance(Menu, transient=True)
    
    def get_children(self, object):
        """ Get the object's children. """
        pass
        
    ######################################################################
    # Non-public interface
    ######################################################################

    def _menu_default(self):
        """ Standard menus for network nodes """
        
        menu_actions = [Separator(), \
                        self._TracksRender, \
                        self._TrackVisLaunchAction]
        
        return Menu( *menu_actions)
        
