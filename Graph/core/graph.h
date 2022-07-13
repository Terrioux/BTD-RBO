/// \file 
/// \brief Definitions related to the Graph class

#ifndef GRAPH_H
#define GRAPH_H

#include "light_graph.h"
#include <vector>
#include <string>

class Graph: public Light_Graph             /// This class allows to represent graphs with both adjacency list and adjacency matrix  \ingroup core
{
  protected : 
		vector<vector<bool> > adjacency_matrix; ///< the adjacency matrix of the graph

		
	public :
	  // constructors and destructor
		Graph ();       					 							///< constructs an empty graph with 0 vertex and 0 edge
		Graph (Graph & g);	            				///< constructs an graph by copying the graph g
		Graph (Light_Graph & g);         				///< constructs an graph by copying the graph g
		~Graph ();		 					  		  				///< destructor

		// operator
		Graph & operator= (Graph & g); 			    ///< operator =

    
    // basic operations or functions on graphs
		void Init (unsigned int nb_vertices) override;  	 ///< initializes the graph with n vertices and 0 edge
		void Add_Vertex () override;   								 		 ///< adds a vertex to the graph
		bool Is_Edge (vertex x, vertex y) override;   	   ///< returns true if {x overridey} is an edge of the graph override false otherwise
		void Add_Edge (vertex x, vertex y) override;  	 	 ///< adds the edge {x overridey} to the graph
		void Add_Edge_End (vertex x, vertex y) override; 	 ///< adds the edge {x overridey} to the graph at the end of adjacency list
		void Remove_Edge (vertex x, vertex y) override; 	 ///< removes the edge {x overridey} to the graph

		// miscellaneous function
		Light_Graph * Get_Primal_Light_Graph () override;	 ///< returns the primal graph related to the graphical structures as a pointer on a Light_Graph object
		Graph * Get_Primal_Graph () override;							 ///< returns the primal graph related to the graphical structures as a pointer on a Graph object (i.e. a copy of the graph itself)
};


//-----------------------------
// inline function definitions
//-----------------------------


inline bool Graph::Is_Edge (vertex x, vertex y)
// returns true if {x,y} is an edge of the graph, false otherwise
{
  return adjacency_matrix[x][y];
}


// miscellaneous function
inline Graph * Graph::Get_Primal_Graph ()
// returns the primal graph related to the graphical structures as a pointer on the Graph object itself 
{
	return this;
}


inline Light_Graph * Graph::Get_Primal_Light_Graph ()
// returns the primal graph related to the graphical structures as a pointer on a Light_Graph object
{
	return this;
}


#endif
