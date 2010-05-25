#!/usr/bin/env python

from distutils.core import setup
from setuptools import find_packages
import os.path
import numpy as np

from build_helpers import make_cython_ext, package_check, INFO_VARS

# Hard and soft dependency checking
# XXX: When things could be easily installed with install_requires, why have it here?
# Also: with the installation script, required packages (h5py,cython,...) are installed from the web

package_check('scipy', INFO_VARS['scipy_min_version'])
package_check('networkx', INFO_VARS['networkx_min_version'])

def _h5py_version(pkg_name):
    from h5py import version
    return version.version
package_check('h5py', INFO_VARS['h5py_min_version'],version_getter=_h5py_version)

def _may_version(pkg_name):
    from enthought.mayavi import version
    return version.version

package_check('enthought.mayavi', INFO_VARS['mayavi_min_version'],version_getter=_may_version)

def _traits_version(pkg_name):
    from enthought.traits import version
    return version.__version__
package_check('enthought.traits', INFO_VARS['traits_min_version'],version_getter=_traits_version)

# Cython can be a build dependency
def _cython_version(pkg_name):
    from Cython.Compiler.Version import version
    return version
package_check('cython',
              INFO_VARS['cython_min_version'],
              optional=True,
              version_getter=_cython_version,
              messages={'opt suffix': ' - you will not be able '
                        'to rebuild Cython source files into C files',
                        'missing opt': 'Missing optional build-time '
                        'package "%s"'}
              )


# we use cython to compile the module if we have it
try:
    import Cython
except ImportError:
    has_cython = False
else:
    has_cython = True
    
per_ext, cmdclass = make_cython_ext(
    'cviewer.plugins.dipy.core.track_performance',
    has_cython,
    include_dirs = [np.get_include()])


tvol_ext, cmdclass = make_cython_ext(
    'cviewer.plugins.dipy.io.track_volumes',
    has_cython,
    include_dirs = [np.get_include()])

###

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
    description = "The ConnectomeViewer for Visualization and Analysis of Multi-Modal, Multi-Level Connectome Data",
    entry_points = {
        'console_scripts': [
            'connectomeviewer = cviewer.run:main',
            ],
        },
    install_requires = ["EnvisageCore >= 3.1.1",
                        "EnvisagePlugins >= 3.1.1",
                        "TraitsBackendWX >= 3.2.0",
                        "EnthoughtBase >= 3.0.3",
                        "TraitsGUI >= 3.1.0",
                        "Chaco >= 3.2.0",
                        "lxml >= 2.2.2"
                        ],
    license = "GPLv3",
    long_description = INFO_VARS['long_description'],
    maintainer = 'Stephan Gerhard',
    maintainer_email = 'info@connectomics.org',
    platforms = ["Linux", "Unix"],
    url = 'http://www.connectomeviewer.org/',
    packages = find_packages(),
    include_package_data = True,
    package_dir={'cviewer': 'cviewer'},
    scripts = [os.path.join('cviewer', 'scripts', 'cviewer_post_script.py'),
               'connectomeviewer.py',
               'build_helpers.py'],
    zip_safe = False,
    # From DiPy
    ext_modules = [per_ext,tvol_ext],
    cmdclass    = cmdclass,  
   )
