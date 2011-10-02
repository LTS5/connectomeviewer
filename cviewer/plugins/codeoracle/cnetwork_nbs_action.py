from traits.api import Code, Button, Int, on_trait_change, Any, HasTraits,List, Str, Enum, Instance, Bool, Property, Float
from traitsui.api import (View, Item, Group, HGroup, CodeEditor,
                                     spring, Handler, EnumEditor)
from traitsui.api \
    import View, HGroup, Item, TabularEditor
    
from traitsui.tabular_adapter \
    import TabularAdapter

class MultiSelectAdapter ( TabularAdapter ):

    columns = [ ( 'Value', 'value' ) ]

    value_text = Property

    def _get_value_text ( self ):
        return self.item

class NBSMoreParameter ( HasTraits ):

    
    THRES = Float(3)
    K = Int(10)
    TAIL = Enum('left', ['left', 'equal', 'right'])

    view = View(
             Item('first_edge_value', label = "Edge value for first group"),
             Item('second_edge_value', label = "Edge value for second group"),
             Item('THRES', label = "Threshold"),
             Item('K', label="# Interations"),
             Item('TAIL', label="Tail"),
             buttons=['OK'], 
             resizable=True,
             title = "More parameters"
             )

    def __init__(self, cfile, selected_network1, selected_network2, **traits):
        super(NBSMoreParameter, self).__init__(**traits)  
        
        for cobj in cfile.connectome_network:
            if cobj.name == selected_network1:
                cnet1 = cobj
                break
        # get edge parameters
        edval1 = cnet1._get_edge_values()

        for cobj in cfile.connectome_network:
            if cobj.name == selected_network2:
                cnet2 = cobj
                break
        # get edge parameters
        edval2 = cnet2._get_edge_values()

        self.add_trait('first_edge_value',  Enum(edval1) )
        self.add_trait('second_edge_value',  Enum(edval2) )
        
        

class NBSNetworkParameter ( HasTraits ):
    
    choices1  = List( Str )
    selected1 = List( Str )

    choices2  = List( Str )
    selected2 = List( Str )
    
    view = View(
        
        HGroup(
        Group(
            HGroup(
                Item( 'choices1',
                      show_label = False,
                      editor     = TabularEditor(
                                       show_titles  = False,
                                       selected     = 'selected1',
                                       editable     = False,
                                       multi_select = True,
                                       adapter      = MultiSelectAdapter() )
                ),
                Item( 'selected1',
                      show_label = False,
                      editor     = TabularEditor(
                                       show_titles  = False,
                                       editable     = False,
                                       adapter      = MultiSelectAdapter() )
                ),
            ),
            label = "First group"
            ),
        Group(
            HGroup(
                Item( 'choices2',
                      show_label = False,
                      editor     = TabularEditor(
                                       show_titles  = False,
                                       selected     = 'selected2',
                                       editable     = False,
                                       multi_select = True,
                                       adapter      = MultiSelectAdapter() )
                ),
                Item( 'selected2',
                      show_label = False,
                      editor     = TabularEditor(
                                       show_titles  = False,
                                       editable     = False,
                                       adapter      = MultiSelectAdapter() )
                ),
            ),
            label = "Second group"
        ),
        
        ),
        
        title = 'Select networks',
        width = 0.5,
        height = 0.5,
        buttons = ['OK'], 
        resizable=True,
    )
    
    def __init__(self, cfile, **traits):
        super(NBSNetworkParameter, self).__init__(**traits)
        
        li = []
        for cobj in cfile.connectome_network:
            li.append(cobj.obj.name)
            
        self.choices1 = li
        self.choices2 = li
        
        
        
if __name__ == '__main__':
    demo.configure_traits()