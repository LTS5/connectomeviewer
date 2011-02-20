"""Actions for the help menu """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.pyface.action.api import Action
from enthought.traits.ui.api import auto_close_message
from enthought.mayavi.preferences.api import preference_manager
from enthought.pyface.image_resource import ImageResource

# ConnectomeViewer imports
from common import IMAGE_PATH

def browser_open(url, decorated = False):
    import os
    import sys
    if sys.platform == 'darwin':
            os.system('open %s &' % url)
    else:
        import webbrowser
        if webbrowser._iscommand('firefox') and \
                        preference_manager.root.open_help_in_light_browser:
            # Firefox is installed, let's use it, we know how to make it
            # chromeless.
            if decorated:
                webbrowser.open(url, autoraise=1)
            else:
                firefox = webbrowser.get('firefox')
                firefox._invoke(['-chrome', url], remote=False, autoraise=True)
        else:
            webbrowser.open(url, autoraise=1)


class Bugfix(Action):
    """ An action that pop up the bugfix GitHub page in a browser. """

    name = "Bugfixes"
    tooltip       = "Bug Fixes ..."
    description   = "Bug Fixes ..."
    image = ImageResource("bug.png", search_path=[IMAGE_PATH])

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event):
        """ Performs the action. """
        browser_open(url='http://github.com/unidesigner/connectomeviewer/issues', decorated = True)

class Keybindings(Action):
    """ An action that creates a temporary html file to show the key bindings.. """

    name = "Key Bindings"
    tooltip       = "Show Key Bindings in Browser"
    description   = "Key Bindings"
    image = ImageResource("keyboard.png", search_path=[IMAGE_PATH])

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event):
        """ Performs the action. """
        import os.path
        browser_open(url=os.path.join(IMAGE_PATH, '..', 'keybindings', 'index.html'), decorated = True)


######################################################################
# `HelpIndex` class.
######################################################################
class HelpIndex(Action):
    """ An action that pop up the help in a browser. """

    name = "Help"
    tooltip       = "The ConnectomeViewer User Guide"
    description   = "The ConnectomeViewer User Guide"
    image = ImageResource("help-browser.png", search_path=[IMAGE_PATH])

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event):
        """ Performs the action. """
        auto_close_message("Opening help in web browser...")
        browser_open(url='http://www.connectomeviewer.org/documentation', decorated = True)
