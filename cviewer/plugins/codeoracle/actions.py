import logging

from enthought.io.api import File
from enthought.pyface.api import FileDialog, OK
from enthought.pyface.action.api import Action
from enthought.traits.api import Any

from cviewer.plugins.text_editor.editor.text_editor import TextEditor
from cviewer.plugins.ui.preference_manager import preference_manager

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class DoAction(Action):
    """ Open a new file in the text editor.
    """
    tooltip = "Create a new file for editing"
    description = "Create a new file for editing"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        event.print_traits()
        
        from csurface_action import SurfaceParameter
        
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
                
        so = SurfaceParameter(cfile)
        so.edit_traits(kind='modal')
        
        # create a temporary file
        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf,'w')
        f.write("""from enthought.mayavi import mlab
print "%s"
""" % so.engine)
        f.close()   
        
        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)
        