""" Helper dialog to change edge attribute or thresholding """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library import
from enthought.traits.api import HasTraits, Str, Color, Button, Bool, Int, Instance, List, Float, Any, Enum, Range, DictStrList
from enthought.traits.ui.api import View, Item, Group, InstanceEditor, Handler, Tabbed

# ConnectomeViewer imports
from cviewer.plugins.cff.interfaces.i_network import INetwork
import numpy as np

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class CustomHandler(Handler):
    """ Handler used to set NetworkName in TraitsTitle """
    
    def object_network_reference_changed(self, info):
        if not info.initialized:
            info.ui.title = info.object.network_reference.networkname + ' - Edge Visualization Parameter'
            
class ParaSet(HasTraits):
    
    name = Str
    
    show_legend = Bool(False)
    
    default_range = Bool(True)

    def __init__(self, name, **traits):
        super(HasTraits, self).__init__(**traits)
        self.name = name

    absolute_group = Group(
        Item('abs_lower', label = 'Lower threshold'),
        Item('abs_upper', label = 'Upper threshold'),
        label = 'Absolute Threshold'
    )
    
    proportional_group = Group(
        Item('prop_percentage', label = 'Percentage', style = 'simple',),
        Item('prop_uporlow', label = 'Show portion'),
        label = 'Proportional Threshold'
    )
    
    counting_group = Group(
        Item('count_number', label = 'Number of edges', style = 'simple',),
        Item('count_uporlow', label = 'Show portion'),
        label = 'Counting Threshold'
    )
    
    legend_group = Group(
        Item('show_legend', label = 'Show Legend'),
        Item('default_range', label = 'Default Range'),
        label = 'Legend'
    )

    traits_view = View(Tabbed(
                        absolute_group,
                        proportional_group,
                        counting_group,
                        layout='tabbed'
                        ),
                       Item('_'),
                       legend_group,
                    )
    
    def update_prop_percentage(self, old, new):
        cut_ele = int(round(self.edges_size * new / 100.))
        if cut_ele == 0:
            cut_ele = 1
        
        if self.prop_uporlow == 'lower':
            self.abs_lower = self.edges_min
            self.abs_upper = self.edges[cut_ele-1]
        else:
            self.abs_lower = self.edges[cut_ele-1]
            self.abs_upper = self.edges_max
    
    def update_prop_uporlow(self, old, new):
        # how to notify easier? FIXME
        self.update_prop_percentage(self.prop_percentage, self.prop_percentage)
        
    def update_count_uporlow(self, old, new):
        self.update_count_number(self.count_number, self.count_number)
    
    def update_count_number(self, old, new):
        if self.count_uporlow == 'lower':
            self.abs_lower = self.edges_min
            self.abs_upper = self.edges[self.count_number-1]
        else:
            self.abs_lower = self.edges[self.count_number-1]
            self.abs_upper = self.edges_max
        

