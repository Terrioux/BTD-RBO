/// \file 
/// \brief Definitions related to the Root_Heuristic class

#ifndef ROOT_HEURISTIC_H
#define ROOT_HEURISTIC_H

using namespace std;

#include <tree_decomposition.h>
#include "csp.h"

class Root_Heuristic      /// This class allows to represent heuristics for choosing the root cluster among the clusters of a given tree-decomposition \ingroup decomposition
{
	public:
		// destructor
		virtual ~Root_Heuristic() {};	    	///< destructor
	
		// basic functions
		virtual int Choose_Root (Cluster_Tree * ct, CSP * pb)=0;		///< returns the choice of the root cluster according to the considered heuristic
		virtual Root_Heuristic * Duplicate (CSP * pb)=0;						///< duplicates the root cluster heuristic w.r.t. the CSP pb
};

#endif

