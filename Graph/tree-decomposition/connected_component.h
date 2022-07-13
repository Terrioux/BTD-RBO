/// \file 
/// \brief Definitions related to the Connected_Component class

#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include "graph.h"
#include <stack>
#include <tuple>
#include <list>
#include "cluster_node.h"

class Connected_Component     /// This class allows to represent the connected components of graphical structure  \ingroup tree-decomposition
{
	protected:
		Light_Graph * g;	///< the graph corresponding to the considered graphical structure (i.e. the graph itself if the graphical structure is a graph, its primal graph if it is a (multi)-hypergraph)
		
	public:
		// constructor and destructor
		Connected_Component (Graphical_Structure * gs);	  ///< constructs a related to the graphical structure gs
		~Connected_Component ();	  ///< destructor
				
		// basic functions
		bool Is_Connected (pair<int,vertex *> & vs);									///< returns true if the graphical substructure induced by the vertex set vs is connected, false otherwise. vs is defined as a pair (number of vertices,array of vertices)
		list<pair<int,vertex *> > Get_Connected_Components ();	///< returns the list of connected components of the related graphical structure
		list<pair<int,vertex *> > Get_Connected_Components (pair<int,vertex *> & vs, pair<int,vertex *> & except_vs);		///< returns the list of connected components of the related graphical substructure induced by the vertex set vs - except_vs
		void Get_Connected_Components_With_Neighborhood (pair<int,vertex *> & vs, Cluster_Node * except_vs, stack<tuple<pair<int,vertex *>,Cluster_Node *, pair<int,vertex *>>> & S); ///< puts in the stack S the list of both connected components of the related graphical substructure induced by the vertex set vs - except_vs, and their related neighborhood in except_vs
		void Get_Connected_Components_From_Set (Cluster_Node * initial_vs, stack<tuple<pair<int,vertex *>,Cluster_Node *, pair<int,vertex *>>> & S);		///< puts in the stack S the list of both connected components of the related graphical substructure induced from the vertex set initial_vs, and their related neighborhood in initial_vs
		void Get_Connected_Components_From_Set (Cluster_Node * initial_vs, stack<tuple<pair<int,vertex *>,Cluster_Node *, pair<int,vertex *>>> & S, bool is_untreated []);		///< puts in the stack S the list of both connected components of the related graphical substructure induced from the vertex set initial_vs, and their related neighborhood in initial_vs
		
		bool Induce_Bounded_Neighborhood (pair<int,vertex *> & vs, Cluster_Node * except_vs, int k); 			///< returns true if all the connected components of the related graphical substructure induced by the vertex set vs - except_vs has a neighborhood in except_vs whose size does not exceed k; false otherwise
};

#endif

