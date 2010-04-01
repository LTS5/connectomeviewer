""" Interfacing the TrackVis application http://trackvis.org/
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
import os
import time
from threading import Thread

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

def generate_scene_file(trackfname, volumefname, selectiongraph, \
                        scenefname='test.scene', voxeldim = [212,212,102], \
                        voxelsize = [1, 1, 1]):
    """ Generate a .scene file containing a trackgroup with ROIs for TrackVis
    
    Parameters
    ----------
    trackfname : string
        full path to .trk file
    volumefname : string
        full path to (temporary) image file (nifti)
    selectiongraph : `Graph` NetworkX instance
        networkx graph with inforation on the nodes for ROI generation
    scenefname : string
        full path to (temporary) opened ASCII file
    voxeldim : array_like
        voxel dimension of the .trk file
    voxelsize : array_like
        voxel size of the .trk file
    
    """
    logger.debug('Started generating scene file for TrackVis...')
    from xml.dom.minidom import Document
    # hints: http://www.postneo.com/projects/pyxml/
    
    # timestamp for simple unique ID generation
    stampid = int(time.time())
    
    # Create the minidom document
    doc = Document()
    
    # Create the <Scene> base element
    scene = doc.createElement("Scene")
    scene.setAttribute("version", "2.2")
    doc.appendChild(scene)
    
    # create <Dimension>
    dimension = doc.createElement("Dimension")
    dimension.setAttribute("x", "212")
    dimension.setAttribute("y", "212")
    dimension.setAttribute("z", "102")
    scene.appendChild(dimension)
    
    # create <VoxelSize>
    voxelsize = doc.createElement("VoxelSize")
    voxelsize.setAttribute("x", "1")
    voxelsize.setAttribute("y", "1")
    voxelsize.setAttribute("z", "1")
    scene.appendChild(voxelsize)
    
    # create <VoxelOrder>
    voxelorder = doc.createElement("VoxelOrder")
    voxelorder.setAttribute("current", "LPS")
    voxelorder.setAttribute("original", "LPS")
    scene.appendChild(voxelorder)
    
    # create <LengthUnit>
    lengthunit = doc.createElement("LengthUnit")
    lengthunit.setAttribute("value", "1")
    scene.appendChild(lengthunit)
    
    # create <TrackFile>
    trackfile = doc.createElement("TrackFile")
    trackfile.setAttribute("path", trackfname)
    trackfile.setAttribute("rpath", os.path.split(trackfname)[1])
    scene.appendChild(trackfile)
    
    # create <Image>
    image = doc.createElement("Image")
    scene.appendChild(image)
    
    slicex = doc.createElement("SliceX")
    slicex.setAttribute("number", "0")
    slicex.setAttribute("plane", "0")
    slicex.setAttribute("visibility", "0")
    image.appendChild(slicex)

    slicey = doc.createElement("SliceY")
    slicey.setAttribute("number", "0")
    slicey.setAttribute("plane", "1")
    slicey.setAttribute("visibility", "0")
    image.appendChild(slicey)
    
    slicez = doc.createElement("SliceZ")
    slicez.setAttribute("number", "0")
    slicez.setAttribute("plane", "2")
    slicez.setAttribute("visibility", "0")
    image.appendChild(slicez)

    interpol = doc.createElement("Interpolate")
    interpol.setAttribute("value", "0")
    image.appendChild(interpol)

    opacity = doc.createElement("Opacity")
    opacity.setAttribute("value", "0.99")
    image.appendChild(opacity)

    map = doc.createElement("Map")
    #map.setAttribute("window", "33.2")
    #map.setAttribute("level", "15.8245")
    map.setAttribute("path", volumefname)
    map.setAttribute("rpath", os.path.split(volumefname)[1])
    image.appendChild(map)

    curval = doc.createElement("CurrentIndex")
    curval.setAttribute("value", "0")
    image.appendChild(curval)
    
    # create <ROIs>
    rois = doc.createElement("ROIs")
    scene.appendChild(rois)
    
    currentindex = doc.createElement("CurrentIndex")
    currentindex.setAttribute("value", "0")
    rois.appendChild(currentindex)
    
    myids = []
    # create <ROI> with iteration over nodes
    for node in selectiongraph.nodes_iter(data=True):

        iv = node[1]['dn_intensityvalue']

        roi = doc.createElement("ROI")
        roi.setAttribute("name", node[1]['dn_label'])
        roi.setAttribute("type", "FromImage")

        # create an unique id from a timestamp!
        curid = str(stampid + int(iv))
        myids.append(curid)
        roi.setAttribute("id", curid)
        rois.appendChild(roi)
        
        imagefile = doc.createElement("ImageFile")
        imagefile.setAttribute("path", volumefname)
        imagefile.setAttribute("rpath", os.path.split(volumefname)[1])
        imagefile.setAttribute("low", str(iv))
        imagefile.setAttribute("high", str(iv))
        roi.appendChild(imagefile)
        
        edited = doc.createElement("Edited")
        edited.setAttribute("value", "0")
        roi.appendChild(edited)
        
        # convert rgb to tuple
        e = node[1]['dn_colorcode'].replace(' ', '')
        e = e.split(',')
        r = int(e[0])
        g = int(e[1])
        b = int(e[2])
                        
        # TODO: if len != 3 skip
        color = doc.createElement("Color")
        color.setAttribute("r", str(r))
        color.setAttribute("g", str(g))
        color.setAttribute("b", str(b))
        roi.appendChild(color)
        
        opacity = doc.createElement("Opacity")
        opacity.setAttribute("value", "1")
        roi.appendChild(opacity)
        
        visibility = doc.createElement("Visibility")
        visibility.setAttribute("value", "1")
        roi.appendChild(visibility)
        
        #annotationposition = doc.createElement("AnnotationPosition")
        #annotationposition.setAttribute("x", "106.5")
        #annotationposition.setAttribute("y", "90.5")
        #annotationposition.setAttribute("z", "43.5")
        #roi.appendChild(annotationposition)
    
    # create <Tracks>
    tracks = doc.createElement("Tracks")
    scene.appendChild(tracks)
    
    currentindex2 = doc.createElement("CurrentIndex")
    currentindex2.setAttribute("value", "0")
    tracks.appendChild(currentindex2)
    
    # create <Track> -> to fill with ROIs
    track = doc.createElement("Track")
    track.setAttribute("name", "Track 1")
    # create unique id from timestamp
    track.setAttribute("id", str(stampid))
    tracks.appendChild(track)
    
    length = doc.createElement("Length")
    length.setAttribute("low", "0")
    length.setAttribute("high", "1e+08")
    track.appendChild(length)
    
    for i in range(3):
        slice = doc.createElement("Slice")
        slice.setAttribute("plane", "0")
        slice.setAttribute("number", "106")
        slice.setAttribute("thickness", "1")
        slice.setAttribute("testmode", "0")
        slice.setAttribute("enable", "0")
        slice.setAttribute("visible", "1")
        slice.setAttribute("id", "1724121186"+str(i))
        track.appendChild(slice)
    
    # add custom defined <ROI>, correct ids, with operator!
    for myid in myids:
        roi = doc.createElement("ROI")
        roi.setAttribute("id", str(myid))
        roi.setAttribute("testmode", "0")
        roi.setAttribute("enable", "1")
        roi.setAttribute("operator", "Or")
        track.appendChild(roi)
    
    skip = doc.createElement("Skip")
    skip.setAttribute("value", "0.340735")
    skip.setAttribute("enable", "1")
    track.appendChild(skip)
    
    shadingmode = doc.createElement("ShadinMode")
    shadingmode.setAttribute("value", "0")
    track.appendChild(shadingmode)
    
    radius = doc.createElement("Radius")
    radius.setAttribute("value", "0.05")
    track.appendChild(radius)
    
    nrofsides = doc.createElement("NumberOfSides")
    nrofsides.setAttribute("value", "5")
    track.appendChild(nrofsides)
    
    colorcode = doc.createElement("ColorCode")
    colorcode.setAttribute("value", "0")
    track.appendChild(colorcode)
    
    solidcolor = doc.createElement("SolidColor")
    solidcolor.setAttribute("r", "255")
    solidcolor.setAttribute("g", "255")
    solidcolor.setAttribute("b", "0")
    track.appendChild(solidcolor)
    
    scalarindex = doc.createElement("ScalarIndex")
    scalarindex.setAttribute("value", "0")
    track.appendChild(scalarindex)
    
    scalargradient = doc.createElement("ScalarGradient")
    track.appendChild(scalargradient)
    
    colorstop = doc.createElement("ColorStop")
    colorstop.setAttribute("stop", "0")
    colorstop.setAttribute("r", "1")
    colorstop.setAttribute("g", "1")
    colorstop.setAttribute("b", "0")
    scalargradient.appendChild(colorstop)
    
    colorstop = doc.createElement("ColorStop")
    colorstop.setAttribute("stop", "1")
    colorstop.setAttribute("r", "1")
    colorstop.setAttribute("g", "0")
    colorstop.setAttribute("b", "0")
    scalargradient.appendChild(colorstop)
    
    saturation = doc.createElement("Saturation")
    saturation.setAttribute("value", "1")
    track.appendChild(saturation)
    
    helixpoint = doc.createElement("HelixPoint")
    helixpoint.setAttribute("x", "106")
    helixpoint.setAttribute("y", "106")
    helixpoint.setAttribute("z", "51")
    track.appendChild(helixpoint)
    
    helixvector = doc.createElement("HelixVector")
    helixvector.setAttribute("x", "1")
    helixvector.setAttribute("y", "0")
    helixvector.setAttribute("z", "0")
    track.appendChild(helixvector)
    
    helixaxis = doc.createElement("HelixAxis")
    helixaxis.setAttribute("visibility", "1")
    track.appendChild(helixaxis)
    
    visibility = doc.createElement("Visibility")
    visibility.setAttribute("value", "1")
    track.appendChild(visibility)
    
    annotationposition = doc.createElement("AnnotationPosition")
    annotationposition.setAttribute("x", "130.57")
    annotationposition.setAttribute("y", "101.185")
    annotationposition.setAttribute("z", "21.655")
    track.appendChild(annotationposition)
    
    # create <LookUpTable>
    lookup = doc.createElement("LookUpTable")
    scene.appendChild(lookup)
    
    dirscheme = doc.createElement("DirectionScheme")
    dirscheme.setAttribute("value", "0")
    lookup.appendChild(dirscheme)
    
    dirvector = doc.createElement("DirectionVector")
    dirvector.setAttribute("value", "0")
    lookup.appendChild(dirvector)
    
    # create <Coordinate>
    coordinate = doc.createElement("Coordinate")
    scene.appendChild(coordinate)
    
    point000 = doc.createElement("Point000")
    point000.setAttribute("x", "0")
    point000.setAttribute("y", "0")
    point000.setAttribute("z", "0")
    point000.setAttribute("id", "-1")
    coordinate.appendChild(point000)
    
    point100 = doc.createElement("Point000")
    point100.setAttribute("x", "0")
    point100.setAttribute("y", "0")
    point100.setAttribute("z", "0")
    point100.setAttribute("id", "-1")
    coordinate.appendChild(point100)
    
    # ?? <Point##0 x="0" y="1" z="0" id="-1" />
    
    annotationtype = doc.createElement("Annotation")
    annotationtype.setAttribute("type", "2")
    coordinate.appendChild(annotationtype)
    
    # create <Camera>
    camera = doc.createElement("Camera")
    scene.appendChild(camera)
    
    position = doc.createElement("Position")
    position.setAttribute("x", "106")
    position.setAttribute("y", "-340.597")
    position.setAttribute("z", "51")
    camera.appendChild(position)
    
    focalpoint = doc.createElement("FocalPoint")
    focalpoint.setAttribute("x", "106")
    focalpoint.setAttribute("y", "106")
    focalpoint.setAttribute("z", "51")
    camera.appendChild(focalpoint)
    
    viewup = doc.createElement("ViewUp")
    viewup.setAttribute("x", "0")
    viewup.setAttribute("y", "0")
    viewup.setAttribute("z", "1")
    camera.appendChild(viewup)
    
    viewangle = doc.createElement("ViewAngle")
    viewangle.setAttribute("value", "30")
    camera.appendChild(viewangle)
    
    clippingrange = doc.createElement("ClippingRange")
    clippingrange.setAttribute("near", "381.899")
    clippingrange.setAttribute("far", "516.889")
    camera.appendChild(clippingrange)
    
    # create <ObjectAnnotation>
    objannotation = doc.createElement("ObjectAnnotation")
    objannotation.setAttribute("value", "1")
    scene.appendChild(objannotation)
    
    # create <BackgroundColor>
    backgroundcolor = doc.createElement("BackgroundColor")
    backgroundcolor.setAttribute("r", "0")
    backgroundcolor.setAttribute("g", "0")
    backgroundcolor.setAttribute("b", "0")
    scene.appendChild(backgroundcolor)
    
    # Print our newly created XML
    #print doc.toprettyxml(indent="  ")
    logger.debug('Generated XML in '+ str(scenefname))
    
    # output to file
    f = open(scenefname, 'w')
    f.write(doc.toprettyxml(indent="	"))
    f.close()
    logger.debug('File '+ scenefname +' closed again.')


class ThreadedTrackvis(Thread):
    """ Open an instance of the TrackVis application in a new thread """
    
    def __init__(self, tvpath, fname, trkfname, volfname, **kwargs):
        Thread.__init__(self, **kwargs)
        self.tvpath = tvpath
        self.fname = fname
        self.trkfname = trkfname
        self.volfname = volfname

    def run(self):
        logger.debug('Invoking TrackVis in a Thread.')
        
        from subprocess import call
        import os.path
        if os.path.exists(self.tvpath):
            command = os.path.join(self.tvpath, "trackvis")
            if os.path.isfile(command):
                logger.debug('Invoking command: ' + str(command) + " " + self.fname)
                try:
                    retcode = call(os.path.join(self.tvpath, "trackvis") + " " + self.fname, shell=True)
                    if retcode < 0:
                        logger.debug("Trackvis Thread was terminated by signal " + str(retcode))
                    else:
                        logger.debug("Trackvis Thread returned with returncode " + str(retcode))
                        
                    logger.debug('Removing unused files from temporary folder.')
                    os.unlink(self.fname)
                    os.unlink(self.trkfname)
                    os.unlink(self.volfname)
                    
                except OSError, e:
                    logger.error("Execution of trackvis failed:" + e)
            else:
                from enthought.traits.ui.api import message
                message(message = 'In the given Trackvis path in the preference, Trackvis executable not found.', \
                        title = "Error")
        else:
            from enthought.traits.ui.api import message
            message(message = 'You have to choose the path to the Trackvis executable in the preferences!', \
                    title = "Error")
