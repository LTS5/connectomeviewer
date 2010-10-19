""" The ConnectomeViewer Envisage application with Plugins

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

# Modified version. Adapted from the MayaVi2 application

# Standard library imports
import sys
import os.path
import os

# Enthought library imports
from enthought.traits.api import (HasTraits, Instance, Int, on_trait_change)
from enthought.etsconfig.api import ETSConfig

from enthought.logger.api import LogFileHandler, FORMATTER

# ConnectomeViewer imports
from cviewer.cviewer_workbench_application import CViewerWorkbenchApplication
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.info import version as ver

# Logger imports
import logging, logging.handlers
logger = logging.getLogger('root')
logger_envisage = logging.getLogger('enthought.envisage.plugin')
logger_ipython = logging.getLogger('enthought.plugins.ipython_shell.view.ipython_shell_view')
logger_pyface = logging.getLogger('enthought.pyface.ui.wx.workbench.editor_set_structure_handler')
logger_pyfaceview = logging.getLogger('enthought.pyface.workbench.i_view')

def setup_logger(logger, fname, stream=True, mode=logging.ERROR):
    """Setup a log file and the logger.

    Parameters
    ----------
    fname : string
        File name the logger should use.
    stream : bool
        Add a stream handler.
    mode : logging type
        The logging mode of the stream handler.
    
    """
    if not os.path.isabs(fname):
        path = os.path.join(ETSConfig.application_data, fname)
    else:
        path = fname
    
    create_file_handler = True
    # check if path exists
    dirname = os.path.dirname(path)
    
    if not(os.path.exists(dirname)):
        try:
            os.makedirs(dirname)           
        except OSError:
            #logger.error('Can not create path to store the log file.')
            create_file_handler = False

    # check if path is writable
    if not os.access(dirname, os.W_OK):
        print "==========================================="
        print "The application data path is not writable: ", dirname
        print "Please remove it with:"
        print "sudo rm -rf " + dirname
        print "and re-run the ConnectomeViewer with:"
        print "connectomeviewer.py -v"
        print "==========================================="
        raise Exception("PermissionError")

    # check if files exists, if not open it
    if not(os.path.exists(path)):
        # XXX add try catch
        file = open(path, 'w')
        file.close()
        
    # setting the logging level
    logger.setLevel(mode)

    # create formatter
    formatter = logging.Formatter("%(levelname)s - %(name)s - %(message)s")
    # old: "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
    
    if stream:
        # create console handler and set level to debug
        ch = logging.StreamHandler()
        ch.setLevel(mode)
        # add formatter to ch
        ch.setFormatter(formatter)
        # add ch to logger
        logger.addHandler(ch)


    if create_file_handler:
        filehandler = logging.handlers.RotatingFileHandler(path, maxBytes=1000000, backupCount=4)
        
        filehandler.setLevel(mode)
        filehandler.setFormatter(formatter)
        logger.addHandler(filehandler)
        logger_envisage.addHandler(filehandler)

        # does this fix the  
        # no handlers could be found for logger "enthought.envisage.plugin"
    
        logger_ipython.addHandler(filehandler)
        logger_pyface.addHandler(filehandler)
        logger_pyfaceview.addHandler(filehandler)
    
    import datetime
    import sys
    dt = datetime.datetime.now()
    outdate = dt.strftime("%A, %d. %B %Y %I:%M%p")
    logger.info("*"*5)
    if create_file_handler:
        logger.info("logfile: %s" % os.path.abspath(path))
    logger.info("cviewer version: %s " % ver)
    # logger.info("cviewer executable: %s " % str(__main__))
    logger.info("python executable: %s" % sys.executable)
    logger.info("python version: %s" % sys.version.replace('\n', ''))
    logger.info("execution date and time: %s" % outdate)
    logger.info("*"*5)

def get_non_gui_plugin_classes():
    """Get list of basic mayavi plugin classes that do not add any views or
    actions."""
    from enthought.envisage.core_plugin import CorePlugin
    from enthought.envisage.ui.workbench.workbench_plugin import WorkbenchPlugin
    from enthought.tvtk.plugins.scene.scene_plugin import ScenePlugin
    from enthought.mayavi.plugins.mayavi_plugin import MayaviPlugin
    
    plugins = [CorePlugin,
               WorkbenchPlugin,
               MayaviPlugin,
               ScenePlugin,
               ]
    return plugins

def get_non_gui_plugins():
    """Get list of basic mayavi plugins that do not add any views or
    actions."""
    return [cls() for cls in get_non_gui_plugin_classes()]

def get_plugin_classes():
    """Get list of default plugin classes to use for ConnectomeViewer."""

    # Force the selection of a toolkit:
    from enthought.traits.ui.api import toolkit
    toolkit()
    from enthought.etsconfig.api import ETSConfig
    try_use_ipython = preference_manager.cviewerui.useipython

    use_ipython = False
    if ETSConfig.toolkit == 'wx' and try_use_ipython:
        try:
            # If the right versions of IPython, EnvisagePlugins and
            # Pyface are not installed, this import will fail.
            from enthought.plugins.ipython_shell.view.ipython_shell_view \
                    import IPythonShellView
            use_ipython = True
        except: pass

    if use_ipython:
        from enthought.plugins.ipython_shell.ipython_shell_plugin import \
                IPythonShellPlugin
        PythonShellPlugin = IPythonShellPlugin
    else:
        from enthought.plugins.python_shell.python_shell_plugin import PythonShellPlugin
        
    from enthought.tvtk.plugins.scene.ui.scene_ui_plugin import SceneUIPlugin
    from cviewer.plugins.text_editor.text_editor_plugin import TextEditorPlugin
    
    plugins = get_non_gui_plugin_classes()
    plugins.extend([
                SceneUIPlugin,
                TextEditorPlugin,
                PythonShellPlugin,
                ])
    return plugins

def get_plugins():
    """Get list of default plugins to use for Mayavi."""
    return [cls() for cls in get_plugin_classes()]

def get_cviewer_plugins():
    """ Get list of ConnectomeViewer plugins """

    plugins = []

    logger.info('Plugins')
    logger.info('*******')

    from cviewer.plugins.cff.cff_plugin import ConnectomeFilePlugin
    # add ConnectomeFile plugin
    plugins.insert(0, ConnectomeFilePlugin())
    logger.info('Added ConnectomeFilePlugin')
    
#    from cviewer.plugins.cff2.cff_plugin import ConnectomeFile2Plugin
#    # add ConnectomeFile plugin
#    plugins.insert(0, ConnectomeFile2Plugin())
#    logger.info('Added ConnectomeFile2Plugin')
    

    from cviewer.plugins.analysis.analysis_ui_plugin import AnalysisUIPlugin
    # add ConnectomeAnalysis UI plugin    
    plugins.insert(0, AnalysisUIPlugin())
    logger.info('Added AnalysisUIPlugin')     
    
    from cviewer.plugins.ui.cviewer_ui_plugin import CViewerUIPlugin
    # add ConnectomeViewerUserInterface plugin
    plugins.insert(0, CViewerUIPlugin())
    logger.info('Added CViewerUIPlugin')
    
    # add Bindings plugin
    from cviewer.plugins.bindings.bindings_plugin import BindingsPlugin
    plugins.append(BindingsPlugin())
    logger.info('Added BindingsPlugin')

    # add sLORETA Converter plugin
    from cviewer.plugins.sloreta.sloreta_plugin import SLoretaConverterPlugin
    plugins.append(SLoretaConverterPlugin())
    logger.info('Added sLORETA Converter Plugin')
    
    # add NBS plugin
    from cviewer.plugins.nbs.nbs_plugin import NBSPlugin
    plugins.append(NBSPlugin())
    logger.info('Added Network Based Statistics (NBS) Plugin')
    
    # add DiPy plugin
    try:
        dipy_works = True
        from cviewer.plugins.dipy.dipy_plugin import DipyPlugin
    except ImportError:
        dipy_works = False
    if dipy_works:
        plugins.append(DipyPlugin())
        logger.info('Added Diffusion in Python (DiPy) Plugin')
    
    return plugins


###########################################################################
# `CViewer` class.
###########################################################################
class CViewer(HasTraits):
    """The Connectome Viewer application class. """

    # The main envisage application.
    application = Instance('enthought.envisage.ui.workbench.api.WorkbenchApplication')

    # The MayaVi Script instance.
    script = Instance('enthought.mayavi.plugins.script.Script')

    # The logging mode.
    log_mode = Int(logging.ERROR, desc='the logging mode to use')
    
    def main(self, argv=None):
        """The main application is created and launched here.

        Parameters
        ----------
        argv : list of strings
            The list of command line arguments.  The default is `None`
            where no command line arguments are parsed.  To support
            command line arguments you can pass `sys.argv[1:]`.
        log_mode : 
            The logging mode to use.

        """

        # parse any cmd line args.
        if argv is None:
            argv = []
        self.parse_command_line(argv)

        # setup logging
        self.setup_logger()

        # add all default plugins
        plugins = get_plugins()

        # add ConnectomeViewer plugins
        plugins = get_cviewer_plugins() + plugins

        # create the application
        prefs = preference_manager.preferences
        
        # create the application object
        self.application = CViewerWorkbenchApplication(plugins=plugins,
                                                       preferences=prefs)
        
        # start the application.
        self.application.run()
        
        logger.info('We hope you enjoyed using the ConnectomeViewer!')
        
    def setup_logger(self):
        """ Setting up the root logger """
        from enthought.etsconfig.api import ETSConfig
        
        path = os.path.join(ETSConfig.application_data,
                            'ch.connectome.viewer', 'cviewer.log')
        path = os.path.abspath(path)
        
        setup_logger(logger, path, mode=self.log_mode)

    def parse_command_line(self, argv):
        """Parse command line options.

        Parameters
        ----------

        - argv : `list` of `strings`

          The list of command line arguments.
        """
        from optparse import OptionParser
        usage = "usage: %prog [options]"
        parser = OptionParser(usage)

        (options, args) = parser.parse_args(argv)

    def run(self):
        """This function is called after the GUI has started.
        """
        pass


def main(argv=None):
    """ Helper to start up the ConnectomeViewer application. """
    
    m = CViewer()
    m.main(argv)
    return m
