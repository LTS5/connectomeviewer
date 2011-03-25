def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration

    config = Configuration('cff2', parent_package, top_path)

    config.add_subpackage('actions')
    config.add_subpackage('trackvis')
    config.add_subpackage('ui')

    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())
