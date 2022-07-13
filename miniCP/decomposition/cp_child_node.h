/// \file 
/// \brief Definitions related to the CP_Child_Node class

#ifndef CP_CHILD_NODE_H
#define CP_CHILD_NODE_H

#include <child_node.h>
#include "csp.h"
#include "information.h"

 
class CP_Child_Node: public Child_Node    /// This class allows to represent a specialization of the Child_Node class for solving CSP or #CSP instances \ingroup decomposition
{
	protected:
		vector<vertex> proper_variable; ///< the proper variable of the cluster (required in order to record goods on the whole cluster)
		int proper_variable_number;						///< the number of proper variables
		int * tuple;													///< the tuple which will be used for checking if an assignment corresponds to a (no)good (without allocating space at each check)
	
	public:
		// constructor and destructor
		CP_Child_Node (Child_Node & cn, Cluster_Node * cl, CSP * pb);  ///< constructs a CP child node by copying the child node cn
		~CP_Child_Node ();		///< destructor
};



#endif

