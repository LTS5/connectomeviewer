import igraph
from enthought.tvtk.api import tvtk
from enthought.mayavi import mlab
from vtk.util import colors
import numpy as numpy
import divideGraph as divideGraph

def show_nodes(filename):
    g = igraph.Graph.Read_Pajek(filename)

    ll = igraph.Graph.layout_kamada_kawai_3d(g)

    es = igraph.EdgeSeq(g)
    vs = igraph.VertexSeq(g)

    numEdges = igraph.Graph.ecount(g)
    numVertices = igraph.Graph.vcount(g)

    degNorm = range(numVertices)
    degree = igraph.Graph.degree(g)

    for i in range(0, numVertices):
        degNorm[i] = (10*degree[i])/max(degree)
        
    spheres =[]
    sphereActors =[]
    nodeProperties = []

    lines = []
    lineActors = []
    edgeProperties = []
    tubes = []

    # display 3D window
    v = mlab.figure()

    # display nodes
    for i in range(0, numVertices):
        sphere = tvtk.SphereSource(radius=degNorm[i])
        sphereMapper = tvtk.PolyDataMapper(input = sphere.output)
        nodeProperty = tvtk.Property()
        nodeProperty.set(opacity = 0.3, color=colors.grey)
        sphereActor = tvtk.Actor(mapper=sphereMapper, property=nodeProperty)
        sphereActor.position = ll[i][0], ll[i][1], ll[i][2]
        spheres.append(sphere)
        sphereActors.append(sphereActor)
        nodeProperties.append(nodeProperty)
        v.scene.add_actor(sphereActor)
        # labels
        text = tvtk.VectorText(text = vs[vs.indices[i]]["id"])
        textMapper = tvtk.PolyDataMapper(input = text.output)
        textActor = tvtk.Follower(mapper=textMapper)
        textActor.position = ll[i][0], ll[i][1], ll[i][2]
        textActor.camera = v.scene.camera
        v.scene.add_actor(textActor)
        
    # display edges
    for e in range(0, numEdges):
        source = es[es.indices[e]].tuple[0]
        target = es[es.indices[e]].tuple[1]
        line = tvtk.LineSource(point1=ll[source], point2=ll[target])
        tube = tvtk.TubeFilter(radius=0.05*es[es.indices[e]]["weight"])
        tube.set_input_connection(line.output_port)
        lineMapper = tvtk.PolyDataMapper(input = tube.output)
        color = es[es.indices[e]]["color"]
        if color == "grey":
            edgeProperty = tvtk.Property(color=colors.white)
            lineActor = tvtk.Actor(mapper=lineMapper, property=edgeProperty)
        elif color == "blue":
            #set the link color to blue
            edgeProperty = tvtk.Property(color=colors.blue)
            lineActor = tvtk.Actor(mapper=lineMapper, property=edgeProperty)
        elif color == "green":
            #set link color to green
            edgeProperty = tvtk.Property(color=colors.green)
            lineActor = tvtk.Actor(mapper=lineMapper, property=edgeProperty)
        elif color == "red":
            #set link color to red
            edgeProperty = tvtk.Property(color=colors.red)
            lineActor = tvtk.Actor(mapper=lineMapper, property=edgeProperty)
        else:
            #set link color to yellow
            edgeProperty = tvtk.Property(color=colors.yellow)
            lineActor = tvtk.Actor(mapper=lineMapper, property=edgeProperty)
        tubes.append(tube)
        lines.append(line)
        lineActors.append(lineActors)
        edgeProperties.append(edgeProperty)
        v.scene.add_actor(lineActor)
        
    v.render()
    return(g)
    
