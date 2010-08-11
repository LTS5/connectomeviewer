import networkx as nx
import scipy.io as sio

# import the data
# you need to be in the correct path when executing the script
# or enter absolute path

M = sio.loadmat('data/M.mat', matlab_compatible=True)
A = M['ConMatrix']
N = sio.loadmat('data/P.mat', matlab_compatible=True)
P = N['Coordinate']

cfile.add_network_from_matrix_with_pos(name='Network', matrix = A, pos = P, directed = False)
cfile.networks[0].active = True
cfile.networks[0].select_all()

