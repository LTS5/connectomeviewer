/*
 *	File: vtkGEMLayout.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// An implementation of the GEM layout algorithm, based on
// code by Arne Frick placed in the public domain.  See the
// vtkGEMLayout.h file for further details.

#include "vtkGEMLayout.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"

#define MMIN(x,y)   ((x < y) ? (x) : (y))
#define MMAX(x,y)   ((x < y) ? (y) : (x))
#define MABS(v)     ((v > 0) ? (v) : (-(v)))

vtkStandardNewMacro(vtkGEMLayout);
vtkCxxRevisionMacro(vtkGEMLayout,"1.0");

vtkGEMLayout::vtkGEMLayout() 
{
	this->Map = NULL;
	this->Q   = NULL;
}

vtkGEMLayout::~vtkGEMLayout()
{
	if (this->Map)
		{
		delete [] this->Map;
		}
	if (this->Q)
		{
		delete [] this->Q;
		}
}

void vtkGEMLayout::PrintSelf(ostream &os, vtkIndent indent)
{
	vtkGraphToGraphFilter::PrintSelf(os, indent);
}

void vtkGEMLayout::SetGEMParameters()
{
	i_maxtemp      = vtkGEMLayout::IMAXTEMPDEF();
	a_maxtemp      = vtkGEMLayout::AMAXTEMPDEF();
	i_starttemp    = vtkGEMLayout::ISTARTTEMPDEF();
	a_starttemp    = vtkGEMLayout::ASTARTTEMPDEF();
	i_finaltemp    = vtkGEMLayout::IFINALTEMPDEF();
	a_finaltemp    = vtkGEMLayout::AFINALTEMPDEF();
	i_maxiter      = vtkGEMLayout::IMAXITERDEF();
	a_maxiter      = vtkGEMLayout::AMAXITERDEF();
	i_gravity      = vtkGEMLayout::IGRAVITYDEF();
	i_oscillation  = vtkGEMLayout::IOSCILLATIONDEF();
	i_rotation     = vtkGEMLayout::IROTATIONDEF();
	i_shake        = vtkGEMLayout::ISHAKEDEF();
	a_gravity      = vtkGEMLayout::AGRAVITYDEF();
	a_oscillation  = vtkGEMLayout::AOSCILLATIONDEF();
	a_rotation     = vtkGEMLayout::AROTATIONDEF();
	a_shake        = vtkGEMLayout::ASHAKEDEF();
}
       
vtkIdType vtkGEMLayout::Select() 
{
  vtkIdType    u, n, v;

  if (this->Iteration == 0) 
    {
		if (this->Map) 
			{
			delete [] this->Map;
			}
		Map = new vtkIdType[this->NodeCount];
		for (vtkIdType i = 0; i < this->NodeCount; i++)
			{
			this->Map[i] = i;
			}
    }
	n = this->NodeCount - (this->Iteration % this->NodeCount);
  v = rand () % n;  // was 1 + rand() % n due to numbering in GEM
  if (v == this->NodeCount)
		{
		v--;
		}
  if (n == this->NodeCount) 
		{
		n--;
		}
  u = this->Map[v]; 
	this->Map[v] = this->Map[n]; 
	this->Map[n] = u;
  return u;
}

vtkIdType vtkGEMLayout::Bfs(vtkGraph *graph, vtkIdType root) 
{
  vtkIdIterator *nodeSet;
  vtkIdType v, ui;

  if (root >= 0) 
		{
		if (this->Q) 
			{
			delete [] this->Q;
			}
		this->Q = new vtkIdType[graph->GetNumberOfNodes()];
		this->Head = 0;
		this->Tail = 0;
		if (!this->Mark->GetValue(root))
			{ 
			for (vtkIdType vi = 0; vi < this->NodeCount; vi++)
				{
	  		this->In[vi] = 0;
				}
			}
    else
			{
			this->Mark->SetValue(root, 1);
			}
		this->Q[this->Head++] = root;
		this->In[root] = 1;
		}
  if (this->Head == this->Tail)		// Q size is 0
		{
    return -1;          // null
		} 
	v = this->Q[this->Tail++];
	nodeSet = graph->GetNeighbors(graph->GetNodeId(root));
	while (nodeSet->HasNext()) 
    {
		ui = graph->GetNodeIndex(nodeSet->GetNext());
		if (this->In[ui] != 0) 
			{
			this->Q[this->Head++] = ui;
			this->In[ui] = this->In[v]+1;
			}
		}
	nodeSet->Delete();
	return v;
}

vtkIdType vtkGEMLayout::GraphCenter(vtkGraph *graph)
{
  vtkIdType  c, u, v, w;    // nodes
  vtkIdType  h;

  c = -1;
  u = -1;

  h = this->NodeCount + 1;
  for (w = 0; w < this->NodeCount; w++) 
    {
		v = this->Bfs(graph, w);
		while (v >= 0 && this->In[v] < h) 
			{
			u = v;
			v = this->Bfs(graph, -1);  // null
			}
		if (this->In[u] < h) 
			{
			h = this->In[u];
			c = w;
			}
		}
	return c;
}

void vtkGEMLayout::VertexDataInit(vtkGraph *graph, double starttemp) 
{
	double heat;
	vtkIdType vID;
	
	this->Temperature = 0;
	this->Center[0] = 0.0;
  this->Center[1] = 0.0;
	this->Center[2] = 0.0;

  for (vtkIdType v = 0; v < this->NodeCount; v++) 
    {
		heat = starttemp * ELEN();
		this->Heat[v] = heat;
		this->Temperature += (long)(heat * heat);
		this->Ximpulse[v] = 0.0;
		this->Yimpulse[v] = 0.0;
		this->Direction[v]  = 0.0;
		this->Mass[v] = 1 + this->Mass[v]/3;
		vID = graph->GetNodeId(v);
		this->Center[0] += graph->GetX(vID);
		this->Center[1] += graph->GetY(vID);
		this->Center[2] += graph->GetZ(vID);
		}
}


void vtkGEMLayout::IImpulse(
	vtkGraph *graph,
	vtkIdType v, 
	double &ix, 
	double &iy) 
{
  vtkIdIterator *nodeSet;

  double dx, dy, px, py;
  long int n;
  vtkIdType u;
	vtkIdType vID = graph->GetNodeId(v);
	vtkIdType uID;
	
  // gemP = this->GemProp + v;
  px = graph->GetX(vID);
	py = graph->GetY(vID);

  n  = (long)(i_shake * vtkGEMLayout::ELEN());
  ix = rand () % (2 * n + 1) - n;
  iy = rand () % (2 * n + 1) - n;
  ix += (long)((this->Center[0] / this->NodeCount - px) 
						* this->Mass[v] * i_gravity);
  iy += (long)((this->Center[1] / this->NodeCount - py) 
						* this->Mass[v] * i_gravity);

  for (u = 0; u < this->NodeCount; u++) 
    {
		// gemQ = this->GemProp + u;
		uID = graph->GetNodeId(u);
		if (this->In[u] > 0) 
			{
			dx = px - graph->GetX(uID);
			dy = py - graph->GetY(uID);
			n  = (vtkIdType)(dx*dx + dy*dy);
			if (n) 
				{
	      ix += dx * vtkGEMLayout::ELENSQR() / n;
	      iy += dy * vtkGEMLayout::ELENSQR() / n;
				}
			}
    }
	nodeSet = graph->GetNeighbors(vID);
  while (nodeSet->HasNext()) 
    {
		uID = nodeSet->GetNext();
		u   = graph->GetNodeIndex(uID);
		// gemQ = this->GemProp + u;
		if (this->In[u] > 0) 
			{
			dx = px - graph->GetX(uID);
			dy = py - graph->GetY(uID);
			n = (long)((dx*dx + dy*dy) / this->Mass[v]);
			n = MMIN(n, vtkGEMLayout::MAXATTRACT());  //   1048576L
			ix -= (long)(dx*n / vtkGEMLayout::ELENSQR());
			iy -= (long)(dy*n / vtkGEMLayout::ELENSQR());
			}
		}  
}

void vtkGEMLayout::Insert(vtkGraph *graph) 
{
	vtkIdIterator *nodeSet;
  vtkIdType startNode;
  vtkIdType u, v, w;
  vtkIdType d;
	double position[3];
	double pos[3];
	double ix, iy;

  this->VertexDataInit(graph, i_starttemp);
	
  this->Oscillation = i_oscillation;
  this->Rotation = i_rotation;
  this->MaxTemp = (long)(i_maxtemp * vtkGEMLayout::ELEN());

  v = this->GraphCenter(graph);

  for (vtkIdType ui = 0; ui < this->NodeCount; ui++)
		{
    this->In[ui] = 0;
		}
  this->In[v] = -1;

  startNode = -1;
  for (vtkIdType i = 0; i < this->NodeCount; i++) 
    {
		d = 0;
		for (vtkIdType j = 0; j < this->NodeCount; j++)
			{
			if (this->In[j] < d) 
				{
				d = this->In[j];
				v = j;
				}
			}
		this->In[v] = 1;
		
		nodeSet = graph->GetNeighbors(graph->GetNodeId(v));
		while (nodeSet->HasNext()) 
			{
			u = graph->GetNodeIndex(nodeSet->GetNext());
			if (this->In[u] <= 0)
				{
				this->In[u] = this->In[u]-1;
				}
			}
		nodeSet->Delete();
		
		// gemP = this->GemProp + v;
		graph->GetLayout()->SetPoint(v, 0.0, 0.0, 0.0);
		if (startNode >= 0)
			{
			d = 0;
			// gemP = this->GemProp + v;
			nodeSet = graph->GetNeighbors(graph->GetNodeId(v));
			while (nodeSet->HasNext()) 
				{
				w = graph->GetNodeIndex(nodeSet->GetNext());
				// gemQ = this->GemProp + w;
				if (this->In[w] > 0) 
					{
					graph->GetLayout()->GetPoint(v, position);
					graph->GetLayout()->GetPoint(w, pos);
					position[0] += pos[0];
					position[1] += pos[1];
					graph->GetLayout()->SetPoint(v, position);
					d++;
					}
				}
			if (d > 1) 
				{
				graph->GetLayout()->GetPoint(v, position);
				position[0] /= d;
				position[1] /= d;
				graph->GetLayout()->SetPoint(v, position);
				}
			d = 0;
			while ((d++ < i_maxiter) 
				&& (this->Heat[v] > i_finaltemp * ELEN())) 
				{
				this->IImpulse(graph, v, ix, iy);
    		this->Displace(graph, v, (long)ix, (long)iy);
				}
			}
     else
			{
			startNode = i;
			}
   	}
}

void vtkGEMLayout::Displace(
	vtkGraph *graph, 
	vtkIdType v, 
	long impx, 
	long impy) 
{
  long int t, n;
	vtkIdType vID;
	double position[3];
	double impvx, impvy;
	
  // GEMparam *gemP;

  if (impx != 0 || impy != 0) 
    {
		n = MMAX( labs((vtkIdType)impx), labs((vtkIdType)impy)) / 16384L;
		if (n > 1) 
			{
			impx /= n;
			impy /= n;
			}
		vID = graph->GetNodeId(v);
		// gemP = this->GemProp + v;
  	t  = (long)this->Heat[v];
		n  = (long)sqrt((long double)(impx*impx + impy*impy));

		if (n)
			{
  		impx = impx * t / n;
			impy = impy * t / n;
			graph->GetLayout()->GetPoint(v, position);
			position[0] += impx;
			position[1] += impy;
			graph->GetLayout()->SetPoint(v, position);	
			this->Center[0] += impx;
			this->Center[1] += impy;
			}

		impvx = this->Ximpulse[v];
		impvy = this->Yimpulse[v];
		n = t * (long)sqrt((long double) (impvx*impvx + impvy*impvy));
		if (n) 
			{
			this->Temperature -= t * t;
			t += (long)(t * Oscillation * (impx*impvx + impy*impvy) / n);
			t = MMIN(t, MaxTemp);
			this->Direction[v] =
				this->Direction[v] + Rotation * (impx*impvy - impy*impvx) / n;
			t -= (long)(t * MABS(this->Direction[v]) / this->NodeCount);
			t = MMAX(t, 2L);
			this->Temperature += t * t;
			this->Heat[v] = t;
			}
		this->Ximpulse[v] = impx;
		this->Yimpulse[v] = impy;
    }
}


void vtkGEMLayout::ARound(vtkGraph *graph) 
{
  vtkIdIterator *nodeSet;
  vtkIdType u, v, uID;
  double impx, impy, dx, dy;
	double px, py, qx, qy;
	double position[3];
  long int    n;
  // GEMparam *gemP, *gemQ;

  for (vtkIdType i = 0; i < this->NodeCount; i ++) 
		{
		v = this->Select();
		// gemP = GemProp + v;
		graph->GetLayout()->GetPoint(v, position);
		px = position[0];
		py = position[1];

		n = (long)(a_shake * ELEN());
		impx = rand () % (2 * n + 1) - n;
		impy = rand () % (2 * n + 1) - n;
		impx += (long)((this->Center[0]/this->NodeCount - px) 
							* this->Mass[v] * a_gravity);
		impy += (long)((this->Center[1]/this->NodeCount - py)
							* this->Mass[v] * a_gravity);

		for (vtkIdType j = 0; j < this->NodeCount; j++) 
			{
			// gemQ = GemProp + j;
			graph->GetLayout()->GetPoint(j, position);
			qx = position[0];
			qy = position[1];
			dx = px - qx;
			dy = py - qy;
			n  = (vtkIdType)(dx*dx + dy*dy);
			if (n) 
				{
				impx += dx * ELENSQR() / n;
				impy += dy * ELENSQR() / n;
				}
			}
		nodeSet = graph->GetNeighbors(graph->GetNodeId(v));
		while (nodeSet->HasNext()) 
			{
			uID = nodeSet->GetNext();
			u   = graph->GetNodeIndex(uID);
			// gemQ = GemProp + u;
			graph->GetLayout()->GetPoint(u, position);	
			qx = position[0];
			qy = position[1];
			dx = px - qx;
			dy = py - qy;
			n  = (long)((dx*dx + dy*dy) / this->Mass[v]);
			n  = MMIN(n, MAXATTRACT());  // 1048576L  (N2)
			impx -= dx * n / ELENSQR();
			impy -= dy * n / ELENSQR();
			}
		this->Displace(graph, v, (long)impx, (long)impy);
		this->Iteration++;
    }
}

void vtkGEMLayout::Arrange(vtkGraph *graph) 
{
	long int stop_temperature;
	long int last_temperature;
	unsigned long stop_iteration;

	this->VertexDataInit(graph, a_starttemp);

	this->Oscillation = a_oscillation;
	this->Rotation    = a_rotation;
	this->MaxTemp     = (long)(a_maxtemp * ELEN());
	stop_temperature  = (long)(a_finaltemp * a_finaltemp * ELENSQR() * NodeCount);
	stop_iteration    = a_maxiter * NodeCount * NodeCount;
	this->Iteration   = 0;
  last_temperature  = this->Temperature+1;
  
	while (this->Temperature > stop_temperature 
			&& this->Iteration < stop_iteration)
		{
		last_temperature = this->Temperature;
		this->ARound(graph);
    }
}


void vtkGEMLayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
	vtkIdType node, index;
	vtkIdIterator *nodes;
	vtkPoints *layout;

	if (!input)
		{
		return;
		}

  vtkDebugMacro(<<"Starting GEM Layout.");
	
	output->ShallowCopy(input);

	// Allocate a new set of points to hold this layout.
	layout = vtkPoints::New();
	layout->SetDataTypeToDouble();
	layout->SetNumberOfPoints(input->GetNumberOfNodes());
	output->SetLayout(layout);
	

	this->SetGEMParameters();
	
  this->NodeCount = input->GetNumberOfNodes();
	this->In = new vtkIdType[this->NodeCount];
	this->Ximpulse = new double[this->NodeCount];
	this->Yimpulse = new double[this->NodeCount];
	this->Direction = new double[this->NodeCount];
	this->Heat = new double[this->NodeCount];
	this->Mass = new double[this->NodeCount];

	this->Mark = vtkBitArray::New();
	this->Mark->SetNumberOfTuples(this->NodeCount);

  nodes = input->GetNodes();
  while (nodes->HasNext()) 
    {
		node = nodes->GetNext();
		index = input->GetNodeIndex(node);
		this->In[index] = 0;
		this->Ximpulse[index] = 0.0;
		this->Yimpulse[index] = 0.0;
		this->Direction[index] = 0.0;
		this->Heat[index] = 0.0;
		this->Mass[index] = 0.0;
		this->Mark->SetValue(index, 0);
		layout->SetPoint(index, (double)rand(), (double)rand(), (double)rand());
    }
  nodes->Delete();

	this->Map = NULL;
	this->Q = NULL;
  if (i_finaltemp < i_starttemp)
		{
    this->Insert(output);
		}
	if (a_finaltemp < a_starttemp)
		{
		this->Arrange(output);
		}

	// Deallocate working arrays.
  if (this->Map) 
		{
		delete [] this->Map;
		this->Map = NULL;
		}
  if (this->Q) 
		{
		delete this->Q;
		this->Q = NULL;
		}
	delete [] this->In;
	delete [] this->Ximpulse;
	delete [] this->Yimpulse;
	delete [] this->Direction;
	delete [] this->Heat;
	delete [] this->Mass;
	this->Mark->Delete();

  vtkDebugMacro(<< "GEM Layout Done.");
}

