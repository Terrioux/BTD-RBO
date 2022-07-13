/// \file 
/// \brief Definitions related to the Min_Size_Separator_Cluster_Heuristic class

#ifndef MIN_SIZE_SEPARATOR_CLUSTER_HEURISTIC_H
#define MIN_SIZE_SEPARATOR_CLUSTER_HEURISTIC_H

using namespace std;

#include "cluster_heuristic.h"

class Min_Size_Separator_Cluster_Heuristic : public Cluster_Heuristic   /// This class implements the heuristic which chooses as next cluster one having the small separator with the current cluster \ingroup decomposition
{
	public:
		// basic functions
		Cluster_Heuristic * Duplicate () override;					///< duplicates the cluster heuristic
		Cluster_Node * Choose_Next_Cluster (Cluster_Tree * ct, Cluster_Node * cl, CSP * pb, bool is_good[]) override;	///< returns the next cluster to process after the cluster cl
};


inline Cluster_Heuristic * Min_Size_Separator_Cluster_Heuristic::Duplicate ()
// duplicates the cluster heuristic
{
	return new Min_Size_Separator_Cluster_Heuristic ();	
}

#endif
