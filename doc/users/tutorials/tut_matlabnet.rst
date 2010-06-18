.. _matlabnet:

===========================================
Add network from Matlab connectivity matrix
===========================================

It is assumed that you have a connectivity matrix and node positions stored in
Matlab files. Here you learn how to directly load these data in ConnectomeViewer
and visualize the network.

The following code will do ::

    import networkx  as  nx
    import scipy.io as sio

    M = sio.loadmat('M.mat', matlab_compatible=True)
    A = M['ConMatrix']
    N = sio.loadmat('P.mat', matlab_compatible=True)
    P = N['Coordinate']

    cfile.add_network_from_matrix_with_pos(name='Network', matrix = A,pos = P, directed = False)
    cfile.networks[0].active = True
    cfile.networks[0].select_all()

You can either type the code line-by-line in the Ipython console or store it as a
script (Menu: File->New Text File. Ctrl-S to store it as .py file. Run it with Ctrl-R).

Of course you have to adapte the paths to your Matlab files and the names of your
arrays (as seen in the Matlab environment).

If you have a directed network (asymmetric matrix, set directed = True), the edges are
still displayed without arrows. But you can change this by opening the the *Mayavi Visualization Tree*
(Menu: View->Other). Double-click on Connections (parent node is Connectivity Source).
The *Mayavi Object Editor* show the changeable attributes. Under tab Glyph, subtab Glyph Source,
you select *Arrow Source*. Below you can adjust the look of the arrows of your directed network.

See also the Graph Layout tutorial to create different graph layouts.

