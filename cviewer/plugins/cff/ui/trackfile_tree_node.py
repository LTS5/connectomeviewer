""" Specify the TrackfileNode with its action, context-menus """
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
from cviewer.plugins.cff.trackfile import Trackfile

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class TrackfileTreeNode(TreeNode):
    
    # The object that contains the container ;^)
    parent = Any

    # the trackfile associated with this node
    node_for=[Trackfile]

    # a default icons
    # Name of group item icon
    icon_group = Str('trackvis.png')
    # Name of leaf item icon
    icon_item=Str('trackvis.png')
    # Name of opened group item icon
    icon_open=Str('trackvis.png')
    
    # labels
    label='trkname'

    ###
    # Private Traits
    
    _TracksRender = Instance(Action,
                               kw={'name': 'Show Tracks', 
                                   'action': 'object.render_tracks',
                                    'tooltip':'Show the rendered tracks',
                                    }, )

    # the menu shown after right-click
    menu = Instance(Menu, transient=True)
    
    ######################################################################
    # Non-public interface
    ######################################################################

    def _menu_default(self):
        """ Standard menus for network nodes """
        
        menu_actions = [Separator(), self._TracksRender, \
                        Separator(), ]
        
        return Menu( *menu_actions)
        
