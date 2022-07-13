/// \file 
/// \brief Definitions related to the Cluster_Heuristic class

#ifndef CLUSTER_HEURISTIC_H
#define CLUSTER_HEURISTIC_H

using namespace std;

#include "csp_tree_decomposition.h"
#include "csp.h"

class Cluster_Heuristic       /// This class allows to represent the choice of the next cluster \ingroup decomposition
{
	public:
    // destructor
    virtual ~Cluster_Heuristic (){};  ///< destructor
  
		// basic functions
		virtual Cluster_Heuristic * Duplicate ()=0;							///< duplicates the cluster heuristic
		virtual Cluster_Node * Choose_Next_Cluster (Cluster_Tree * ct, Cluster_Node * cl, CSP * pb, bool is_good[])=0;	///< returns the next cluster to process after the cluster cl
};

#endif
