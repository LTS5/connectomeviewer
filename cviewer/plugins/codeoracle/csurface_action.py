
from enthought.traits.api import Code, Button, Int, on_trait_change, Any, HasTraits,List, Str, Enum, Instance, Bool
from enthought.traits.ui.api import (View, Item, Group, HGroup, CodeEditor,
                                     spring, Handler, EnumEditor)

from cviewer.plugins.cff2.csurface_darray import CSurfaceDarray
from nibabel.gifti.util import intent_codes


class CloseHandler(Handler):
    """This class cleans up after the UI for the recorder is closed."""

    def close(self, info, is_ok):
        """This method is invoked when the user closes the UI."""
        
        # recorder = info.object
        # recorder.on_ui_close()
        if is_ok:
            info.object.isok = True
        else:
            info.object.isok = False
            
        return True
    
class SurfaceParameter(HasTraits):
    
    engine = Enum("Mayavi", ["Mayavi"])
    isok = Bool(False)

#    pointset = List(CSurfaceDarray)
#    faces = List(CSurfaceDarray)
    
    view = View(
             Item('engine', label = "Use Engine"),
             Item('pointset', label = "Pointset"),
             Item('faces', label = "Faces"),
             Item('labels', label="labels"),
             id='cviewer.plugins.codeoracle.surfaceparameter',
             buttons=['OK'], 
             resizable=True,
             handler=CloseHandler(),
             title = "Create surface ..."
             )
    
    def __init__(self, cfile, **traits):
        super(SurfaceParameter, self).__init__(**traits)
        
        self.pointset_da = {}
        self.faces_da = {}
        self.labels_da = {}
        
        for cobj in cfile.connectome_surface:
            if cobj.loaded:
                # check darrays
                # if correct intent, add to list
                for i, cdobj in enumerate(cobj.darrays):
                     if cdobj.data.intent == 1008:
                        self.pointset_da[cobj.name + ' / ' + cdobj.dname + ' (%s)' % str(i)] = {'name' : cobj.obj.name,
                                                                             'da_idx' : i}
                        #pointset.append(cdobj)
                     if cdobj.data.intent == 1009:
                        self.faces_da[cobj.name + ' / ' + cdobj.dname + ' (%s)' % str(i)] = {'name' : cobj.obj.name,
                                                                             'da_idx' : i}
                        #faces.append(cdobj.dname)
                     if cdobj.data.intent == 1002:
                        self.labels_da[cobj.name + ' / ' + cdobj.dname + ' (%s)' % str(i)] = {'name' : cobj.obj.name,
                                                                             'da_idx' : i}
                        
        
        if len(self.pointset_da) == 0:
            self.pointset_da["None"] = None
            
        if len(self.faces_da) == 0:
            self.faces_da["None"] = None
        
        if len(self.labels_da) == 0:
            self.labels_da["None"] = None
        
        # assert labels and pointset dimension are the same
        
        self.add_trait('pointset',  Enum(self.pointset_da.keys()) )
        self.add_trait('faces', Enum(self.faces_da.keys()) )
        self.add_trait('labels', Enum(self.labels_da.keys()) )
        
                        
    def on_ui_close(self):
        """Called from the CloseHandler when the UI is closed. This
        method basically stops the recording.
        """
        # show code
#        from oracle import Oracle
##        a = Oracle()
##        a.code = "from enthought.mayavi import mlab\nmlab.test_surf()"
##        a.edit_traits()
#        
#        # create a temporary file
#        import tempfile
#        myf = tempfile.mktemp(suffix='.py', prefix='my')
#        f=open(myf,'w')
#        f.write("""import numpy
#        """)
#        f.close()

        