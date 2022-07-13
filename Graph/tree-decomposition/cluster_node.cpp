#include "cluster_node.h"
#include "child_node.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Cluster_Node::Cluster_Node (Graphical_Structure * gs) : Vertex_Set (gs)
// constructs an empty cluster node related to the graphical structure gs
{
	num = -1;
	separator = 0;
	parent = 0;
}


Cluster_Node::Cluster_Node (vector<vertex> element_set, Graphical_Structure * gs) : Vertex_Set (element_set,gs)
// constructs a vertex set containing the vertices of element_set and related to the graphical structure gs
{
	num = -1;
	separator = 0;
	parent = 0;
}


Cluster_Node::Cluster_Node (Cluster_Node & cn) : Vertex_Set (cn)
// constructs a cluster node by copying the cluster node cn
{
	num = cn.num;
	separator = cn.separator;
	parent = cn.parent;
}


Cluster_Node::~Cluster_Node ()
// destructor
{
}


//----------
// operator
//----------

Cluster_Node & Cluster_Node::operator= (const Cluster_Node & cn)
// operator =
{
	if (this != &cn)
	{
		g = cn.g;
		size = cn.size;
		elements = cn.elements;
		is_element = cn.is_element;
		nb_edges = cn.nb_edges;
		nb_partial_edges = cn.nb_partial_edges;
		nb_components = cn.nb_components;
		num = cn.num;
		separator = cn.separator;
		parent = cn.parent;
	}
	return *this;
}


//----------------
// basic function
//----------------


void Cluster_Node::Set_Separator (Child_Node * cn)
// sets the separator between the current cluster and its parent
{
	separator = cn;
	if (separator != 0)
		parent = separator->Get_Cluster();
}


int Cluster_Node::Get_Depth ()
// returns the depth
{
  int depth = 0;
  
  Cluster_Node * cl = this;
  while (cl->Get_Parent() != 0)
  {
    depth++;
    cl = cl->Get_Parent();
  }
  
	return depth;
}
