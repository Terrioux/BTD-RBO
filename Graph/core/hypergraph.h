/// \file 
/// \brief Definitions related to the Hypergraph class

#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

#include "multi_hypergraph.h"


class Hypergraph: public Multi_Hypergraph          /// This class allows to represent hypergraphs with no redundant edges  \ingroup core
{		
	public :
	  // constructors and destructor
		Hypergraph ();       					 							   ///< constructs an empty hypergraph with 0 vertex and 0 edge
		Hypergraph (Hypergraph & h);	           		   ///< constructs a hypergraph by copying the hypergraph h
		~Hypergraph ();		 					  		  					 ///< destructor

		// operator
		Hypergraph & operator= (Hypergraph & h);       ///< operator =
    
    // basic operations or functions on hypergraphs
		void Add_Edge (vertex elements[], int arity) override;   	  ///< adds the vertices of elements as a new edge of the hypergraph
		void Add_Edge (Edge & e) override;  												///< adds a copy of the edge e as a new edge of the hypergraph
		void Add_Edge_End (vertex elements[], int arity) override;  ///< adds the vertices of elements as a new edge of the hypergraph (the edge is added at the end of adjacency list)
		void Add_Edge_End (Edge & e) override;  										///< adds a copy of the edge e as a new edge of the hypergraph (the edge is added at the end of adjacency list)
};

#endif


