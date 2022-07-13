/// \file 
/// \brief Definitions related to the Sep_K_Neighbors_Next_Vertices_Heuristic class

#ifndef SEP_K_NEIGHBORS_NEXT_VERTICES_HEURISTIC_H
#define SEP_K_NEIGHBORS_NEXT_VERTICES_HEURISTIC_H

#include "graph.h"
#include "cluster_node.h"

class Sep_K_Neighbors_Next_Vertices_Heuristic   /// This class implements the refined heuristic H4 of ICTAI'15  \ingroup tree-decomposition
{
	protected:
    Light_Graph * g;		///< the original graph
		int k;              ///< the maximal size which is allowed for each separator
	
	public:
		// constructors and destructor
		Sep_K_Neighbors_Next_Vertices_Heuristic (Light_Graph * ref_g, int sep_k);					///< constructs a heuristic which chooses vertices in the neighborhood (w.r.t. the graph ref_g) of a set of vertices 
		
		// basic functions
		void Choose (pair<int,vertex *> & vs, Cluster_Node * cl, pair<int,vertex *> & sep, int current_w, bool is_available[]);			///< returns as next vertices all neighbors of separator until a certain distance d where all the connected components induced have separator <= k (CC are isolated as soon as possible)
};


#endif
