/// \file 
/// \brief Definitions related to the Light_Graph class

#ifndef LIGHT_GRAPH_H
#define LIGHT_GRAPH_H

#include "graphical_structure.h"
#include <list>
#include <vector>


class Light_Graph: public Graphical_Structure           /// This class allows to represent graphs with adjacency list  \ingroup core
{
  protected : 
  	vector<list<vertex> > adjacency_list;   ///< the adjacency list of the graph
		
	public :
	  // constructors and destructor
		Light_Graph ();       					 							///< constructs an empty graph with 0 vertex and 0 edge
		Light_Graph (Light_Graph & g);         				///< constructs an graph by copying the graph g
		~Light_Graph ();		 					  		  				///< destructor

		// operator
		Light_Graph & operator= (Light_Graph & g); 		///< operator =

    // iterators
    typedef list<vertex>::iterator Neighbor_iterator;
		Neighbor_iterator Begin_Neighbor (vertex x); ///< returns an iterator referring to the first neighbor of the vertex x
		Neighbor_iterator End_Neighbor (vertex x);   ///< returns an iterator referring to the position after the last neighbor of the vertex x
    
    // basic operations or functions on graphs
		void Init (unsigned int nb_vertices) override; 					///< initializes the graph with n vertices and 0 edge
		void Add_Vertex () override;   													///< adds a vertex to the graph
		virtual bool Is_Edge (vertex x, vertex y);   						///< returns true if {x,y} is an edge of the graph, false otherwise
		void Add_Edge (vertex elements[], int arity) override; 	///< adds the vertices of elements as a new edge of the graph if the value of arity is 2, otherwise adds these vertices as a new clique of the graph
		virtual void Add_Edge (vertex x, vertex y);   					///< adds the edge {x,y} to the graph
		virtual void Add_Edge_End (vertex x, vertex y); 				///< adds the edge {x,y} to the graph at the end of adjacency list
		virtual void Remove_Edge (vertex x, vertex y); 				  ///< removes the edge {x,y} to the graph


    // miscellaneous functions
    Light_Graph * Get_Primal_Light_Graph () override;	 ///< returns the primal graph related to the graphical structures as a pointer on a Light_Graph object (i.e. a copy of the graph itself)
		Graph * Get_Primal_Graph () override;							 ///< returns the primal graph related to the graphical structures as a pointer on a Graph object
};


//-----------------------------
// inline function definitions
//-----------------------------

//-----------
// iterators
//-----------


inline list<vertex>::iterator Light_Graph::Begin_Neighbor (vertex x)
// returns an iterator referring to the first neighbor of the vertex x
{
	return adjacency_list[x].begin();
}


inline list<vertex>::iterator Light_Graph::End_Neighbor (vertex x)
// returns an iterator referring to the position after the last neighbor of the vertex x
{
	return adjacency_list[x].end();
}


inline Light_Graph * Light_Graph::Get_Primal_Light_Graph ()
// returns the primal graph related to the graphical structures as a pointer on the Light_Graph object itself
{
	return this;
}

#endif
