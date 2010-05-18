# This file needs to be imported within numpy distutils, before nipy has
# been built or installed.  It can't use any nipy components or relative
# imports
import os
_THIS_DIR = os.path.dirname(__file__)

# Please do not change these lines to use execfile because execfile is
# not available in Python 3
INFO_VARS = {}
exec(open(os.path.join(_THIS_DIR, 'info.py'), 'rt').read(), {}, INFO_VARS)

version = INFO_VARS['version']

if not INFO_VARS['is_release']:
    suffix = '.dev'
    import subprocess
    cmd = 'git log --max-count=1 --date=short --format="%cd#%h"'
    try:
        # Use git to get the revision number.  Execute git in the same
        # directory as this file.
        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            shell=True,
            cwd=_THIS_DIR)
    except OSError:
        # Either git was not found or nipy was imported from a
        # directory that isn't a git branch.  Just ignore.
        pass
    else:
        rev_str, err = proc.communicate()
        if not rev_str == '':
            rev_str = rev_str.strip().replace('-', '')
            rev_str = rev_str.replace('#', '-')
            suffix += rev_str
    version += suffix
