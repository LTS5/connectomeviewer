"""
A Recorder subclass that presents a simple user interface.
"""
# Author: Prabhu Ramachandran <prabhu@aero.iitb.ac.in>
# Copyright (c) 2008, Prabhu Ramachandran.
# License: BSD Style.

from enthought.traits.api import Code, Button, Int, on_trait_change, Any, HasTraits,List,Str
from enthought.traits.ui.api import (View, Item, Group, HGroup, CodeEditor,
                                     spring, Handler)

#from recorder import Recorder

######################################################################
# `CloseHandler` class.
######################################################################
class CloseHandler(Handler):
    """This class cleans up after the UI for the recorder is closed."""

    def close(self, info, is_ok):
        """This method is invoked when the user closes the UI."""
        recorder = info.object
        recorder.on_ui_close()
        return True


################################################################################
# `RecorderWithUI` class.
################################################################################ 
class Oracle(HasTraits):
    """
    This class represents a Recorder but with a simple user interface.
    """

    # The lines of code recorded.
    lines = List(Str)
    
    # The code to display
    code = Code(editor=CodeEditor(line='current_line'))

    # Button to save script to file.
    run_script = Button('Run Script')
    save_script = Button('Save Script')

    # The current line to show, used by the editor.
    current_line = Int

    # The root object which is being recorded.
    root = Any

    ########################################
    # Traits View.
    view = View(
             Group(
                HGroup(spring,
                       Item('save_script', show_label=False),
                       Item('run_script', show_label=False),
                ),
                Group(Item('code', show_label=False)),
                ),
             width=800, 
             height=460,
             id='cviewer.plugins.codeoracle',
             buttons=['Cancel'], 
             resizable=True,
             handler=CloseHandler(),
             title = "ConnectomeViewer Code Oracle"
             )

    ######################################################################
    # RecorderWithUI interface.
    ######################################################################
    def on_ui_close(self):
        """Called from the CloseHandler when the UI is closed. This
        method basically stops the recording.
        """
        pass

    def get_code(self):
        """Returns the recorded lines as a string of printable code."""
        return '\n'.join(self.lines) + '\n'
    
    ######################################################################
    # Non-public interface.
    ######################################################################
    @on_trait_change('lines[]')
    def _update_code(self):
        self.code = self.get_code()
        self.current_line = len(self.lines) + 1

    def save(self, file):
        """Save the recorded lines to the given file.  It does not close
        the file.
        """
        file.write(self.get_code())
        file.flush()
        
    def ui_save(self):
        """Save recording to file, pop up a UI dialog to find out where
        and close the file when done.
        """
        from enthought.pyface.api import FileDialog, OK
        wildcard = 'Python files (*.py)|*.py|' + FileDialog.WILDCARD_ALL
        dialog = FileDialog(title='Save Script',
                            action='save as', wildcard=wildcard
                            )
        if dialog.open() == OK:
            fname = dialog.path
            f = open(fname, 'w')
            self.save(f)
            f.close()
            
    def _save_script_fired(self):
        self.ui_save()

    def _run_script_fired(self):
        exec(self.get_code())


if __name__ == '__main__':
    a = Oracle()
    a.lines.append("import pyconto\nprint 'me'")
    a.configure_traits()
