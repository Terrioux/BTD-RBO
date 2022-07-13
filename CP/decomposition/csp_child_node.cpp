#include "csp_child_node.h"
#include <typeinfo>


CSP_Child_Node::CSP_Child_Node (Child_Node & cn, Cluster_Node * cl, CSP * pb) : CP_Child_Node (cn,cl,pb)
// constructs a CSP child node by copying the child node cn
{
	unassigned_variable_number = size;
}


CSP_Child_Node::~CSP_Child_Node ()
// destructor
{
}
