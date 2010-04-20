""" Test module for the Python Connectome Toolbox
"""

import nose
import numpy as np
from numpy.testing import *
from cviewer.plugins.pyconto.basics import *

# http://somethingaboutorange.com/mrl/projects/nose/0.11.2/testing_tools.html

# include common test data
import commontestdata as ctd

class TestBasics:

    def __init__(self):
        self.BU = ctd.BU

    #@nose.with_setup(setup_func, teardown_func)
    def test_degree(self):
        assert_array_almost_equal(self.BU, degree())

# Parametric tests
#def check_even(n, nn):
#    """A check function to be used in a test generator."""
#    assert n % 2 == 0 or nn % 2 == 0
#
#def test_evens():
#    for i in range(0,4,2):
#        yield check_even, i, i*3


if __name__ == "__main__":
    run_module_suite()