class EdgeParameters(HasTraits):
    """ Represents the parameter for simple changes of edge parameters for the
    visualization.
    """
    
    # the reference to the network this dialog manages
    network_reference = Instance(INetwork)
    
    # the enumerated edge keys
    edge_attribute_keys = Any
    
    # selected ParameterSet
    parameterset = Instance(ParaSet)
    
    # a list of possible parameter sets
    all_parasets = List(ParaSet)
    
    def __init__(self, network_ref, current_selection_attr = None, **traits):
        super(HasTraits, self).__init__(**traits)
        
        self.network_reference = network_ref

        # this is how it would go to set an enum upon input of a dynamcial list
        #self.add_trait('edgeattr', Enum(network_ref))
        
        # get the reference to the dict with the edge data
        self.edge_attribute_keys = self.network_reference.datasourcemanager._srcobj.scalarsdict
        
        # initialize enum of edge attributes
        for k, v in self.edge_attribute_keys.items():
            # create parameter element
            # setting up correctly with the available data (threshold min/max)
            myele = ParaSet(k)
            
            # now setting up all the threshold parameters for each edge attribute
            # TODO: this copies all the edge arrays to have a sorted array
            # maybe this can be done more efficiently, but how?
            myele.edges = v.copy()
            myele.edges.sort()
            myele.edges_max = myele.edges.max()
            myele.edges_min = myele.edges.min()
            myele.edges_size = len(myele.edges)
            
            myele.add_trait('abs_lower', Range(value=-1.0e20,
                            low='edges_min',
                            high='edges_max',
                            enter_set=True,
                            auto_set=False,))
            myele.add_trait('abs_upper', Range(value=1.0e20,
                            low='edges_min',
                            high='edges_max',
                            enter_set=True,
                            auto_set=False,))            
            
            myele.add_trait('prop_uporlow', Enum('upper', 'lower'))
            myele.add_trait('prop_percentage', Range(0, 100))
            myele.add_trait('count_uporlow', Enum('upper', 'lower'))
            myele.add_trait('count_number', Range(value=1,
                                                  low=1,
                                                  high=int(myele.edges_size)))

            # on trait changes
            myele.on_trait_change(self.update_absolute_thr_lower, 'abs_lower')
            myele.on_trait_change(self.update_absolute_thr_upper, 'abs_upper')
            
            myele.on_trait_change(myele.update_prop_percentage, 'prop_percentage')
            myele.on_trait_change(myele.update_prop_uporlow, 'prop_uporlow')
            
            myele.on_trait_change(myele.update_count_uporlow, 'count_uporlow')
            myele.on_trait_change(myele.update_count_number, 'count_number')
            
            myele.on_trait_change(self.update_show_legend, 'show_legend')
            myele.on_trait_change(self.update_default_range, 'default_range')
            
            if k == current_selection_attr:
                default_ele = myele
                
            self.all_parasets.append(myele)
            
        # default selection
        if current_selection_attr == None:
            self.parameterset = self.all_parasets[0]
        else:
            self.parameterset = default_ele
            self.parameterset.abs_lower = float(self.network_reference.rendermanager.thres.lower_threshold)
            self.parameterset.abs_upper = float(self.network_reference.rendermanager.thres.upper_threshold)
        
    edge_para_group = Group(
        Item('parameterset',
                              editor =
                                  InstanceEditor( name = 'all_parasets',
                                                  editable = True),
                              style = 'custom',
                              show_label = False
                             ),
        Item('_'),
        label = 'Select Edge Attribute'
    )
    
    traits_view = View(
                        edge_para_group,
                        buttons = ['OK'],
                        width     = 600,
                        height    = 400,
                        resizable = True,
                        handler   = CustomHandler(),
                        title     = 'Edge Visualization Parameters',
                    )

    def set_to_edge_parameter(self, edge_para):
        """ Sets the parameterset to the given edge_para string when found """
        for ele in self.all_parasets:
            if ele.name == edge_para:
                self.parameterset = ele
                
                # setting the weight key of the graph to the current attribute
                #self.network_reference.set_weight_key(edge_para)

    def _parameterset_changed(self, old, new):
        # update the parameter set in store
        # set the data correctly in the visualization
        self.network_reference.rendermanager.attract.point_scalars_name = new.name
        
        # update the threshold values
        self.network_reference.rendermanager.thres.lower_threshold = new.abs_lower
        self.network_reference.rendermanager.thres.upper_threshold = new.abs_upper
        
        # update the show legend / default range values
        self.update_show_legend(old = new.show_legend, new = new.show_legend)
        self.update_default_range(old = new.default_range, new = new.default_range)
        
        # setting the weight key of the graph to the current attribute
        self.network_reference.set_weight_key(new.name)
        
    def update_show_legend(self, old, new):
        self.network_reference.rendermanager.vectors.parent.scalar_lut_manager.show_legend = new

    def update_default_range(self, old, new):
        if new:
            self.network_reference.rendermanager.vectors.parent.scalar_lut_manager.use_default_range = new
        else:
            self.network_reference.rendermanager.vectors.parent.scalar_lut_manager.use_default_range = new
            from numpy import array
            na = array([self.parameterset.edges_min, self.parameterset.edges_max])
            self.network_reference.rendermanager.vectors.parent.scalar_lut_manager.data_range = na

    def update_absolute_thr_lower(self, old, new):
        #logger.debug('Setting lower to new value ' + str(new))
        self.network_reference.rendermanager.thres.lower_threshold = new
        
    def update_absolute_thr_upper(self, old, new):
        #logger.debug('Setting upper to new value ' + str(new))
        self.network_reference.rendermanager.thres.upper_threshold = new

        
