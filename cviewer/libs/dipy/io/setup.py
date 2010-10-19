def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration

    config = Configuration('io', parent_package, top_path)

    config.add_subpackage('tests')
    # do i want to include the .c. Is the .pyx automatically included?
    config.add_data_files('track_volumes.pyx')


    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())