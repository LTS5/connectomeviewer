import _bct as bct
import numpy as np

def threshold_absolute(cmatrix, threshold):
    """ Applies an absolute weight threshold to a graph.  All weights below
    this threshold, as well as those on the main diagonal, are set to zero.
    """
    m = bct.to_gslm(cmatrix.tolist())
    thr = bct.threshold_absolute(m, threshold)
    r = bct.from_gsl(thr)
    bct.gsl_free(m)
    bct.gsl_free(thr)
    return np.asarray(r)

def threshold_proportional_dir(cmatrix, p):
    """ Preserves a given proportion of the strongest weights in a directed
    graph. All other weights, as well as those on the main diagonal, are
    set to zero.
    """
    m = bct.to_gslm(cmatrix.tolist())
    thr = bct.threshold_proportional_dir(m, p)
    r = bct.from_gsl(thr)
    bct.gsl_free(m)
    bct.gsl_free(thr)
    return np.asarray(r)

def threshold_proportional_und(cmatrix, p):
    """ Preserves a given proportion of the strongest weights in an
    undirected graph. All other weights, as well as those on the main
    diagonal, are set to zero.
    """
    m = bct.to_gslm(cmatrix.tolist())
    thr = bct.threshold_proportional_und(m, p)
    r = bct.from_gsl(thr)
    bct.gsl_free(m)
    bct.gsl_free(thr)
    return np.asarray(r)