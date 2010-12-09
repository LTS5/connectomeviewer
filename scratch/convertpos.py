""" This uses the OLD ConnectomeViewer to extract node
positions based on the centroids of the surface patches """

import numpy as np
import networkx as nx

for n in cfile.networks:
    n._de_activate()
    nn = len(n.graph.nodes())
    
    g = nx.read_graphml('/home/stephan/dev/cmt/cmt/data/parcellation/lausanne2008/resolution%i/resolution%i.graphml' % (nn, nn))
    
    for s in n.surfaces[1].surfaces:
        s._layout_3DView()
        pos = n.datasourcemanager._srcobj.positions
        for i in range(0,len(pos)):
            g.node[str(i+1)]['dn_position_%s'%s.name.lower()] = str(tuple(pos[i]))
    
    nx.write_graphml(g, '/home/stephan/dev/cmt/cmt/data/parcellation/lausanne2008/resolution%i/resolution%i.graphml2' % (nn, nn))
    n._de_activate()
    #np.save('fs_%s_%s.npy' % (n.networkname, s.name),
    #        n.datasourcemanager._srcobj.positions)