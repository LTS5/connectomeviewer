/*
 *	File: vtkConeGeometry.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkConeGeometry.h"
#include "vtkObjectFactory.h"
#include "vtkMath.h"
#include "vtkIdIterator.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkCellData.h"

// Allocation and Printing -------------------------------

vtkStandardNewMacro(vtkConeGeometry);
vtkCxxRevisionMacro(vtkConeGeometry, "1.0");

vtkConeGeometry::vtkConeGeometry()
{
	this->MaxBladeAngle = 30.0;
}


void vtkConeGeometry::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkSource::PrintSelf(os, indent);
  os << indent << "Blade angle: " << this->MaxBladeAngle << "\n";
}


double vtkConeGeometry::Angle(
  vtkIdType     c,  // apex of current cone
  vtkIdType     p,  // child of c
  vtkIdType     q,  // child of c
	vtkGraph *graph)
{
  double cc[3];
  double pc[3];
  double qc[3];
  double dot, res;
  double v1[3], v2[3];

  graph->GetLayout()->GetPoint(graph->GetNodeIndex(c), cc);
	graph->GetLayout()->GetPoint(graph->GetNodeIndex(p), pc);
  graph->GetLayout()->GetPoint(graph->GetNodeIndex(q), qc);

	// Want the angle between the nodes when projected into
	// a plane parallel to z=0.
	cc[2] = pc[2] = qc[2] = 0.0;
	
  dot = 0.0;
  for (int i = 0; i < 3; i++)
    {
    v1[i] = pc[i] - cc[i];
    v2[i] = qc[i] - cc[i];
    dot += v1[i] * v2[i];
    }
  res = acos( dot / (vtkMath::Norm(v1)*vtkMath::Norm(v2)) );

  return 180.0*res/vtkMath::Pi();
}

void vtkConeGeometry::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  
  vtkIdType node, child, startFan, startBlade, prev;
  vtkCellArray *polygons;
  vtkCellArray *lines;
  vtkIdIterator *nodeIterator;
	vtkIdIterator *children;

  double currAngle;

  vtkIdType nrchildren;
  int done;

  vtkDebugMacro(<< "Starting ConeGeometry");

	// Set point positions in output to be node positions in 
	// input.
	output->SetPoints(input->GetLayout());

  polygons = vtkCellArray::New();
  polygons->Initialize();

  lines = vtkCellArray::New();
  lines->Initialize();

  nodeIterator = input->GetNodes();
  while (nodeIterator->HasNext())
    {
    node = nodeIterator->GetNext();
    nrchildren = input->GetNumberOfChildren(node);

    // No children --> nothing further to do, so continue
		// with the next node.
    if (nrchildren == 0)
      {
      continue;
      }

		// startFan = child node that starts the current fan
		// startBlade = child node that starts the current blade
		// of this fan.
		children = input->GetChildren(node);
    child = children->GetNext();
    
    // One child --> produce a line, and
    // then continue with next node.
    if (nrchildren == 1)
      {
      lines->InsertNextCell(2);
      lines->InsertCellPoint(input->GetNodeIndex(node));
      lines->InsertCellPoint(input->GetNodeIndex(child));
      continue;
      }


    // More than one child ... as we traverse the children, we
		// need to keep track of the previous child in the current
		// blade.    
		startBlade = child;
		startFan = startBlade;
    done  = 0;
    while (!done)
      {
      prev  = child;
			// Check if we are about to close the fan, by returning
			// to the child that started the fan.
      if (children->HasNext())  // childnr < nrchildren
        {
				child = children->GetNext();
        }
      else
        {
        // visited all children, so close the loop by
        // considering the pair (last, start).
        child = startFan;
        done = 1;
        }
        			
      // Find the angle formed by the parent (node),
      // the current child, and the start of this blade.
      currAngle = this->Angle(node, startBlade, child, input);
			  
			// Consider whether to output a new blade.  A new blade
			// will be output if:
			// 1. we are done, or
			// 2. the angle between the current child and start of blade
			// is larger than the threshold: this means that the child 
			// cannot be added to the blade.
      if (done || currAngle > this->MaxBladeAngle)
        {
				// Try and close fan back to the start, if possible.
				if (done && currAngle <= this->MaxBladeAngle)
					{
        	polygons->InsertNextCell(3);
        	polygons->InsertCellPoint(input->GetNodeIndex(node));
        	polygons->InsertCellPoint(input->GetNodeIndex(startBlade));
        	polygons->InsertCellPoint(input->GetNodeIndex(child));
					}
				else
					{
					// If the previous child node was the start of the blade,
					// it is not possible to draw a triangle, instead a single
					// line is output.
    	    if (prev == startBlade)
						{
						// output a line
            lines->InsertNextCell(2);
						lines->InsertCellPoint(input->GetNodeIndex(node));
						lines->InsertCellPoint(input->GetNodeIndex(prev));          
						}
      		else
        		{
        		// output a triangle
        		polygons->InsertNextCell(3);
        		polygons->InsertCellPoint(input->GetNodeIndex(node));
        		polygons->InsertCellPoint(input->GetNodeIndex(startBlade));
        		polygons->InsertCellPoint(input->GetNodeIndex(prev));
						}
					// Now decide on where the next blade should start. 
					// If the current and previous children are within
					// MaxBladeAngle of each other, then start the next
					// blade with the previous child, otherwise start 
					// with the current child.
					if (this->Angle(node, prev, child, input) <= this->MaxBladeAngle)
						{
						startBlade = prev;
						}
					else
						{
						startBlade = child;
						}
        	}
				} 	// output new blade
      } // traverse children
		children->Delete();
    } // travserse nodes
	nodeIterator->Delete();

  output->SetLines(lines);
  output->SetPolys(polygons);

  // Pass data fields through to output
  output->GetPointData()->ShallowCopy(input->GetNodeData());
  output->GetCellData()->ShallowCopy(input->GetEdgeData());

  // De-reference objects allocated in this filter.
  lines->Delete();
  polygons->Delete();
	vtkDebugMacro(<<"ConeGeometry done.");
}

