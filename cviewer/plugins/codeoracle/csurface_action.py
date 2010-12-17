
from enthought.traits.api import Code, Button, Int, on_trait_change, Any, HasTraits,List, Str, Enum, Instance
from enthought.traits.ui.api import (View, Item, Group, HGroup, CodeEditor,
                                     spring, Handler, EnumEditor)

from cviewer.plugins.cff2.csurface_darray import CSurfaceDarray
from nibabel.gifti.util import intent_codes


class CloseHandler(Handler):
    """This class cleans up after the UI for the recorder is closed."""

    def close(self, info, is_ok):
        """This method is invoked when the user closes the UI."""
        recorder = info.object
        recorder.on_ui_close()
        return True
    
class SurfaceParameter(HasTraits):
    
    engine = Enum("Mayavi", ["Mayavi", "fos.me"])
    
#    pointset = List(CSurfaceDarray)
#    faces = List(CSurfaceDarray)
    
    view = View(
             Item('engine', label = "Use Engine"),
             Item('pointset', label = "Pointset"),
             Item('faces', label = "Faces"),
             id='cviewer.plugins.codeoracle.surfaceparameter',
             buttons=['OK', 'Cancel'], 
             resizable=True,
             handler=CloseHandler(),
             title = "Create surface ..."
             )
    
    def __init__(self, cfile, **traits):
        super(SurfaceParameter, self).__init__(**traits)
        
        pointset = []
        faces = []
        
        for cobj in cfile.connectome_surface:
            if cobj.loaded:
                # check darrays
                # if correct intent, add to list
                for cdobj in cobj.darrays:
                     if cdobj.data.intent == 1008:
                        pointset.append(cobj.name + ' / ' + cdobj.dname)
                        #pointset.append(cdobj)
                     if cdobj.data.intent == 1009:
                        faces.append(cobj.name + ' / ' + cdobj.dname)
                        #faces.append(cdobj.dname)
                        
        
        if len(pointset) == 0:
            pointset = ["None"]
            
        if len(faces) == 0:
            faces = ["None"]
        
        print "piontset",pointset
        
        self.add_trait('pointset', Enum(pointset) )
        self.add_trait('faces', Enum(faces) )
        
        self.pointset = pointset[0]
        self.faces = faces[0]
                        
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

        