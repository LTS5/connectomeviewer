""" Helper module for displaying node information """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library import
from enthought.traits.api import HasTraits, Str, Color, Button, Int
from enthought.traits.ui.api import View, Item

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class Node(HasTraits):
    """ A small node wrapper class for displaying node information
    """
    
    dn_label = Str('')
    dn_freesurfer_structname = Str('')
    dn_internal_id = Str('')
    xlink = Str('')
    openurl = Button
    # node degree
    #indegree = Int(-1) # when directed
    #outdegree = Int(-1)
    degree = Int(-1) # when not directed
    dn_color = Color((0,0,0))
    dn_intensityvalue = Int(-1)
    has_selfloop = Str('')

    def __init__(self, nodedata, **traits):
        super(HasTraits, self).__init__(**traits)
        self.dn_internal_id = nodedata['dn_internal_id']
        for key, value in nodedata.items():
            if key == 'dn_label':
                self.dn_label = value
            elif key == 'dn_freesurfer_structname':
                self.dn_freesurfer_structname = value
            elif key == 'xlink':
                self.xlink = value
            elif key == 'dn_colorcode':
                import types
                if type(value) == types.TupleType:
                    self.dn_color = value
                else:
                    # try to parse the string
                    # must be in the form: R,G,B
                    e = value.replace(' ', '')
                    e = e.split(',')
                    self.dn_color = str(tuple(map(int,e)))
            elif key == 'dn_intensityvalue':
                self.dn_intensityvalue = int(value)
            elif key == 'degree':
                self.degree = int(value)
            elif key == 'has_selfloop':
                if value:
                    self.has_selfloop = 'Yes'
                else:
                    self.has_selfloop = 'No'
                    
            #elif key == 'indegree':
            #    self.indegree = int(value)
            #elif key == 'outdegree':
            #    self.outdegree = int(value)

    view = View( 
        Item( name = 'dn_label', label = 'Label:', style = 'readonly', visible_when = "dn_label != ''",),
        Item( name = 'dn_freesurfer_structname', label = 'Freesurfer Name:', style = 'readonly', visible_when = "dn_freesurfer_structname != ''",),
        Item( name = 'dn_internal_id', label = 'Internal Node ID:', style = 'readonly', visible_when = "dn_internal_id != ''",),
        Item( name = 'dn_color', label = 'Color Code:', style = 'readonly', ),
        Item( name = 'degree', label = 'Node Degree:', style = 'readonly', visible_when = "degree != -1"),
        #Item( name = 'indegree', label = 'Node In-Degree:', style = 'readonly', visible_when = "indegree != -1"),
        #Item( name = 'outdegree', label = 'Node Out-Degree:', style = 'readonly', visible_when = "outdegree != -1"),
        Item( name = 'dn_intensityvalue', label = 'Intensity Value:', style = 'readonly', visible_when = "dn_intensityvalue != -1"),
        Item( name = 'has_selfloop', label = 'Has self-loop?', style = 'readonly', visible_when = "has_selfloop != ''"),
        Item( name = 'openurl', label = 'Open information in Webbrowser ...', show_label = False, visible_when = "xlink != ''"),
        title     = 'Node Information',
        buttons   = [ 'OK' ],
        width     = 500,
        height    = 250,
        resizable = True,
    )
    
    def _openurl_changed(self):
        """ Button was clicked """
        from cviewer.action.help import browser_open
        browser_open(url=self.xlink,decorated=True)
    