#include "cp_child_node.h"
#include <typeinfo>


//----------------------------
// constructor and destructor
//----------------------------

CP_Child_Node::CP_Child_Node (Child_Node & cn, Cluster_Node * cl, CSP * pb) : Child_Node (cn)
// constructs a CSP child node by copying the child node cn
{
	proper_variable.resize (cl->Size() - Size());
	proper_variable_number = 0;
	
	for (vector<vertex>::iterator iter = cl->begin(); iter != cl->end(); iter++)
		if (! Is_Element (*iter))
		{
			proper_variable[proper_variable_number] = *iter;
			proper_variable_number++;
		}
		
	tuple = new int [size];	
}


CP_Child_Node::~CP_Child_Node ()
// destructor
{
	delete [] tuple;
}
