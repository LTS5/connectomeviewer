''' Module with helper functions for cython building and package checking (derived from NiPy) '''

import os
from os.path import join as pjoin
import warnings

from distutils.version import LooseVersion
from distutils.extension import Extension
from numpy.distutils import log


# Get project related strings. Please do not change this line to use
# execfile because execfile is not available in Python 3
_info_fname = pjoin('cviewer', 'info.py')
INFO_VARS = {}
exec(open(_info_fname, 'rt').read(), {}, INFO_VARS)


# Dependency checks
def package_check(pkg_name, version=None,
                  optional=False,
                  checker=LooseVersion,
                  version_getter=None,
                  messages=None
                  ):
    ''' Check if package `pkg_name` is present, and correct version

    Parameters
    ----------
    pkg_name : str
       name of package as imported into python
    version : {None, str}, optional
       minimum version of the package that we require. If None, we don't
       check the version.  Default is None
    optional : {False, True}, optional
       If False, raise error for absent package or wrong version;
       otherwise warn
    checker : callable, optional
       callable with which to return comparable thing from version
       string.  Default is ``distutils.version.LooseVersion``
    version_getter : {None, callable}:
       Callable that takes `pkg_name` as argument, and returns the
       package version string - as in::
       
          ``version = version_getter(pkg_name)``

       If None, equivalent to::

          mod = __import__(pkg_name); version = mod.__version__``
    messages : None or dict, optional
       dictionary giving output messages
    '''
    if version_getter is None:
        def version_getter(pkg_name):
            mod = __import__(pkg_name)
            return mod.__version__
    if messages is None:
        messages = {}
    msgs = {
         'missing': 'Cannot import package "%s" - is it installed?',
         'missing opt': 'Missing optional package "%s"',
         'opt suffix' : '; you may get run-time errors',
         'version too old': 'You have version %s of package "%s"'
         ' but we need version >= %s', }
    msgs.update(messages)
    try:
        __import__(pkg_name)
    except ImportError:
        if not optional:
            raise RuntimeError(msgs['missing'] % pkg_name)
        log.warn(msgs['missing opt'] % pkg_name +
                 msgs['opt suffix'])
        return
    if not version:
        return
    try:
        have_version = version_getter(pkg_name)
    except AttributeError:
        raise RuntimeError('Cannot find version for %s' % pkg_name)
    if checker(have_version) < checker(version):
        if optional:
            log.warn(msgs['version too old'] + msgs['opt suffix'],
                     have_version,
                     pkg_name,
                     version)
        else:
            raise RuntimeError(msgs['version too old'],
                               have_version,
                               pkg_name,
                               version)
            


def make_cython_ext(modulename,
                    has_cython,
                    include_dirs=None,
                    extra_c_sources=None):
    ''' Create Cython extension builder from module names

    Returns extension for building and command class depending on
    whether you want to use Cython and ``.pyx`` files for building
    (`has_cython` == True) or the Cython-generated C files (`has_cython`
    == False).

    Assumes ``pyx`` or C file has the same path as that implied by
    modulename. 

    Parameters
    ----------
    modulename : string
       module name, relative to setup.py path, with python dot
       separators, e.g mypkg.mysubpkg.mymodule
    has_cython : bool
       True if we have cython, False otherwise
    include_dirs : None or sequence
       include directories
    extra_c_sources : None or sequence
       sequence of strings giving extra C source files

    Returns
    -------
    ext : extension object
    cmdclass : dict
       command class dictionary for setup.py

    Examples
    --------
    You will need Cython on your python path to run these tests. 
    
    >>> modulename = 'pkg.subpkg.mymodule'
    >>> ext, cmdclass = make_cython_ext(modulename, True, None,['test.c'])
    >>> ext.name == modulename
    True
    >>> pyx_src = os.path.join('pkg', 'subpkg', 'mymodule.pyx')
    >>> ext.sources == [pyx_src, 'test.c']
    True
    >>> import Cython.Distutils
    >>> cmdclass['build_ext'] == Cython.Distutils.build_ext
    True
    >>> ext, cmdclass = make_cython_ext(modulename, False, None, ['test.c'])
    >>> ext.name == modulename
    True
    >>> pyx_src = os.path.join('pkg', 'subpkg', 'mymodule.c')
    >>> ext.sources == [pyx_src, 'test.c']
    True
    >>> cmdclass
    {}
    '''
    if include_dirs is None:
        include_dirs = []
    if extra_c_sources is None:
        extra_c_sources = []
    if has_cython:
        src_ext = '.pyx'
    else:
        src_ext = '.c'
    pyx_src = pjoin(*modulename.split('.')) + src_ext
    sources = [pyx_src] + extra_c_sources
    ext = Extension(modulename, sources, include_dirs = include_dirs)
    if has_cython:
        from Cython.Distutils import build_ext
        cmdclass = {'build_ext': build_ext}
    else:
        cmdclass = {}
    return ext, cmdclass
        

