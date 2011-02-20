""" The ConnectomeViewer wrapper for a cfflib object """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os

# Enthought library imports
from enthought.traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo, Property
from enthought.traits.ui.api import View, Item, auto_close_message, message

# ConnectomeViewer imports

from cviewer.visualization.render_manager import RenderManager
from cviewer.sources.datasource_manager import DatasourceManager
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.plugins.cff.ui.edge_parameters_view import EdgeParameters

import cfflib

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

from cbase import CBase

class CNetwork(CBase):
    """ The implementation of the Connectome Networks """
    
    # the render manager of this network
    rendermanager = Instance(RenderManager)
    
    # DatasourceManager Instance of this network
    datasourcemanager = Instance(DatasourceManager)
    
    # the cfflib CNetwork object
    obj = Instance(cfflib.CNetwork)
    
    graph = Property(Any, depends_on = [ 'obj' ])

    # edge parameters for visualization
    _edge_para = Instance(EdgeParameters)
    
    def _get_graph(self):
        return self.obj.data

    def _relabel_to_int(self, graph):
        import networkx as nx
        def intmap(x): return int(x)
        return nx.relabel_nodes(graph,intmap)

    def _get_matdict(self):
        if not self.loaded:
            self.load()
        import networkx as nx
        matdict = {}
        g = self._relabel_to_int(self.graph)
        # grab keys from the first edge, discarding id
        dl = self._get_edge_values()
                
        # create numpy matrix for each key using recarray
        matrec = nx.to_numpy_recarray(g, dtype=zip(dl, [float]*len(dl)) )
        for k in dl:
            matdict[k] = matrec[k]
        return matdict

    def _get_nodelabels(self, nodekey = 'dn_label'):
        if not self.loaded:
            self.load()
        g = self._relabel_to_int(self.graph)
        a = []
        return [v[nodekey] for n,v in g.nodes_iter(data=True)]

    def _get_edge_values(self):
        if not self.loaded:
            self.load()
        if len(self.obj.data.edges()) == 0:
            return
        edi = self.obj.data.edges_iter(data=True)
        u,v,ed = edi.next()
        ret = []
        for k,v in ed.items():
            if isinstance(v, float) or isinstance(v, int):
                ret.append(k)
        return ret

    def invoke_matrix_viewer(self, nodelabelkey = 'dn_label'):
        """ Invoke the Connectome Matrix Viewer """
        from cviewer.visualization.matrix.matrix_viewer2 import MatrixViewer
                
        cmatrix_viewer = MatrixViewer(self._get_nodelabels(nodekey = nodelabelkey),
                                      self._get_matdict())
        cmatrix_viewer.edit_traits()

    def __init__(self, **traits):
        super(CNetwork, self).__init__(**traits)
        
        
        
    