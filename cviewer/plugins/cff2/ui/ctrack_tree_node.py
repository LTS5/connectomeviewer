""" Specify the TreeNode object with its action, context-menus """
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
        
