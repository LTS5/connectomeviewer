""" The preferences for the ConnectomeViewer UI Plugin. """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.preferences.ui.api import PreferencesPage
from enthought.traits.api import Bool, Directory
from enthought.traits.ui.api import View, Group, Item

class CViewerUIPreferencesPage(PreferencesPage):
    """ The preferences page for the Connectome Viewer UI Plugin. """

    #### 'PreferencesPage' interface ##########################################

    # The page's category (e.g. 'General/Appearance'). The empty string means
    # that this is a top-level page.
    category = ''

    # The page name (this is what is shown in the preferences dialog.
    name = 'Connectome Viewer'

    # The path to the preference node that contains the preferences.
    preferences_path = 'cviewer.plugins.ui'

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
    show_splash_screen = Bool(desc='if the Connectome Viewer splashscreen is shown on startup')
    
    #### Traits UI views ######################################################
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

#### EOF ######################################################################
