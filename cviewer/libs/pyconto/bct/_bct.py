

import sys
# check for linux support
if sys.platform == 'linux2':   
    # examine the architecture
    import platform as pf
    if '32' in pf.architecture()[0]:
        from bit32.bct import *
    elif '64' in pf.architecture()[0]:
        from bit64.bct import *
    else:
        raise('Can not determine your architecture settings.')
elif sys.platform == 'darwin':
    import platform as pf
    if '32' in pf.architecture()[0]:
        from mac.bct import *
    else:
        raise('No precompiled bct library available for your platform.')
else:
    raise('Your platform is not supported.')
        