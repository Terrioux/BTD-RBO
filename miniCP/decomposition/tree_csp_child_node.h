/// \file 
/// \brief Definitions related to the Tree_CSP_Child_Node class

#ifndef TREE_CSP_CHILD_NODE_H
#define TREE_CSP_CHILD_NODE_H

#include "csp_child_node.h"
#include "labeled_lexicographic_tree.h"

 
class Tree_CSP_Child_Node: public CSP_Child_Node      /// This class implements a specialization (based on lexicographic trees) of the Child_Node class for solving CSP instances \ingroup decomposition
{
	protected:
		Labeled_Lexicographic_Tree<vector<int>>  good_set;	///< the set of goods related to the child node
	
	public:
		// constructor and destructor
		Tree_CSP_Child_Node (Child_Node & cn, Cluster_Node * cl, CSP * pb, bool with_nogood_set);  ///< constructs a CSP child node by copying the child node cn and adding the good / nogood sets (expressed as lexicographic trees). The nogood set is only used if with_nogood_set is set to true
				
		// good/nogood functions
		bool Is_Good (Assignment & A) override;				///< returns true if the assignment A corresponds to a good w.r.t. the current child node
		void Record_Good (Assignment & A) override;		///< records the assignment A as new good w.r.t. the current child node
		void Add_Good_To_Assignment (Assignment & A) override;	///< adds to the assignment A the good corresponding to the assignment (if it exists)
};


//-----------------------------
// inline function definitions
//-----------------------------

inline bool Tree_CSP_Child_Node::Is_Good (Assignment & A)
// returns true if the assignment A corresponds to a good w.r.t. the current child node
{
	for (int i = 0; i < size; i++)
		tuple[i] = A.Get_Value (elements[i]);
	
	if (good_set.Is_Element (tuple,size))
		return true;
	else return false;	
}

#endif

