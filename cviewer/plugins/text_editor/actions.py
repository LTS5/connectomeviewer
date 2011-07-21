import logging

from apptools.io.api import File
from pyface.api import FileDialog, OK
from pyface.action.api import Action
from traits.api import Any

from editor.text_editor import TextEditor
from cviewer.plugins.ui.preference_manager import preference_manager

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class NewFileAction(Action):
    """ Open a new file in the text editor.
    """
    tooltip = "Create a new file for editing"
    description = "Create a new file for editing"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        logger.info('NewFileAction.perform()')
        self.window.workbench.edit(File(''), kind=TextEditor,
            use_existing=False)


class OpenFileAction(Action):
    """ Open an existing file in the text editor.
    """
    tooltip = "Open a file for editing"
    description = "Open a file for editing"

    def perform(self, event=None):
        logger.info('OpenFileAction.perform()')
        pref_script_path = preference_manager.cviewerui.scriptpath
        dialog = FileDialog(parent=self.window.control,
            title='Open File',
            default_directory=pref_script_path)
        if dialog.open() == OK:
            self.window.workbench.edit(File(dialog.path), kind=TextEditor)

            