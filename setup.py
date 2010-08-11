#!/usr/bin/env python
import sys
from glob import glob
from distutils import log
from distutils.cmd import Command
import numpy as np

# monkey-patch numpy distutils to use Cython instead of Pyrex
from build_helpers import generate_a_pyrex_source, package_check, make_cython_ext, INFO_VARS

from numpy.distutils.command.build_src import build_src
build_src.generate_a_pyrex_source = generate_a_pyrex_source

def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration

    config = Configuration(None, parent_package, top_path)
    config.set_options(ignore_setup_xxx_py=True,
                       assume_default_configuration=True,
                       delegate_options_to_subpackages=True,
                       quiet=True)
    
    config.add_subpackage('cviewer')
    return config


################################################################################
# Dependency check
################################################################################

def _mayavi_version(pkg_name):
    from enthought.mayavi import version
    return version.version

# Cython can be a build dependency
def _cython_version(pkg_name):
    from Cython.Compiler.Version import version
    return version

def _traits_version(pkg_name):
    from enthought.traits import version
    return version.__version__

package_check('scipy', INFO_VARS['scipy_min_version'])
package_check('networkx', INFO_VARS['networkx_min_version'])
package_check('numpy', INFO_VARS['numpy_min_version'])
package_check('enthought.mayavi', INFO_VARS['mayavi_min_version'],version_getter=_mayavi_version)
package_check('enthought.traits', INFO_VARS['traits_min_version'],version_getter=_traits_version)
package_check('cython',INFO_VARS['cython_min_version'],optional=True,version_getter=_cython_version)

################################################################################
# we use cython to compile the module if we have it. does it work?
################################################################################
try:
    import Cython
except ImportError:
    has_cython = False
else:
    has_cython = True
    
per_ext, cmdclass = make_cython_ext(
    'cviewer.libs.dipy.core.track_performance',
    include_dirs = [np.get_include()])

tvol_ext, cmdclass = make_cython_ext(
    'cviewer.libs.dipy.io.track_volumes',
    include_dirs = [np.get_include()])

rec_ext, cmdclass = make_cython_ext(
    'cviewer.libs.dipy.core.reconstruction_performance',
    include_dirs = [np.get_include()])

################################################################################
    
def main(**extra_args):
    from numpy.distutils.core import setup
    setup(
        name = 'ConnectomeViewer',
        version = INFO_VARS['version'],
        author = "Stephan Gerhard",
        author_email = "stephan.gerhard@epfl.ch",
        classifiers = [c.strip() for c in """\
            Development Status :: 4 - Beta
            Intended Audience :: Developers
            Intended Audience :: Science/Research
            Operating System :: OS Independent
            Operating System :: POSIX
            Operating System :: POSIX :: Linux 
            Operating System :: Unix
            Programming Language :: Python
            Topic :: Scientific/Engineering
            Topic :: Software Development
            """.splitlines() if len(c.split()) > 0],    
        description = "Connects Multi-Modal Multi-Scale Neuroimaging Datasets For Analysis and Visualization in Python",
#        install_requires = ["EnvisageCore >= 3.1.1",
#                            "EnvisagePlugins >= 3.1.1",
#                            "TraitsBackendWX >= 3.2.0",
#                            "EnthoughtBase >= 3.0.3",
#                            "TraitsGUI >= 3.1.0",
#                            "Chaco >= 3.2.0",
#                            "lxml >= 2.2.2"
#                            ],
        license = "GPLv3",
        long_description = INFO_VARS['long_description'],
        maintainer = 'Stephan Gerhard',
        maintainer_email = 'info@connectomics.org',
        platforms = ["Linux", "Unix"],
        url = 'http://www.connectomeviewer.org/',
        scripts = glob('scripts/*.py'),
        ext_modules = [per_ext,tvol_ext, rec_ext],         
        configuration = configuration,
        **extra_args
        )

    
if __name__ == "__main__":
    main()
