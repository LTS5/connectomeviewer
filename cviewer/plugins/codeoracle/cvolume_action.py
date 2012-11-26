
from traits.api import Code, Button, Int, on_trait_change, Any, HasTraits,List, Str, Enum, Instance, Bool
from traitsui.api import (View, Item, Group, HGroup, CodeEditor,
                                     spring, Handler, EnumEditor)

from cviewer.plugins.cff2.cvolume import CVolume

class VolumeParameter(HasTraits):
       
    view = View(
             Item('volume', label = "Volume"),
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
            
        self.add_trait('volume',  Enum(self.volumes.keys()) )
        
