/// \file 
/// \brief Definitions related to the Max_Weight_Sum_Root_Heuristic class

#ifndef MAX_WEIGHT_SUM_ROOT_HEURISTIC_H
#define MAX_WEIGHT_SUM_ROOT_HEURISTIC_H

#include "root_heuristic.h"
#include "erwa_weights.h"

class Max_Weight_Sum_Root_Heuristic: public Root_Heuristic    /// This class implements a heuristic which chooses as first cluster a cluster which maximizes the sum of the weights of the contraint whose scope intersects the cluster \ingroup decomposition
{
  protected:
    Erwa_Weights * weights;      ///< the weight of each variable
  
	public:
    // constructor
    Max_Weight_Sum_Root_Heuristic (Erwa_Weights * w);   ///< constructs the heuristics with its related weights w
  
		// basic functions
		int Choose_Root (Cluster_Tree * ct, CSP * pb) override;		///< chooses a root cluster which maximizes the sum of the weights of the contraint whose scope intersects the cluster
		Root_Heuristic * Duplicate (CSP * pb) override;						///< duplicates the root cluster heuristic w.r.t. the CSP pb
};


//-------------------------------
// definition of inline function
//-------------------------------


inline Root_Heuristic * Max_Weight_Sum_Root_Heuristic::Duplicate (CSP * pb)
// duplicates the root cluster heuristic
{
	return new Max_Weight_Sum_Root_Heuristic (weights);
}


#endif
