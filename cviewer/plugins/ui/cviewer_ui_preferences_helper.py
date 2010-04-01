""" The ConnectomeViewer User Interface Preferences """
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

# Enthought library imports
from enthought.preferences.api import PreferencesHelper
from enthought.traits.api import Bool, Directory
from enthought.traits.ui.api import View, Group, Item

class CViewerUIPreferencesHelper(PreferencesHelper):
    """ Helper for the Connectome Viewer UI preferences """
    
    # The preferences path for which we use.
    preferences_path = 'cviewer.plugins.ui'

    # actually, the same as in cviewer_preferences_page.py
    
   #### Preferences ##########################################################

    # Load labels automatically
    labelload = Bool(desc='if to load and show all node labels in the 3D View')

    # Use IPython in Shell
    useipython = Bool(desc='if to use IPython as default Shell')
    
    # Default path to load cff files
    cffpath = Directory(desc='the default path to open Connectome files from')
    
    # default path for scripts
    scriptpath = Directory(desc='the default path to find executable Python scripts')

    # Path to Trackvis
    trackvispath = Directory(desc='the path where the TrackVis executables reside')

    # show the ConnectomeViewer splash screen
    show_splash_screen = Bool(desc='if the ConnectomeViewer splashscreen is shown on startup')
    
    ######################################################################
    # Traits UI view.

    trait_view = View(Group(
                            Item('show_splash_screen', label='Show Splash Screen:'),
                            Item('labelload', label='Load All Node Labels:'),
                            Item('useipython', label='Use IPython:'),
                            Item('cffpath', label='Connectome File Path:'),
                            Item('scriptpath', label='Python Script Path:'),
                            Item('trackvispath', label='TrackVis Executable Path:')
                           ),
                      resizable=True
                     )
    