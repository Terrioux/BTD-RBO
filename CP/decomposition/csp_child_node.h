/// \file 
/// \brief Definitions related to the CSP_Child_Node class

#ifndef CSP_CHILD_NODE_H
#define CSP_CHILD_NODE_H

#include "cp_child_node.h"
#include "assignment.h"

 
class CSP_Child_Node: public CP_Child_Node    /// This class allows to represent a specialization of the Child_Node class for solving CSP instances \ingroup decomposition
{
	protected:
		int unassigned_variable_number;				///< the number of unassigned variables in the child node
	
	public:
		// constructor and destructor
		CSP_Child_Node (Child_Node & cn, Cluster_Node * cl, CSP * pb);  ///< constructs a CSP child node by copying the child node cn
		virtual ~CSP_Child_Node ();		///< destructor
		
		// basic functions
		void Inc_Unassigned_Variable_Number ();		///< increases by one the number of unassigned variables in the child node
		void Dec_Unassigned_Variable_Number ();		///< decreases by one the number of unassigned variables in the child node
		int Get_Unassigned_Variable_Number ();		///< returns the number of unassigned variables in the child node
		
		// good/nogood functions
		virtual bool Is_Good (Assignment & A) = 0;				///< returns true if the assignment A corresponds to a good w.r.t. the current child node
		virtual void Record_Good (Assignment & A) = 0;		///< records the assignment A as new good w.r.t. the current child node
		virtual void Add_Good_To_Assignment (Assignment & A) = 0;	///< adds to the assignment A the good corresponding to the assignment (if it exists)
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void CSP_Child_Node::Inc_Unassigned_Variable_Number ()
// increases by one the number of unassigned variables in the child node
{
	unassigned_variable_number++;
}


inline void CSP_Child_Node::Dec_Unassigned_Variable_Number ()
// decreases by one the number of unassigned variables in the child node
{
	unassigned_variable_number--;
}


inline int CSP_Child_Node::Get_Unassigned_Variable_Number ()
// decreases by one the number of unassigned variables in the child node
{
	return unassigned_variable_number;
}

#endif

