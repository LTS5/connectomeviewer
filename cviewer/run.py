""" Startup script for the ConnectomeViewer application

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# Standard library imports
import sys
import logging

# First, and before importing any Enthought packages, set the ETS_TOOLKIT
# environment variable to qt4, to tell Traits that we will use Qt.
#import os
#os.environ['ETS_TOOLKIT'] = 'qt4'

# ConnectomeViewer imports
from cviewer.app import CViewer

# Testing for corrext wxversion
# Try forcing the use of wx 2.8 before any other import.
import sys
if not 'wx' in sys.modules:
    try:
        from enthought.etsconfig.api import ETSConfig
        if ETSConfig.toolkit in ('wx', ''):
            import wxversion
            wxversion.ensureMinimal('2.8')
    except ImportError:
        """ wxversion not installed or not 2.8 or higher"""

##########################################################################
# `CViewerApp` class
##########################################################################

class CViewerApp(CViewer):
    
    def parse_command_line(self, argv):
        """Parse command line options. """

        # This is a overwritten method
        # Parse and store command line options to process
        # once app has started in `run`.
        
        from optparse import OptionParser
        usage = "usage: %prog [options]"
        parser = OptionParser(usage, version="%prog 0.1.0")
        #parser.add_option("--load-cff", 
        #              type="string", dest="cfile", default='',
        #              help="Open the given .cff file directly.")
        parser.add_option("-v", "--verbose",
                      dest="verbose", default=False,
                      action="store_true", 
                      help="Print out debug information.")
    
        (options, args) = parser.parse_args(argv)
        
        self.cmd_line_opts = (options, args)
        
        if options.verbose:
            self.log_mode = logging.DEBUG
        else:
            self.log_mode = logging.ERROR


    def run(self):
        """Process the command line options and setup cviewer as per
        the users needs.
        """
        
        options, args = self.cmd_line_opts
        # Process the options.
        

def main():
    """ Startup of the Connectome Viewer """
    global cviewer
    
    # allowing imports like cviewer.plugins.
    if '' not in sys.path:
        sys.path.insert(0, '')
    # to prevent a bug (?) on 64 bit machines
    # we need to import the gifti libraries before the main loop
    import cviewer.io.gifti.gifti as gifti
    
    cviewer = CViewerApp()
    cviewer.main(sys.argv[1:])
