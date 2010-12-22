
from enthought.traits.api import Code, Button, Int, on_trait_change, Any, HasTraits,List, Str, Enum, Instance, Bool
from enthought.traits.ui.api import (View, Item, Group, HGroup, CodeEditor,
                                     spring, Handler, EnumEditor)

from cviewer.plugins.cff2.cnetwork import CNetwork

class NetworkParameter(HasTraits):
    
    engine = Enum("Mayavi", ["Mayavi"])
#    graph = Any
    
    view = View(
             Item('engine', label = "Use Engine"),
             Item('graph', label = "Graph"),
             Item('node_position', label = "Node Positions"),
             Item('edge_value', label="Edge Value"),
             Item('node_label', label="Node Label"),
             id='cviewer.plugins.codeoracle.networkparameter',
             buttons=['OK'], 
             resizable=True,
             title = "Create surface ..."
             )
    
    def _graph_changed(self, value):
        self.remove_trait("node_position")
        self.remove_trait("edge_value")
        self.remove_trait("node_label")
        self.add_trait('node_position',  Enum(self.netw[value]['pos']) )
        self.add_trait('edge_value',  Enum(self.netw[value]['ev']) )   
        self.add_trait('node_label',  Enum(self.netw[firstk]['lab']) )      
        
    def __init__(self, cfile, **traits):
        super(NetworkParameter, self).__init__(**traits)
        
        self.netw = {}
        
        for cobj in cfile.connectome_network:
            if cobj.loaded:
                if isinstance(cobj, CNetwork):
                    # add more info
                    a=cobj.obj.data.nodes_iter(data=True)
                    n, dn = a.next()
                    npos = []
                    lab = []
                    for k in dn.keys():
                        if 'position' in k or 'pos' in k or 'location' in k:
                            npos.append(k)
                        if 'name' in k or 'label' in k:
                            lab.append(k)
                    if len(npos) == 0:
                        npos = ["None"]
                    if len(lab) == 0:
                        lab = ["None"]
                        
                    a=cobj.obj.data.edges_iter(data=True)
                    e1, e2, de = a.next()
                    ev = []
                    for k in de.keys():
                        if isinstance(de[k], float) or isinstance(de[k], int):
                           ev.append(k) 
                    if len(ev) == 0:
                        ev = ["None"]
                        
                    self.netw[cobj.name] = {'name' : cobj.obj.name,
                                            'ev' : ev, 'pos' : npos, 'lab' : lab}

        if len(self.netw) == 0:
            self.netw["None"] = {'name' : "None", 'ev' : "None", 'pos' : "None", 'lab' : "None"}
                    
        self.add_trait('graph',  Enum(self.netw.keys()) )
        firstk = self.netw.keys()[0]
        self.add_trait('node_position',  Enum(self.netw[firstk]['pos']) )
        self.add_trait('edge_value',  Enum(self.netw[firstk]['ev']) )
        self.add_trait('node_label',  Enum(self.netw[firstk]['lab']) )      
        
        