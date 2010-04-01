SET (KIT Graphs)
SET ( Kit_SRCS
vtkIdMap.cxx
vtkStringTable.cxx
vtkIdIterator.cxx
vtkIdArrayIterator.cxx
vtkIdListIterator.cxx
vtkChildIterator.cxx
vtkParentIterator.cxx
vtkGraph.cxx
vtkIndexedSet.cxx
vtkGraphSource.cxx
vtkXGMLReader.cxx
vtkGMLReader.cxx
vtkGMLWriter.cxx
vtkGraphToGraphFilter.cxx
vtkGraphToPolyDataFilter.cxx
vtkForestToTree.cxx
vtkConeLayout.cxx
vtkRadialLayout.cxx
vtkReingoldTilfordLayout.cxx
vtkUseLayout.cxx
vtkGEMLayout.cxx
vtkHDELayout.cxx
vtkApplyLayout.cxx
vtkSpanLayout.cxx
vtkSpanningDAG.cxx
vtkEdgeSpanTree.cxx
vtkEdgeGeometry.cxx
vtkNodeGeometry.cxx
vtkConeGeometry.cxx
vtkDualGeometry.cxx
vtkSubgraphFilter.cxx
vtkSelectOnSubrange.cxx
vtkSelectNthBestNodes.cxx
vtkSelectReachable.cxx
vtkDistanceMetric.cxx
vtkStrahlerMetric.cxx
vtkGraphStrahlerMetric.cxx
vtkEdgeTubeFilter.cxx
vtkLabelMatcher.cxx
vtkGraphOperations.cxx
vtkGraphToUnstructuredGrid.cxx
vtkUnstructuredGridToGraph.cxx
vtkGraphReader.cxx
vtkGraphWriter.cxx
vtkGraphStringMapper.cxx
)

SET_SOURCE_FILES_PROPERTIES(
vtkGraphSource.cxx
vtkGraphToGraphFilter.cxx
vtkGraphToPolyDataFilter.cxx
ABSTRACT
)

SET_SOURCE_FILES_PROPERTIES(
  vtkIndexedSet
WRAP_EXCLUDE
)

INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)


# Setup vtkInstantiator registration for this library's classes.
VTK_MAKE_INSTANTIATOR2(vtk${KIT}Instantiator KitInstantiator_SRCS
                       ${Kit_SRCS}
                       EXPORT_MACRO VTK_EXPORT
                       HEADER_LOCATION ${VTK_BINARY_DIR})

ADD_LIBRARY(vtk${KIT} ${Kit_SRCS} ${KitInstantiator_SRCS})


IF(VTK_APPLE_SHARED_FLAGS_NEEDED)
  SET_TARGET_PROPERTIES(vtk${KIT} PROPERTIES LINK_FLAGS "-init _oft_init${KIT}")
ENDIF(VTK_APPLE_SHARED_FLAGS_NEEDED)

ADD_LIBRARY(vtkGraphs ${Graphs_SRCS} ${GraphsInstantiator_SRCS})

# Allow the user to customize their build with some local options
#
INCLUDE (${VTK_BINARY_DIR}/Graphs/LocalUserOptions.cmake OPTIONAL)
INCLUDE (${VTK_SOURCE_DIR}/Graphs/LocalUserOptions.cmake OPTIONAL)
INCLUDE_DIRECTORIES(${VTK_SOURCE_DIR}/Graphs)

IF (VTK_WRAP_TCL)
  VTK_WRAP_TCL2(vtk${KIT}TCL KitTCL_SRCS ${Kit_SRCS})
  ADD_LIBRARY(vtk${KIT}TCL ${KitTCL_SRCS})
  TARGET_LINK_LIBRARIES (vtk${KIT}TCL vtk${KIT} vtkRenderingTCL vtkIOTCL)
  IF (WIN32)
    IF (VTK_USE_VIDEO_FOR_WINDOWS)
      TARGET_LINK_LIBRARIES (vtk${KIT}TCL vfw32)
    ENDIF (VTK_USE_VIDEO_FOR_WINDOWS)
    IF (VTK_USE_MATROX_IMAGING)
      TARGET_LINK_LIBRARIES (vtk${KIT}TCL ${MIL_LIBRARY})
    ENDIF (VTK_USE_MATROX_IMAGING)
  ENDIF (WIN32)

  INSTALL_TARGETS(/lib/vtk vtk${KIT}TCL)
