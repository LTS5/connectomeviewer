import ConfigParser
import os

################################################################################
def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    from numpy.distutils.system_info import system_info
    
    config = Configuration('cviewer', parent_package, top_path)

    # List all packages to be loaded here
    config.add_subpackage('action')
    config.add_subpackage('io')
    config.add_subpackage('libs')    
    config.add_subpackage('plugins')
    config.add_subpackage('resources') # necessary when in data_dir?
    config.add_subpackage('sources')
    config.add_subpackage('visualization')
    
    # List all data directories to be loaded here
    config.add_data_dir('resources')

    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())