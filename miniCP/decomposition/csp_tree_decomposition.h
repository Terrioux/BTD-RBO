/// \file 
/// \brief Definitions related to the CSP_Tree_Decomposition class


#ifndef CSP_TREE_DECOMPOSITION_H
#define CSP_TREE_DECOMPOSITION_H

#include <tree_decomposition.h>
#include "csp.h"


class CSP_Tree_Decomposition: public Tree_Decomposition     /// This class implements a specialization of the Tree_Decomposition class for solving CSP instances \ingroup decomposition
{
	public:
		// constructors
		CSP_Tree_Decomposition (Tree_Decomposition * td, CSP * pb, bool with_nogood_set);  ///< constructs a CSP tree-decomposition from a tree-decomposition
    
    // basic function
    bool Is_Valid (CSP * pb);   ///< returns true if the current tree-decomposition is valid, false otherwise (nogood base constraints of pb are ignored)
    void Select_First_Cluster_Tree (int num);   ///< selects as a first cluster tree one which contains the variables whose number is num
};


#endif

