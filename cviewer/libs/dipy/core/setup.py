def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration

    config = Configuration('core', parent_package, top_path)

    config.add_subpackage('bench')
    config.add_subpackage('stat')
    config.add_subpackage('tests')
    
    config.add_data_dir('matrices')
    #config.add_data_files('reconstruction_performance.pyx', 'track_performance.pyx', 'pyalloc.pxd')
        
    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())