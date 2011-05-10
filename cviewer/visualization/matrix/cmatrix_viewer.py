""" Connectome Matrix Viewer
* Left-drag pans the plot
* Mousewheel up and down zooms the plot in and out
* Pressing "z" brings up the Zoom Box, and you can click-drag a rectangular 
   region to zoom.  If you use a sequence of zoom boxes, pressing alt-left-arrow
   and alt-right-arrow moves you forwards and backwards through the "zoom 
   history".
"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License


# Major library imports
from enthought.enable.api import BaseTool

# Enthought library imports
from enthought.enable.api import Component, ComponentEditor, Window
from enthought.traits.api import HasTraits, Instance, Str, Enum, Float, Int, Any
from enthought.traits.ui.api import Item, Group, View, HGroup, Handler

# Chaco imports
from enthought.chaco.api import ArrayPlotData, ColorBar, HPlotContainer, jet, LinearMapper, Plot
from enthought.chaco.tools.api import PanTool, RangeSelection, RangeSelectionOverlay, ZoomTool

# ConnectomeViewer imports
from cviewer.plugins.cff.ui.edge_parameters_view import EdgeParameters
from cviewer.visualization.matrix.matrix_viewer import MatrixViewer

class CustomHandler(Handler):
    """ Handler used to set NetworkName in TraitsTitle """
    
    def object_network_reference_changed(self, info):
        if not info.initialized:
            info.ui.title = info.object.network_reference.networkname + ' - Matrix Viewer'
            
class CustomTool(BaseTool):
    
    #event_state = Enum("normal", "mousedown")
    xval = Float
    yval = Float
    
    def normal_mouse_move(self, event):
        xval, yval = self.component.map_data((event.x, event.y))
        
        self.xval = xval
        self.yval = yval
    
    #def normal_left_down(self, event):
    #    self.event_state = "mousedown"
    #    event.handled = True
    #
    #def mousedown_mouse_move(self, event):
    #    print "Data:", self.component.map_data((event.x, event.y))
    #    
    #def mousedown_left_up(self, event):
    #    self.event_state = "normal"
    #    event.handled = True

class CMatrixViewer(MatrixViewer):
    
    tplot = Instance(Plot)
    plot = Instance(Component)
    custtool = Instance(CustomTool)
    colorbar = Instance(ColorBar)
    
    edge_parameter = Instance(EdgeParameters)
    network_reference = Any
    matrix_data_ref = Any
    labels = Any
    fro = Any
    to = Any
    val = Float

    traits_view = View(
                    Group(
                        Item('plot', editor=ComponentEditor(size=(800,600)),
                             show_label=False),
                        HGroup(
                        Item('fro', label="From", style = 'readonly', springy=True),
                        Item('to', label="To", style = 'readonly', springy=True),
                        Item('val', label="Value", style = 'readonly', springy=True),
                        ),
                        orientation = "vertical"),
                    Item('edge_parameter_name', label="Choose edge"),
                    handler=CustomHandler(),
                    resizable=True, title="Matrix Viewer"
                    )

    
    def __init__(self, net_ref, **traits):
        """ net_ref is a reference to a cnetwork """
        super(MatrixViewer, self).__init__(**traits)
        
        self.network_reference = net_ref
        self.edge_parameter = self.network_reference._edge_para
        self.matrix_data_ref = self.network_reference.datasourcemanager._srcobj.edgeattributes_matrix_dict
        self.labels = self.network_reference.datasourcemanager._srcobj.labels
        
        # get the currently selected edge
        self.curr_edge = self.edge_parameter.parameterset.name
        # create plot
        self.plot = self._create_plot_component()
        
        # set trait notification on customtool
        self.custtool.on_trait_change(self._update_fields, "xval")
        self.custtool.on_trait_change(self._update_fields, "yval")
        
        # add edge parameter enum
        self.add_trait('edge_parameter_name', Enum(self.matrix_data_ref.keys()))
        self.edge_parameter_name = self.curr_edge
        
    def _edge_parameter_name_changed(self, new):
        # update edge parameter dialog
        self.edge_parameter.set_to_edge_parameter(self.edge_parameter_name)
        
        # update the data
        self.pd.set_data("imagedata", self.matrix_data_ref[self.edge_parameter_name])
        
        # set range
        #self.my_plot.set_value_selection((0.0, 1.0))
        
    def _update_fields(self):
        from numpy import trunc
        
        # map mouse location to array index
        frotmp = int(trunc(self.custtool.yval))
        totmp = int(trunc(self.custtool.xval))
        
        # check if within range
        sh = self.matrix_data_ref[self.edge_parameter_name].shape
        # assume matrix whose shape is (# of rows, # of columns)
        if frotmp >= 0 and frotmp < sh[0] and totmp >= 0 and totmp < sh[1]:
            self.fro = self.labels[frotmp]
            self.to = self.labels[totmp]
            self.val = self.matrix_data_ref[self.edge_parameter_name][frotmp, totmp]
        
    def _create_plot_component(self):
        
        # we need the matrices!
        # start with the currently selected one
        #nr_nodes = self.matrix_data_ref[curr_edge].shape[0]
        
        # Create a plot data obect and give it this data
        self.pd = ArrayPlotData()
        self.pd.set_data("imagedata", self.matrix_data_ref[self.curr_edge])
    
        # Create the plot
        self.tplot = Plot(self.pd, default_origin="top left")
        self.tplot.x_axis.orientation = "top"
        self.tplot.img_plot("imagedata", 
                      name="my_plot",
                      #xbounds=(0,nr_nodes),
                      #ybounds=(0,nr_nodes),
                      colormap=jet)
    
        # Tweak some of the plot properties
        self.tplot.title = self.curr_edge
        self.tplot.padding = 50
    
        # Right now, some of the tools are a little invasive, and we need the 
        # actual CMapImage object to give to them
        self.my_plot = self.tplot.plots["my_plot"][0]
    
        # Attach some tools to the plot
        self.tplot.tools.append(PanTool(self.tplot))
        zoom = ZoomTool(component=self.tplot, tool_mode="box", always_on=False)
        self.tplot.overlays.append(zoom)
        
        # my custom tool to get the connection information
        self.custtool = CustomTool(self.tplot)
        self.tplot.tools.append(self.custtool)
    
        # Create the colorbar, handing in the appropriate range and colormap
        colormap = self.my_plot.color_mapper
        self.colorbar = ColorBar(index_mapper=LinearMapper(range=colormap.range),
                            color_mapper=colormap,
                            plot=self.my_plot,
                            orientation='v',
                            resizable='v',
                            width=30,
                            padding=20)
            
        self.colorbar.padding_top = self.tplot.padding_top
        self.colorbar.padding_bottom = self.tplot.padding_bottom
        
        # TODO: the range selection gives a Segmentation Fault,
        # but why, the matrix_viewer.py example works just fine!
        # create a range selection for the colorbar
        self.range_selection = RangeSelection(component=self.colorbar)
        self.colorbar.tools.append(self.range_selection)
        self.colorbar.overlays.append(RangeSelectionOverlay(component=self.colorbar,
                                                       border_color="white",
                                                       alpha=0.8,
                                                       fill_color="lightgray"))
    
        # we also want to the range selection to inform the cmap plot of
        # the selection, so set that up as well
        #self.range_selection.listeners.append(self.my_plot)
    
        # Create a container to position the plot and the colorbar side-by-side
        container = HPlotContainer(use_backbuffer = True)
        container.add(self.tplot)
        container.add(self.colorbar)
        container.bgcolor = "white"
    
        # my_plot.set_value_selection((-1.3, 6.9))
    
        return container




