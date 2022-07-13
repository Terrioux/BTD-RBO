/// \file 
/// \brief Definitions related to the CSP_Cluster_Tree class

#ifndef CSP_CLUSTER_TREE_H
#define CSP_CLUSTER_TREE_H

#include <cluster_tree.h>
#include "csp.h"


class CSP_Cluster_Tree: public Cluster_Tree       /// This class implements a specialization of the Cluster_Tree class for solving CSP instances \ingroup decomposition
{
	public:
		// constructor and destructor
		CSP_Cluster_Tree (Cluster_Tree * ct, CSP * pb, bool with_nogood_set);																 ///< constructs a CSP cluster tree from an existing cluster tree
		~CSP_Cluster_Tree ();			///< destructor
		
		void Merge_Clusters (Cluster_Node * cn);		///< merges the cluster num with its parent (specialized version for CSP)
};


#endif

