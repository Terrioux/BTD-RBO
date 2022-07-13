/// \file 
/// \brief Definitions related to the Max_Ratio_Root_Heuristic class

#ifndef MAX_RATIO_ROOT_HEURISTIC_H
#define MAX_RATIO_ROOT_HEURISTIC_H

#include "root_heuristic.h"

class Max_Ratio_Root_Heuristic: public Root_Heuristic   /// This class implements a heuristic which chooses as first cluster a cluster having the maximum ratio number of constraints / (size-1) \ingroup decomposition
{
	public:
		// basic function
		int Choose_Root (Cluster_Tree * ct, CSP * pb) override;		///< returns the choice of the root cluster according to the considered heuristic
		Root_Heuristic * Duplicate (CSP * pb) override;						///< duplicates the root cluster heuristic w.r.t. the CSP pb
};


//-------------------------------
// definition of inline function
//-------------------------------


inline Root_Heuristic * Max_Ratio_Root_Heuristic::Duplicate (CSP * pb)
// duplicates the root cluster heuristic w.r.t. the CSP pb
{
	return new Max_Ratio_Root_Heuristic;
}

#endif

