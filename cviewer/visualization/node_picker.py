"""NodePicker class implementing the picking logic

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

# Enthought library imports
from enthought.traits.api import Trait, Instance, Array, Long, Float
from enthought.traits.ui.api import View, Item, Group

# TVTK library imports
from enthought.tvtk.tvtk_base import false_bool_trait
from enthought.tvtk.pyface.picker import Picker, PickHandler

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class NodePickHandler(PickHandler):
    """ Subclasses from the default picker """

    ID = Trait(None, None, Long, desc='the picked ID')
    
    coordinate = Trait(None, None, Array('d', (3,)),
                       desc='the coordinate of the picked point')

    scalar = Trait(None, None, Float, desc='the scalar at picked point')
    
    default_view = View(Item(name='coordinate', label='Coordinates (X,Y,Z):', style='readonly'),
                        #Item(name='scalar', label='Node ID',  style='readonly'),
                        )
                                
    def __init__(self, **traits):
        super(NodePickHandler, self).__init__(**traits)
        self.data = {'ID':[], 'coordinate':[], 'scalar':[]}
        
    def handle_pick(self, data):
        """Called when a pick event happens.
    
        Parameters
        ----------
        data : `PickedData` instance.
        
        """

        # Basically extracted from the Mayavi code
        if data.valid_:
            if data.point_id > -1:
                self.ID = data.point_id
            elif data.cell_id > -1:
                self.ID = data.cell_id
            self.coordinate = data.coordinate

            if data.data:
                array_data = {'scalar': data.data.scalars}
            else:
                array_data = {'scalar': None}
                
            for name in array_data.keys():
                if array_data[name]:
                    setattr(self, name, array_data[name][self.ID])
                else:
                    setattr(self, name, None)
        else:
            for name in ['ID', 'coordinate', 'scalar']:
                setattr(self, name, None)


class NodePicker(Picker):
    """ NodePicker class handles picking of the nodes """

    # overwrite the default pickhandler by my own
    pick_handler = Trait(NodePickHandler(), Instance(PickHandler))
    
    # has mouse moved?
    mouse_moved = False
    
    # double clicked, True after second click without movement
    double = False
    
    # show info box
    show_info = Trait(false_bool_trait)
    
    # toggle surface
    toggle_selection = Trait(false_bool_trait)
    
    # select node with key press '1'
    select_node = Trait(false_bool_trait)
    
    # select node with key press '2' for neighbourhood
    select_node2 = Trait(false_bool_trait)
    
    # toggle node label with 'g'
    toggle_label = Trait(false_bool_trait)
    
    default_view = View(Group(Group(Item(name='pick_handler', style='custom'),
                                    show_border=True, show_labels=False),
                              #Group(Item(name='show_gui'),
                              #      Item(name='auto_raise'), show_border=True),
                              ),
                        resizable=True,
                        buttons=['OK'])
        
    def __init__(self, renwin, **traits):
        super(NodePicker, self).__init__(renwin, **traits)
        
        self.renwin = renwin

    def key_pressed(self, vtk_interactor, event):
        
        key = vtk_interactor.GetKeyCode()
              

        #if event == 'KeyPressEvent':
            #keycode = vtk_interactor.GetKeyCode()
            #control_key = vtk_interactor.GetControlKey()
            #shift_key = vtk_interactor.GetShiftKey()
            #alt_key = vtk_interactor.GetAltKey() # seems to not work :-(

        self.select_node2 = False
        self.select_node = False
        self.toggle_selection = False
        self.show_info = False
        self.toggle_label = False
        
        if key == '1':
            self.select_node = True
        
        elif key == '2':
            self.select_node2 = True
            
        elif key.lower() in ['t']:
            self.toggle_selection = True
            
        elif key.lower() in ['i']:
            self.show_info = True
        
        elif key.lower() in ['g']:
            self.toggle_label = True
            
        else:
            return
            
        x, y = vtk_interactor.GetEventPosition()
        data = self.pick_point(x, y)
        self.pick_handler.handle_pick(data)
        return


    def on_mouse_move(self, obj, evt):
        self.mouse_moved = True
        self.double = False
        self.show_info = False
        self.toggle_selection = False
        self.select_node = False
        self.select_node2 = False
        self.toggle_label = False


