import logging

from enthought.io.api import File
from enthought.pyface.api import FileDialog, OK
from enthought.pyface.action.api import Action
from enthought.traits.api import Any

from cviewer.plugins.text_editor.editor.text_editor import TextEditor

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class OpenFile(Action):
    """ Open a file in the text editor
    """
    tooltip = "Create a surface"
    description = "Create a surface"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, file, event=None):
        print "event", event
        print "app", self.application
        print "window", self.window