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

class ShowNetworks(Action):
    tooltip = "Create a surface"
    description = "Create a surface"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from cnetwork_action import NetworkParameter
        from scripts import netscript
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        
        no = NetworkParameter(cfile)
        no.edit_traits(kind='livemodal')

        if not no.netw[no.graph]['name'] == "None":
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            f.write(netscript % (no.netw[no.graph]['name'],
                                  no.node_position,
                                  no.edge_value,
                                  no.node_label))
            f.close()
            
            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)


class ShowSurfaces(Action):
    """ Open a new file in the text editor
    """
    tooltip = "Create a surface"
    description = "Create a surface"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from csurface_action import SurfaceParameter
        from scripts import surfscript
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
                
        so = SurfaceParameter(cfile)
        so.edit_traits(kind='livemodal')
        
        if not so.pointset_da[so.pointset]['name'] == "None":
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            f.write(surfscript % (so.pointset_da[so.pointset]['name'],
                                  so.pointset_da[so.pointset]['da_idx'],
                                  so.faces_da[so.faces]['name'], 
                                  so.faces_da[so.faces]['da_idx'],
                                  so.labels_da[so.labels]['name'],
                                  so.labels_da[so.labels]['da_idx']))
            f.close()
            
            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

