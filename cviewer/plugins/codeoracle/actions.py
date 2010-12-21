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

class ShowSurfaces(Action):
    """ Open a new file in the text editor.
    """
    tooltip = "Create a new file for editing"
    description = "Create a new file for editing"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        event.print_traits()
        
        from csurface_action import SurfaceParameter
        from scripts import surfscript, surfscript_nola
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
                
        so = SurfaceParameter(cfile)
        # can not be modal if we have add_trait methods
        #so.configure_traits(kind='modal')
        #so.edit_traits(kind='modal')
        so.edit_traits(kind='livemodal')
        
        if not so.pointset_da[so.pointset] is None:
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            if so.labels_da[so.labels] is None:
                f.write(surfscript_nola % (so.pointset_da[so.pointset]['name'], \
                                      so.pointset_da[so.pointset]['da_idx'], \
                                      so.faces_da[so.faces]['name'], \
                                      so.faces_da[so.faces]['da_idx'] ) )
            else:
                f.write(surfscript % (so.pointset_da[so.pointset]['name'],
                                      so.pointset_da[so.pointset]['da_idx'],
                                      so.faces_da[so.faces]['name'], 
                                      so.faces_da[so.faces]['da_idx'],
                                      so.labels_da[so.labels]['name'],
                                      so.labels_da[so.labels]['da_idx']))
            f.close()
            
            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)
                