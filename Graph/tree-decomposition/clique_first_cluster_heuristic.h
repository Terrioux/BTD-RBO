/// \file 
/// \brief Definitions related to the Clique_First_Cluster_Heuristic class

#ifndef CLIQUE_FIRST_CLUSTER_HEURISTIC_H
#define CLIQUE_FIRST_CLUSTER_HEURISTIC_H

#include "graphical_structure.h"
#include "cluster_node.h"

class Clique_First_Cluster_Heuristic  /// This class defines a heuristic which chooses as first cluster a clique of the primal graph of the graphical structure  \ingroup tree-decomposition
{
	protected:
    Graphical_Structure * g;    ///< the graphical structure we want to decompose
		bool with_initial_v;    		///< true if an initial vertex is provided, false otherwise
		vertex initial_v;			    	///< the first vertex of the clique
	
	public:
		// constructors and destructor
		Clique_First_Cluster_Heuristic (Graphical_Structure * gs);	 							  ///< constructs a heuristic which chooses a clique (of the primal graph of the graphical structure gs) containing the vertex having the largest degree
		Clique_First_Cluster_Heuristic (Graphical_Structure * gs, vertex init_v);	  ///< constructs a heuristic which chooses a clique (of the primal graph of the graphical structure gs) containing the vertex init_v
		
		// basic functions
		pair<int,vertex *> Choose ();							  			///< returns the first cluster for a tree-decomposition according to a heuristic method. This cluster must be connected
		Cluster_Node * Choose (bool is_available[]);			///< returns the first cluster for a tree-decomposition according to a heuristic method. This cluster must be connected and its vertices must be chosen among the vertices for which the flag is_available is set to true. For each selected vertex, is_available is set to false
};


#endif
