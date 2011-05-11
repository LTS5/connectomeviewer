""" Interface to invoke a thread to start he volume slicer """

# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

import logging
logger = logging.getLogger('root.'+__name__)

from threading import Thread

class ThreadedVolumeSlicer(Thread):
    """ Open an instance of the Volume slicer """
    
    def __init__(self, fname, **kwargs):
        Thread.__init__(self, **kwargs)
        self.fname = fname

    def run(self):
        logger.debug('Invoking Volume Slicer in a Thread.')
        
        from subprocess import call
        import sys
        import os.path
        if os.path.isfile(self.fname):
            
            # invoke volume slicer as external thread, quite hackish but works
            # FIXME: maybe better with subprocess module!
            
            # get current path
            command = os.path.join(os.path.dirname(__file__) , 'volume_slicer.py')
            logger.debug('Invoke command: ' + command)
            # get python interpreter
            # pyt = os.path.join(sys.prefix, 'bin', 'python')
            pyt = sys.executable
            logger.debug('Python Interpreter: '+pyt)
            try:
                retcode = call(pyt + " " + command + " " + self.fname, shell=True)
            except e:
                logger.error("Execution of Volume Slicer failed:" + e)
                    
