
from os.path import join as pjoin, dirname

import numpy as np

from numpy.testing import assert_array_equal, assert_array_almost_equal

from nose.tools import assert_true, assert_false, \
     assert_equal, assert_raises
     

try:
    import pyconto.bct.measures as bm
except ImportError:
    from nose import SkipTest
    raise SkipTest



cm = np.random.random( (10,10) )

def test_degree(self):
    all, ain, aout = bm.degree(cm, True)
    print all
    print ain
    print aout

def test_edge_betweenness(self):
    ebc, bc = bm.edge_betweenness(cm, True)
    print ebc
    print bc
    
def test_findpaths(self):
    out = bm.findpaths(cm, np.array([0,1,3]), 1)
