""" Executes a script in the console """

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

# Adapted from enthought.mayavi.action.save_load

# Standard library imports.
import sys 
from os.path import isfile, dirname

# Enthought library imports.
from enthought.pyface.api import FileDialog, OK
from enthought.pyface.action.api import Action
from cviewer.plugins.ui.preference_manager import preference_manager

# Local imports
from enthought.mayavi.plugins.script import get_imayavi
from enthought.mayavi.core.common import error, exception

######################################################################
# `RunScript` class.
######################################################################
class RunScript(Action):
    """ An action that runs a mayavi script.  
    
    WARNING: this can be dangerous since the file runs execfile! """

    tooltip       = "Execute a Python script (typically a Mayavi script)"

    description   = "Execute a Python script (typically a Mayavi script)"

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event):
        """ Performs the action. """
        wildcard = 'Python files (*.py)|*.py'
        parent = self.window.control
        
        # path from preference manager
        pref_script_path = preference_manager.cviewerui.scriptpath
        
        if pref_script_path == '':
            # store executed script path in preferences
            dialog = FileDialog(parent=parent,
                            title='Open Python file',
                            action='open', wildcard=wildcard,
                            )            
        else:
            dialog = FileDialog(parent=parent,
                            title='Open Python file',
                            action='open', wildcard=wildcard,
                            default_directory=pref_script_path,
                            )
        
        if dialog.open() == OK:
            if not isfile(dialog.path):
                error("File '%s' does not exist"%dialog.path, parent)
                return
            
            # Get the globals.
            # The following code is taken from scripts/mayavi2.py.
            g = sys.modules['__main__'].__dict__
            if 'mayavi' not in g:
                mv = get_imayavi(self.window)
                g['mayavi'] = mv
                g['engine'] = mv.engine
                
            if 'cfile' not in g:
                # load cfile reference into gloabl name space
                cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
                g['cfile'] = cfile
            
            # always store last executed path in preferences
            # but this only gets definitely stored when one open the preference manager
            preference_manager.cviewerui.scriptpath = dirname(dialog.path)
            
            # Do execfile
            try:
                # If we don't pass globals twice we get NameErrors and nope,
                # using exec open(script_name).read() does not fix it.
                execfile(dialog.path, g, g)
            except Exception, msg:
                exception(str(msg))
