""" The ConnectomeViewer User Interface Preferences """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

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

    # Use IPython in Shell
    useipython = Bool(desc='if to use IPython as default Shell')
    
    # Default path to load cff files
    cffpath = Directory(desc='the default path to open Connectome files from')
    
    # default path for scripts
    scriptpath = Directory(desc='the default path to find executable Python scripts')

    # show the ConnectomeViewer splash screen
    show_splash_screen = Bool(desc='if the Connectome Viewer splashscreen is shown on startup')
    
    ######################################################################
    # Traits UI view.

    trait_view = View(Group(
                            Item('show_splash_screen', label='Show Splash Screen:'),
                            Item('useipython', label='Use IPython:'),
                            Item('cffpath', label='Connectome File Path:'),
                            Item('scriptpath', label='Python Script Path:'),
                           ),
                      resizable=True
                     )
    