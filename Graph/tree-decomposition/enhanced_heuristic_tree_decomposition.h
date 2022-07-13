/// \file 
/// \brief Definitions related to the Enhanced_Heuristic_Tree_Decomposition class

#ifndef ENHANCED_HEURISTIC_TREE_DECOMPOSITION_H
#define ENHANCED_HEURISTIC_TREE_DECOMPOSITION_H

#include "tree_decomposition.h"
#include "clique_first_cluster_heuristic.h"
#include "sep_k_neighbors_next_vertices_heuristic.h"

class Enhanced_Heuristic_Tree_Decomposition: public Tree_Decomposition     /// This class allows to compute a tree-decomposition thanks to heuristic methods (enhanced version)  \ingroup tree-decomposition
{
	protected:
		Clique_First_Cluster_Heuristic * first_cluster;    ///< the heuristic exploited for computing the first cluster
		Sep_K_Neighbors_Next_Vertices_Heuristic * next_vertices;    ///< the heuristic exploited for choosing the next vertices
		
	public:
		// constructors and destructor
		Enhanced_Heuristic_Tree_Decomposition (Graphical_Structure * gs, Clique_First_Cluster_Heuristic * fc, Sep_K_Neighbors_Next_Vertices_Heuristic * nv); ///< constructs a tree-decomposition related to the graphical structure gs by exploiting the heuristic fc for computing the first cluster and the heuristic nv for choosing the next vertex to consider
		~Enhanced_Heuristic_Tree_Decomposition ();													///< destructor
				
		// basic functions
		void Compute ();      ///< computes a tree-decomposition of the graphical structure
};

#endif
