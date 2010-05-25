# Write pajek file for mapequation.org

def write_pajek(network, wkey, lkey, fname):
    """ Write Pajek file for use in mapequation.org 

    Parameters
    ----------
    network : `INetwork`
    wkey : edge key to use as weight
    lkey : node key to use as label
    fname : file name to write to
    """

    f = open(fname, 'w')
    # is network directed?
    dir = network.directed
    # get graph reference
    G = network.graph
    
    # write nodes according to pajek format
    f.write("*Vertices %s\n"%(G.order()))
    nodes = G.nodes()
    nodenumber=dict(zip(nodes,range(1,len(nodes)+1)))
    for n in nodes:
        na = G.node[n]
        if na.has_key(lkey):
            f.write("%d \"%s\" 1.0" %(nodenumber[n], na[lkey]))
        else:
            print lkey, ' not available for node ', n
        f.write("\n")

    # write edges according to pajek format
    if dir:
        f.write("*Arcs\n")
    else:
        f.write("*Edges\n")
    for u,v,edgedata in G.edges(data=True):

        if edgedata.has_key(wkey):
           f.write("%d %d %f "%(nodenumber[u],nodenumber[v], float(edgedata[wkey])))
        else:
           print wkey, ' not available for edge from ', u, ' to ', v
        
        f.write("\n")

    f.close()
    
write_pajek(cfile.networks[3], 'de_nnz', 'dn_label', 'highres.net')
