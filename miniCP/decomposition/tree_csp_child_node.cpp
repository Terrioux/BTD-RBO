#include "tree_csp_child_node.h"
#include <typeinfo>


Tree_CSP_Child_Node::Tree_CSP_Child_Node (Child_Node & cn, Cluster_Node * cl, CSP * pb, bool with_nogood_set)  : CSP_Child_Node (cn,cl,pb)
// constructs a CSP child node by copying the child node cn and adding the good / nogood sets (expressed as lexicographic trees). The nogood set is only used if with_nogood_set is set to true
{
}


//-----------------------
// good/nogood functions
//-----------------------


void Tree_CSP_Child_Node::Record_Good (Assignment & A)
// records the assignment A as new good w.r.t. the current child node
{
	for (int i = 0; i < size; i++)
		tuple[i] = A.Get_Value (elements[i]);
	
	vector<int> t (proper_variable_number);
	
	for (int k = 0; k < proper_variable_number; k++)
		t[k] = A.Get_Value (proper_variable[k]);

	good_set.Add_Element (tuple,size,t);
}


void Tree_CSP_Child_Node::Add_Good_To_Assignment (Assignment & A)
// adds to the assignment A the good corresponding to the assignment (if it exists)
{
	int i;

	for (i = 0; i < size; i++)
		tuple[i] = A.Get_Value (elements[i]);

	vector<int> * t = good_set.Get_Label (tuple,size);
	
	if (t != 0)
	{		
		for (int k = 0; k < proper_variable_number; k++)
		{
			if (! A.Is_Assigned (proper_variable[k]))
				A.Assign (proper_variable[k],(*t)[k]);
		}
	}
}
