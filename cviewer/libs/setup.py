import ConfigParser
import os

################################################################################
def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    from numpy.distutils.system_info import system_info
    
    config = Configuration('libs', parent_package, top_path)

    config.add_subpackage('pyeeg')
    config.add_subpackage('tomoviz')
    
        
    # List all data directories to be loaded here
    # add setup.py with specifics
    config.add_data_dir('dipy')
    
    # add pyconto as datadir, because it needs to include all the __init__
    # config.add_subpackage('pyconto/algorithms/statistics/nbs')
    config.add_data_dir('pyconto')
    
    # config.add_data_dir('joblib')
    # config.add_data_dir('parallelpython')
    # config.add_data_dir('openmeeg')
    # config.add_data_dir('pyxnat')

    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())