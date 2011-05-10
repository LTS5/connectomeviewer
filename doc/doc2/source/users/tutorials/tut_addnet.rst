.. _addnet:

=============================
Add a simple directed network
=============================

We want to add a simple directed network to an empty connectome file. In the IPython Shell,
use the following code ::
    import numpy as np
    cfile.add_network_from_matrix_with_pos(name='Random Network', \
        matrix = np.random.random( (20,20) ), pos = np.random.random( (20, 3) ) * 10, \
        directed = True)
        
Activate this network by double-clicking on it (or right-click and activate).
A Mayavi scene is generated and the network should be rendered. You can do this also with::

    cfile.networks[0].active = True
    
Probably, you will need to update the scaling for the nodes::

    cfile.networks[0].rendermanager.update_node_scale_factor(0.5)

In the *Mayavi Visualization Tree*, click on Connections (parent node is Connectivity Source).
The *Mayavi Object Editor* show the changeable attributes. There, you can change the edge look.
Tab: Glyph, Sub-Tab: Glyph Source. You can select Arrow Source under section Glyph Source to
make the edges more appealing.

To get back the connectivity matrix, the following code will do::

    cfile.networks[0].get_matrix()
    
You get a binary matrix back. But you can also specify what edge attribute you want to use. They must
exist of course::

    cfile.networks[0].get_matrix(weight_key = 'my_edge_key')
    
Internally, the graph is stored as a NetworkX Graph. This means that you can use all
the features for analysis that are provided by the great `NetworkX toolbox <http://networkx.lanl.gov/>`_.
The graph is here::

    cfile.networks[0].graph
    
For example, calculate the clustering coefficients::

    import networkx as nx
    nx.clustering(cfile.networks[0].graph)

.. If you feel like having a different layout for your graph, you can use NetworkX layouting algorithms,
.. such as Fruchterman-Reingold force-directed algorithm.

..   import networkx as nx
..   import numpy as np
..  connectivity_matrix = np.random.random( (20,20) )