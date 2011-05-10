import os
_THIS_DIR = os.path.dirname(__file__)

# Please do not change these lines to use execfile because execfile is
# not available in Python 3
INFO_VARS = {}
exec(open(os.path.join(_THIS_DIR, 'info.py'), 'rt').read(), {}, INFO_VARS)

version = INFO_VARS['version']
INFO_VARS['is_release']
