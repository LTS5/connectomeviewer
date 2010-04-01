#!/usr/bin/env python

from distutils.core import setup
from setuptools import find_packages
import os.path

# From DiPy Setup
import numpy as np

from cviewer.scripts.build_helpers import make_cython_ext

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
    version = '0.1.6',
    author = "Stephan Gerhard",
    author_email = "connectome@unidesign.ch",
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
    description = "The ConnectomeViewer for Multi-Modal, Multi-Level Network Visualization and Analysis",
    entry_points = {
        'console_scripts': [
            'connectomeviewer = cviewer.run:main',
            ],
        },
   # install_requires = ["networkx==1.0", ],
    license = "GPLv3",
    long_description = """The ConnectomeViewer is a extensible, scriptable, pythonic software tool for visualization
and analysis in structural neuroimaging research on many spatial scales. Employing the
Connectome File Format, diverse data such as networks, surfaces, volumes, tracks and metadata
are handled and integrated. """,
    maintainer = 'Stephan Gerhard',
    maintainer_email = 'info@connectomics.org',
    platforms = ["Linux", "Unix"],
    url = 'http://www.connectomeviewer.org/',
    packages = find_packages(),
    include_package_data = True,
    package_dir={'cviewer': 'cviewer'},
    scripts = [os.path.join('cviewer', 'scripts', 'cviewer_post_script.py'),
               'connectomeviewer.py'],
    # From DiPy
    ext_modules = [per_ext,tvol_ext],
    cmdclass    = cmdclass,  
   )
