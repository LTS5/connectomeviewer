""" The preferences for the ConnectomeViewer UI Plugin. """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from apptools.preferences.ui.api import PreferencesPage
from traits.api import Bool, Directory
from traitsui.api import View, Group, Item

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

    # Use IPython in Shell
    useipython = Bool(desc='if to use IPython as default Shell')
    
    # Default path to load cff files
    cffpath = Directory(desc='the default path to open Connectome files from')
    
    # default path for scripts
    scriptpath = Directory(desc='the default path to find executable Python scripts')

    # show the ConnectomeViewer splash screen
    show_splash_screen = Bool(desc='if the Connectome Viewer splashscreen is shown on startup')
    
    #### Traits UI views ######################################################
    trait_view = View(Group(
                            Item('show_splash_screen', label='Show Splash Screen:'),
                            Item('useipython', label='Use IPython:'),
                            Item('cffpath', label='Connectome File Path:'),
                            Item('scriptpath', label='Python Script Path:'),
                           ),
                      resizable=True
                     )

#### EOF ######################################################################