ENDIF (VTK_WRAP_TCL)

# if we are wrapping into Python then add the library and extra
# source files
#
IF (VTK_WRAP_PYTHON)
  VTK_WRAP_PYTHON(vtkGraphsPython GraphsPython_SRCS ${Graphs_SRCS})
  IF (APPLE)
    ADD_LIBRARY(vtkGraphsPython MODULE ${GraphsPython_SRCS})
  ELSE (APPLE)
    ADD_LIBRARY(vtkGraphsPython SHARED ${GraphsPython_SRCS})
    TARGET_LINK_LIBRARIES (vtkGraphsPython vtkRenderingPython vtkIOPython)
  ENDIF (APPLE)
  TARGET_LINK_LIBRARIES (vtkGraphsPython ${OPENGL_LIBRARY})
  IF(WIN32)
    TARGET_LINK_LIBRARIES (vtkGraphsPython
                           debug ${PYTHON_DEBUG_LIBRARY}
                           optimized ${PYTHON_LIBRARY})
  ENDIF(WIN32)
  TARGET_LINK_LIBRARIES(vtkGraphsPython vtkGraphs)

  IF (WIN32)
    IF (VTK_USE_VIDEO_FOR_WINDOWS)
      TARGET_LINK_LIBRARIES (vtkGraphsPython vfw32)
    ENDIF (VTK_USE_VIDEO_FOR_WINDOWS)
    IF (VTK_USE_MATROX_IMAGING)
      TARGET_LINK_LIBRARIES (vtkGraphsPython ${MIL_LIBRARY})
    ENDIF (VTK_USE_MATROX_IMAGING)
  ENDIF (WIN32)

  INSTALL_TARGETS(/lib/vtk vtkGraphsPython)
ENDIF (VTK_WRAP_PYTHON)

IF (VTK_WRAP_JAVA)
  VTK_WRAP_JAVA(vtkGraphsJava GraphsJava_SRCS ${Graphs_SRCS})
  ADD_LIBRARY(vtkGraphsJava SHARED ${GraphsJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGraphsJava vtkGraphs vtkRenderingJava vtkIOJava)
  IF (WIN32)
    IF (VTK_USE_VIDEO_FOR_WINDOWS)
      TARGET_LINK_LIBRARIES (vtkGraphsJava vfw32)
    ENDIF (VTK_USE_VIDEO_FOR_WINDOWS)
    IF (VTK_USE_MATROX_IMAGING)
      TARGET_LINK_LIBRARIES (vtkGraphsJava ${MIL_LIBRARY})
    ENDIF (VTK_USE_MATROX_IMAGING)
  ENDIF (WIN32)

  INSTALL_TARGETS(/lib/vtk vtkGraphsJava)
ENDIF (VTK_WRAP_JAVA)

TARGET_LINK_LIBRARIES (vtkGraphs vtkCommon vtkFiltering vtkGraphics vtkRendering vtkIO)

IF (WIN32)
  IF (VTK_USE_VIDEO_FOR_WINDOWS)
    TARGET_LINK_LIBRARIES (vtkGraphs vfw32)
  ENDIF (VTK_USE_VIDEO_FOR_WINDOWS)
  IF (VTK_USE_MATROX_IMAGING)
    TARGET_LINK_LIBRARIES (vtkGraphs ${MIL_LIBRARY})
  ENDIF (VTK_USE_MATROX_IMAGING)
ENDIF (WIN32)

INSTALL_TARGETS(/lib/vtk vtk${KIT})
INSTALL_FILES(/include/vtk .h ${Kit_SRCS})
