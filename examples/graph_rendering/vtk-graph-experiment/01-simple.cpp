#include "vtkMutableUndirectedGraph.h"
#include "vtkSmartPointer.h"
#include <iostream>

using namespace std;

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

int main() {

	// Create Graph
	VTK_CREATE(vtkMutableUndirectedGraph, graph);

	// Add vertices
	graph->AddVertex();
	graph->AddVertex();
	graph->AddVertex();

	// Add edges
	graph->AddEdge(0, 1);
	graph->AddEdge(1, 2);
	graph->AddEdge(0, 2);

}
