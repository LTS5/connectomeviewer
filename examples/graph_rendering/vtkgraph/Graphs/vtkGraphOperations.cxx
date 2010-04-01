/*
 *	File: vtkGraphOperations.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkGraphOperations.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"
#include "vtkIdTypeArray.h"
#include "vtkPoints.h"
#include "vtkDataArrayCollection.h"
#include "vtkDataArrayCollectionIterator.h"
#include "vtkFieldData.h"
#include "vtkDataArray.h"

//-------------------------------------------------
// Initialization, creation, constructor etc.

vtkStandardNewMacro(vtkGraphOperations);
vtkCxxRevisionMacro(vtkGraphOperations, "1.0");

vtkGraphOperations::_vtkOpEntry_s vtkGraphOperations::GraphOps[]
= { { "copyA",				VTK_COPY_A			},
		{ "copyB", 				VTK_COPY_A			},
		{ "intersection",	VTK_INTERSECT 	},
		{ "union",				VTK_UNION 			},
		{ "difference",		VTK_DIFFERENCE	},
		{ NULL,						-1							}};
	
vtkGraphOperations::vtkGraphOperations()
{
	this->NodeOperation = VTK_COPY_A;
	this->EdgeOperation = VTK_COPY_A;
}

vtkGraphOperations::~vtkGraphOperations()
{
}

//-------------------------------------------------
// Set operation names using string.

void vtkGraphOperations::SetNodeOperation(char *opName)
{
	for (int i = 0; GraphOps[i].opName; i++)
		{
		if (!strcmp(GraphOps[i].opName, opName))
			{
			this->SetNodeOperation(GraphOps[i].opCode);
			return;
			}
		}
	vtkErrorMacro(<<"Operation "<<opName<<" not recognized.");
}

void vtkGraphOperations::SetEdgeOperation(char *opName)
{
	for (int i = 0; GraphOps[i].opName; i++)
		{
		if (!strcmp(GraphOps[i].opName, opName))
			{
			this->SetEdgeOperation(GraphOps[i].opCode);
			return;
			}
		}
	vtkErrorMacro(<<"Operation "<<opName<<" not recognized.");
}

void vtkGraphOperations::FieldIntersect(
	vtkFieldData *dataA, 
	vtkFieldData *dataB,
	vtkFieldData *dataO,
	vtkDataArrayCollection *set,
	double **tuple)
{
	vtkDataArray *arrayA, *arrayB, *arrayO;
	int nrComps, maxNrComponents;
	
	maxNrComponents = 0;
	for (int a=0; a < dataA->GetNumberOfArrays(); a++)
		{
		arrayA = dataA->GetArray(a);
		arrayB = dataB->GetArray(arrayA->GetName());
		if (arrayB && arrayA->GetDataType() == arrayB->GetDataType())
			{
			set->AddItem(arrayA);
			set->AddItem(arrayB);
			arrayO = vtkDataArray::CreateDataArray(arrayA->GetDataType());
			arrayO->SetName(arrayA->GetName());
			nrComps = arrayA->GetNumberOfComponents();
			arrayO->SetNumberOfComponents(nrComps);
			arrayO->SetNumberOfTuples(
				arrayA->GetNumberOfTuples() + arrayB->GetNumberOfTuples());
			if (nrComps > maxNrComponents)
				{
				maxNrComponents = nrComps;
				}
			dataO->AddArray(arrayO);
			set->AddItem(arrayO);
			arrayO->Delete();
			}
		}
	*tuple = NULL;
	if (maxNrComponents > 0)
		{
		*tuple = new double[maxNrComponents];
		}
}

void vtkGraphOperations::FieldUnion(
	vtkFieldData *dataA, 
	vtkFieldData *dataB,
	vtkFieldData *dataO,
	vtkDataArrayCollection *fromA, 
	vtkDataArrayCollection *fromB,
	double **tuple)
{
	vtkDataArray *arrayA, *arrayB, *arrayO;
	int nrComps, maxNrComponents, a;
	
	maxNrComponents = 0;
	// Make a copy of each array from graph A
	for (a=0; a < dataA->GetNumberOfArrays(); a++)
		{
		arrayA = dataA->GetArray(a);
		fromA->AddItem(arrayA);
		arrayO = vtkDataArray::CreateDataArray(arrayA->GetDataType());
		arrayO->SetName(arrayA->GetName());
		arrayO->SetNumberOfTuples(arrayA->GetNumberOfTuples());
		nrComps = arrayA->GetNumberOfComponents();
		arrayO->SetNumberOfComponents(nrComps);
		if (nrComps > maxNrComponents)
			{
			maxNrComponents = nrComps;
			}
		dataO->AddArray(arrayO);
		fromA->AddItem(arrayO);
		arrayO->Delete();
		}
	// For each array in graph B, if it is not present 
	// in graph A, add a copy of it to the output.
	for (a=0; a < dataB->GetNumberOfArrays(); a++)
		{
		arrayB = dataB->GetArray(a);
		arrayA = dataA->GetArray(arrayB->GetName());
		if (!arrayA)
			{
			fromB->AddItem(arrayB);
			arrayO = vtkDataArray::CreateDataArray(arrayB->GetDataType());
			arrayO->SetName(arrayB->GetName());
			arrayO->SetNumberOfTuples(arrayB->GetNumberOfTuples());
			nrComps = arrayB->GetNumberOfComponents();
			arrayO->SetNumberOfComponents(nrComps);
			if (nrComps > maxNrComponents)
				{
				maxNrComponents = nrComps;
				}
			dataO->AddArray(arrayO);
			fromB->AddItem(arrayO);
			arrayO->Delete();
			}
		}			
	*tuple = NULL;
	if (maxNrComponents > 0)
		{
		*tuple = new double[maxNrComponents];
		}
}


void vtkGraphOperations::EnableStringArrays(
	vtkGraph *gA,
	vtkGraph *gB,
	vtkGraph *gO,
	int location, 
	int mode)
{
	int inA, inB, inO;
	vtkFieldData *field = gO->GetAttributeData(location);
	vtkDataArray *array;
	const char *name;

	for (int a = 0; a < field->GetNumberOfArrays(); a++)
		{
		array = field->GetArray(a);
		name = array->GetName();
		inA = gA->IsDataStringArray(location, name);
		inB = gB->IsDataStringArray(location, name);
		inO = 0;
		switch (mode)
			{
			case VTK_COPY_A:
				inO = inA;
				break;
			case VTK_COPY_B:
				inO = inB;
				break;
			case VTK_UNION:
				inO = inA*inB;
				break;
			case VTK_INTERSECT:
				inO = inA+inB;
				break;
			case VTK_DIFFERENCE:
				inO = inA;
				break;
			default:
				vtkErrorMacro(<<"Undefined mode.");
			}
		if (inO)
			{
			gO->MarkAsDataStringArray(location, name);
			}
		}
}
		

//-------------------------------------------------

void vtkGraphOperations::Execute()
{
  vtkGraph *graphA = this->GetInput();
  vtkGraph *output = this->GetOutput();
  vtkGraph *graphB = (vtkGraph*)this->Inputs[1];
	vtkGraph *source;
  	
	vtkIdType node, edge, src, dst;

	vtkIdType size;
	double *tuple = NULL; 	// for copying data array tuples

	vtkIdIterator *nodes, *edges;
	vtkDataArrayCollection *arrays, *arraysB;
	vtkDataArrayCollectionIterator *iter, *iterB;
	vtkFieldData *dataA, *dataB, *dataI, *dataO;
	vtkDataArray *arrayA, *arrayB, *arrayO;
	
	// For copying bends.
	vtkIdType *bends;
	vtkIdType abend;
	int bnd;
	
  // Initialize

  vtkDebugMacro(<<"GraphOperations executing.");

  if (this->NumberOfInputs < 2)
    {
		vtkErrorMacro(<< "Two graphs are required." );
		return;
    }
	
	arrays  = vtkDataArrayCollection::New();
	arraysB = vtkDataArrayCollection::New();
	iter  = vtkDataArrayCollectionIterator::New();
	iterB = vtkDataArrayCollectionIterator::New();
	
		
	// PERFORM THE NODE OPERATION ---------------------------------
	dataA = graphA->GetNodeData();
	dataB = graphB->GetNodeData();
	dataO = output->GetNodeData();
	switch(this->NodeOperation)
		{
		case VTK_COPY_A:
		case VTK_COPY_B:
			if (this->NodeOperation == VTK_COPY_A)
				{
				source = graphA;
				dataI = dataA;
				}
			else
				{
				source = graphB;
				dataI = dataB;
				}
			output->GetNodeData()->CopyStructure(source->GetNodeData());
			output->SetInitialNodeSize(source->GetNumberOfNodes());
			size = dataI->GetNumberOfComponents();
			tuple = (size > 0) ? new double[size] : NULL;

			nodes = source->GetNodes();	
			while (nodes->HasNext())
				{
				node = nodes->GetNext();
				output->CreateNode(node);
				if (tuple)
					{
					dataI->GetTuple(source->GetNodeIndex(node), tuple);
					dataO->InsertTuple(output->GetNodeIndex(node), tuple);
					}
				if (source->GetNodeIndex(node) < 
						source->GetLayout()->GetNumberOfPoints())
					{
					output->SetPosition(node, source->GetPosition(node));
					}
				}
			nodes->Delete();
			break; //-------------------------------------------------
			
		case VTK_UNION:
			{
			output->SetInitialNodeSize(
				graphA->GetNumberOfNodes() + graphB->GetNumberOfNodes());
			
			this->FieldIntersect(dataA, dataB, dataO, arrays, &tuple);
			// "arrays" consists of a sequence of triples of arrays, the
			// first is the array in graph A from which data will be
			// taken for nodes from A, the second is the array from 
			// graph B used for nodes from B, the third is the new
			// array in output to which the data will be copied.
			iter->SetCollection(arrays);
			iter->InitTraversal();
		
			// Copy all nodes from graph A to output. Also copy
			// attribute data for arrays common to both graphs.	
			nodes = graphA->GetNodes();		
			while (nodes->HasNext())
				{
				node = nodes->GetNext();
				output->CreateNode(node);
				iter->GoToFirstItem();
				while (!iter->IsDoneWithTraversal())
					{
					arrayA = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayA->GetTuple(graphA->GetNodeIndex(node), tuple);
					arrayO->SetTuple(output->GetNodeIndex(node), tuple);
					}
				if (graphA->GetNodeIndex(node) < 
						graphA->GetLayout()->GetNumberOfPoints())
					{
					output->SetPosition(node, graphA->GetPosition(node));
					}
				}
			nodes->Delete();
			
			// Copy nodes from graph B to output UNLESS the node
			// is already present.  Also copy any attribute data.
			nodes = graphB->GetNodes();		
			while (nodes->HasNext())
				{
				node = nodes->GetNext();
				if (output->HasNode(node))
					{
					continue;
					}
				output->CreateNode(node);
				
				iter->GoToFirstItem();
				while (!iter->IsDoneWithTraversal())
					{
					arrayA = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB->GetTuple(graphB->GetNodeIndex(node), tuple);
					arrayO->SetTuple(output->GetNodeIndex(node), tuple);
					}
				if (graphB->GetNodeIndex(node) < 
						graphB->GetLayout()->GetNumberOfPoints())
					{
					output->SetPosition(node, graphB->GetPosition(node));
					}
				}
			nodes->Delete();
			}
			break;
			
		case VTK_INTERSECT:
			{
			output->SetInitialNodeSize(graphA->GetNumberOfNodes());
			
			this->FieldUnion(dataA, dataB, dataO, arrays, arraysB, &tuple);
			// arrays will consist of pairs, of an array from
			// graphA/B, and an array from the output.
			iter->SetCollection(arrays);
			iter->InitTraversal();
			iterB->SetCollection(arraysB);
			iterB->InitTraversal();

			// Copy nodes from A to output PROVIDED the same node
			// is also present in graph B.
			nodes = graphA->GetNodes();		
			while (nodes->HasNext())
				{
				node = nodes->GetNext();
				if (!graphB->HasNode(node))
					{
					continue;
					}
				output->CreateNode(node);
				if (!tuple)
					{
					continue;
					}
				// Copy tuples from data arrays in graph A
				iter->GoToFirstItem();
				while (!iter->IsDoneWithTraversal())
					{
					arrayA = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayA->GetTuple(graphA->GetNodeIndex(node), tuple);
					arrayO->SetTuple(output->GetNodeIndex(node), tuple);
					}
				// Copy tuples from data arrays in graph B
				iterB->GoToFirstItem();
				while (!iterB->IsDoneWithTraversal())
					{
					arrayB = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB->GetTuple(graphB->GetNodeIndex(node), tuple);
					arrayO->SetTuple(output->GetNodeIndex(node), tuple);
					}
				if (graphA->GetNodeIndex(node) < 
						graphA->GetLayout()->GetNumberOfPoints())
					{
					output->SetPosition(node, graphA->GetPosition(node));
					}
				}
			nodes->Delete();
			}
			break;
			
		case VTK_DIFFERENCE:
			{
			output->SetInitialNodeSize(graphA->GetNumberOfNodes());
			size = dataA->GetNumberOfComponents();
			tuple = (size > 0) ? new double[size] : NULL;

			// Copy nodes from graph A to output UNLESS the node
			// happens to be a member of graph B.
			nodes = graphA->GetNodes();		
			while (nodes->HasNext())
				{
				node = nodes->GetNext();
				if (graphB->HasNode(node))
					{
					continue;
					}
				output->CreateNode(node);
				if (tuple)
					{
					dataA->GetTuple(graphA->GetNodeIndex(node), tuple);
					dataO->InsertTuple(output->GetNodeIndex(node), tuple);
					}
				if (graphA->GetNodeIndex(node) < 
						graphA->GetLayout()->GetNumberOfPoints())
					{
					output->SetPosition(node, graphA->GetPosition(node));
					}
				}
			nodes->Delete();
			}
			break;
			
		default:
			vtkErrorMacro(<<"Unknown node operation.");
			break;
		}
	// Remove temporary tuple.
	if (tuple)
		{
		delete [] tuple;
		}
	


	// PERFORM THE EDGE OPERATION ---------------------------------
	
	dataA = graphA->GetEdgeData();
	dataB = graphB->GetEdgeData();
	dataO = output->GetEdgeData();
	
	switch(this->EdgeOperation)
		{
		case VTK_COPY_A:
		case VTK_COPY_B:
			{
			if (this->EdgeOperation == VTK_COPY_A)
				{
				source = graphA;
				dataI = dataA;
				}
			else
				{
				source = graphB;
				dataI = dataB;
				}
			output->SetInitialEdgeSize(source->GetNumberOfEdges());
			output->GetEdgeData()->CopyStructure(source->GetEdgeData());
			size = dataI->GetNumberOfComponents();
			tuple = (size > 0) ? new double[size] : NULL;

			// Copy all edges from source to output PROVIDED the 
			// src and dst nodes are already present.
			edges = source->GetEdges();	
			while (edges->HasNext())
				{
				edge = edges->GetNext();
				source->GetEdge(edge, src, dst);
				if (!(output->HasNode(src) && output->HasNode(dst)))
					{
					continue;
					}
				output->CreateEdge(edge, src, dst);
				bends = source->GetBendArray(edge);
				for (bnd = 0; bnd < source->GetNumberOfBends(edge); bnd++)
					{
					output->InsertBend(edge, bnd, bends[bnd]);
					}
				if (tuple)
					{
					dataI->GetTuple(source->GetEdgeIndex(edge), tuple);
					dataO->InsertTuple(output->GetEdgeIndex(edge), tuple);
					}
				}
			edges->Delete();
			}
			break;
		
		case VTK_UNION:
			{
			output->SetInitialEdgeSize(
					graphA->GetNumberOfEdges() + graphB->GetNumberOfEdges());

			this->FieldIntersect(dataA, dataB, dataO, arrays, &tuple);
			// arrays is a sequence of triples, arrayA, arrayB, arrayO.
			iter->SetCollection(arrays);
			iter->InitTraversal();

			// Copy all edges from graph A to output, provided nodes
			// are present in the output.
			edges = graphA->GetEdges();	
			while (edges->HasNext())
				{
				edge = edges->GetNext();
				graphA->GetEdge(edge, src, dst);
				if (!(output->HasNode(src) && output->HasNode(dst)))
					{
					continue;
					}
				// Note that for the union case, we don't reuse the
				// edge-ids, as there might be clashes between the
				// two graphs.
				output->CreateEdge(src, dst);
				bends = graphA->GetBendArray(edge);
				for (bnd = 0; bnd < graphA->GetNumberOfBends(edge); bnd++)
					{
					output->InsertBend(edge, bnd, bends[bnd]);
					}
				iter->GoToFirstItem();
				while (!iter->IsDoneWithTraversal())
					{
					arrayA = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayA->GetTuple(graphA->GetEdgeIndex(edge), tuple);
					arrayO->SetTuple(output->GetEdgeIndex(edge), tuple);
					}
				}
			edges->Delete();
			
			// Copy edges from graph B to output provided (i) that
			// the source and target nodes are present in the output,
			// and (ii) the edge is not already present.  Note that
			// we are looking for an edge between the src and dst
			// nodes; the id of the edge is immaterial.  Any bend
			// in an edge is copied to output, but the index of the
			// bend must be offset, as the bend points of the output
			// will be those of graphA followed by those of graph B.
			abend = graphA->GetNumberOfBends();
			edges = graphB->GetEdges();	
			while (edges->HasNext())
				{
				edge = edges->GetNext();
				graphB->GetEdge(edge, src, dst);
				if (!output->HasNode(src) || 
						!output->HasNode(dst) || 
						!output->HasEdge(src, dst))
					{
					continue;
					}
				output->CreateEdge(src, dst);
				bends = graphB->GetBendArray(edge);
				for (bnd = 0; bnd < graphA->GetNumberOfBends(edge); bnd++)
					{
					output->InsertBend(edge, bnd, abend + bends[bnd]);
					}
				iter->GoToFirstItem();
				while (!iter->IsDoneWithTraversal())
					{
					arrayA = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB->GetTuple(graphB->GetEdgeIndex(edge), tuple);
					arrayO->SetTuple(output->GetEdgeIndex(edge), tuple);
					}
				}
			edges->Delete();
			}
			break;
		
		case VTK_INTERSECT:
			{
			output->SetInitialEdgeSize(graphA->GetNumberOfEdges());
			this->FieldUnion(dataA, dataB, dataO, arrays, arraysB, &tuple);
			// arrays/B is a sequence of pairs, of an array from
			// graphA/B, and an array from the output.
			iter->SetCollection(arrays);
			iter->InitTraversal();
			iterB->SetCollection(arraysB);
			iterB->InitTraversal();

			// Copy edges from graph A to output provided the source
			// and target are in the output, and the edge (its id)
			// is also present in graphB.  The bends of the output
			// edge are taken from graph A.
			edges = graphA->GetEdges();		
			while (edges->HasNext())
				{
				edge = edges->GetNext();
				graphA->GetEdge(edge, src, dst);
				if (!output->HasNode(src) || 
						!output->HasNode(dst) ||
						!graphB->HasEdge(src, dst))
					{
					continue;
					}
				output->CreateEdge(edge, src, dst);
				bends = graphA->GetBendArray(edge);
				for (bnd = 0; bnd < graphA->GetNumberOfBends(edge); bnd++)
					{
					output->InsertBend(edge, bnd, bends[bnd]);
					}
				// Copy tuples from data arrays in graph A
				iter->GoToFirstItem();
				while (!iter->IsDoneWithTraversal())
					{
					arrayA = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayA->GetTuple(graphA->GetEdgeIndex(edge), tuple);
					arrayO->SetTuple(output->GetEdgeIndex(edge), tuple);
					}
				// Copy tuples from data arrays in graph B
				iterB->GoToFirstItem();
				while (!iterB->IsDoneWithTraversal())
					{
					arrayB = iter->GetDataArray();
					iter->GoToNextItem();
					arrayO = iter->GetDataArray();
					iter->GoToNextItem();
					arrayB->GetTuple(graphB->GetEdgeIndex(edge), tuple);
					arrayO->SetTuple(output->GetEdgeIndex(edge), tuple);
					}
				}
			edges->Delete();
			}
			break;
			
		case VTK_DIFFERENCE:
			{
			output->SetInitialEdgeSize(graphA->GetNumberOfEdges());
			size = graphA->GetNodeData()->GetNumberOfComponents();
			tuple = (size > 0) ? new double[size] : NULL;

			// Copy edges from graph A to output provided the source
			// and target are in the output, and the edge (its id)
			// is NOT present in graph B.
			edges = graphA->GetEdges();		
			while (edges->HasNext())
				{
				edge = edges->GetNext();
				graphA->GetEdge(edge, src, dst);
				if (!output->HasNode(src) || 
						!output->HasNode(dst) ||
						graphB->HasEdge(src, dst))
					{
					continue;
					}
				output->CreateEdge(edge, src, dst);
				bends = graphA->GetBendArray(edge);
				for (bnd = 0; bnd < graphA->GetNumberOfBends(edge); bnd++)
					{
					output->InsertBend(edge, bnd, bends[bnd]);
					}
				if (tuple)
					{
					graphA->GetEdgeData()->GetTuple(
							graphA->GetEdgeIndex(edge), tuple);
					output->GetEdgeData()->InsertTuple(
							output->GetEdgeIndex(edge), tuple);
					}
				}
			edges->Delete();
			}
			break;
			
		default:
			vtkErrorMacro(<<"Unknown edge operation.");
			break;
		}

	if (tuple)
		{
		delete [] tuple;
		}

	this->EnableStringArrays(
		graphA, graphB, output, NODE_DATA, this->NodeOperation);
	this->EnableStringArrays(
		graphA, graphB, output, EDGE_DATA, this->EdgeOperation);

	// Deal with the bends.  Apart from union, the bend points of
	// the output are taken to be those of graph A, so the bends
	// can simply be shallow-copied.  For union, we must make a 
	// fresh set of points containing the bends of both A and B.
	if (this->EdgeOperation != VTK_UNION)
		{
		output->SetBends(graphA->GetBends());
		}
	else
		{
		output->SetBends(vtkPoints::New());
		output->GetBends()->DeepCopy(graphA->GetBends());
		abend = graphA->GetNumberOfBends();
		for (bnd = 0; bnd < graphB->GetNumberOfBends(); bnd++)
			{
			output->GetBends()->InsertPoint(abend + bnd, 
				graphB->GetBends()->GetPoint(abend));
			}
		}
	arrays->Delete();
	arraysB->Delete();
	iter->Delete();
	iterB->Delete();
	
	// node/edge allocation sizes were estimates, now
	// reclaim any excess space.
	output->Squeeze();
		
	vtkDebugMacro(<<"GraphOperations done.");
}

void vtkGraphOperations::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
  os << indent << "GraphOperations filter.\n";
	os << indent << "Node operation: " << this->NodeOperation << "\n";
	os << indent << "Edge operation: " << this->EdgeOperation << "\n";
}


