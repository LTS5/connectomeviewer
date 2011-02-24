import threading
import subprocess
import os

import logging
logger = logging.getLogger('root.'+__name__)

class ThreadedTrackvis(threading.Thread):
    
    def __init__(self, tvpath, trkfname):
        self.tvpath = tvpath
        self.trkfname = trkfname
        self.stdout = None
        self.stderr = None
        threading.Thread.__init__(self)

    def run(self):
        newcmd = '%s %s ' % (self.tvpath, self.trkfname)
        if os.path.isfile(self.tvpath):
            process = subprocess.call( newcmd, shell = True)
            if process < 0:
                logger.debug("Trackvis Thread was terminated by signal " + str(process))
            else:
                logger.debug("Trackvis Thread returned with returncode " + str(process))
            