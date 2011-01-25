
from enthought.traits.api import Code, Button, Int, on_trait_change, Any, HasTraits,List, Str, Enum, Instance, Bool
from enthought.traits.ui.api import (View, Item, Group, HGroup, CodeEditor,
                                     spring, Handler, EnumEditor)

from cviewer.plugins.cff2.cvolume import CVolume

class VolumeParameter(HasTraits):
    
    engine = Enum("Mayavi", ["Mayavi"])
    
    view = View(
             Item('engine', label = "Use Engine"),
             Item('myvolume', label = "Volume"),
             id='cviewer.plugins.codeoracle.volumeparameter',
             buttons=['OK'], 
             resizable=True,
             title = "Create volume ..."
             )
    
    def __init__(self, cfile, **traits):
        super(VolumeParameter, self).__init__(**traits)
        
        self.volumes = {}
        
        for cobj in cfile.connectome_volume:
            if cobj.loaded:
                if isinstance(cobj, CVolume):
                    self.volumes[cobj.name] = {'name' : cobj.obj.name}
                        
        if len(self.volumes) == 0:
            self.volumes["None"] = {'name' : "None"}
            
        self.add_trait('myvolume',  Enum(self.volumes.keys()) )
        
