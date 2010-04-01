""" Python Connectome Toolbox (PyConTo) basic measures """

# Goals: Python Connectome Toolbox: PyConTo
# - a wrapper for particular networkx functions (algorithms)
# - re-implementation of additional matlab brain connectivity toolbox functions
# - use fast scipy/numpy matrix operations for calculations
# - use networkx data structures
# - clear API ready to build processing pipelines with scripts
#   especially for handling: BD, BU, WU, WD, disconnected graphs
# - methods:
        # binarize matrices
        # principal component of a disconnected network
        # build H0 networks (random, ordered, other)
        # thresholding (absolute, proportional weight threshold)
        # network characterisation across a broad range of thresholds
        # un-directionalize (sum, average, ...)
        # local measures, generate distributions

# Thoughts
# - if you want to meaningfully compare networks, the nodes have to represent
#   "the same", e.g. parcellation scheme. if they are different, diff. network measure outcomes
# - what to to with negative weights? with self-connections?

# Later
# - nice GUIs for the functions
# - network analysis tree with processing steps & visualizations
# - employ parallel processing capabilities


def degree():
    import numpy as np
    return np.array([[1,2,2],[2,3,4]])