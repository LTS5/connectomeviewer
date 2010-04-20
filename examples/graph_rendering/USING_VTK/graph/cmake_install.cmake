# Install script for directory: /home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/vtk" TYPE FILE FILES
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkIdMap.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkStringTable.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkIdIterator.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkIdArrayIterator.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkIdListIterator.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkChildIterator.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkParentIterator.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraph.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkIndexedSet.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphSource.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkXGMLReader.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGMLReader.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGMLWriter.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphToGraphFilter.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphToPolyDataFilter.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkForestToTree.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkConeLayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkRadialLayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkReingoldTilfordLayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkUseLayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGEMLayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkHDELayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkApplyLayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkSpanLayout.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkSpanningDAG.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkEdgeSpanTree.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkEdgeGeometry.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkNodeGeometry.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkConeGeometry.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkDualGeometry.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkSubgraphFilter.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkSelectOnSubrange.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkSelectNthBestNodes.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkSelectReachable.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkDistanceMetric.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkStrahlerMetric.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphStrahlerMetric.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkEdgeTubeFilter.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkLabelMatcher.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphOperations.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphToUnstructuredGrid.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkUnstructuredGridToGraph.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphReader.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphWriter.h"
    "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/vtkGraphStringMapper.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/vtk" TYPE STATIC_LIBRARY FILES "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/libvtkGraphs.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/graph/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
