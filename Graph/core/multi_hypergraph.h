/// \file 
/// \brief Definitions related to the Multi_Hypergraph class

#ifndef MULTI_HYPERGRAPH_H
#define MULTI_HYPERGRAPH_H

#include "graphical_structure.h"
#include "edge.h"
#include "graph.h"
#include <list>
#include <vector>
#include <map>


class Multi_Hypergraph : public Graphical_Structure     /// This class allows to represent hypergraphs with eventually redundant edges  \ingroup core
{
  protected : 
		int max_arity;											 	        ///< the arity of the largest edge of the hypergraph
		map<int,Edge *>	edges;							      		///< the list of edges reachable by their number
  	vector<list<Edge *> >  adjacency_list;        ///< the adjacency list of the hypergraph
    bool recompute_primal_graph;                  ///< true if the (light) primal graph must be recomputed, false otherwise
		
	public :
	  // constructors and destructor
		Multi_Hypergraph ();       					 							   ///< constructs an empty hypergraph with 0 vertex and 0 edge
		Multi_Hypergraph (Multi_Hypergraph & h);       		   ///< constructs a hypergraph by copying the hypergraph h
		~Multi_Hypergraph ();		 					  		  					 ///< destructor

		// operator
		Multi_Hypergraph & operator= (Multi_Hypergraph & h); ///< operator =

    // iterators
    map<int,Edge*>::iterator Begin_Edge_List ();         ///< returns an iterator referring to the first edge of the hypergraph
		map<int,Edge*>::iterator End_Edge_List ();           ///< returns an iterator referring to the position after the last edge of the hypergraph
		
    list<Edge *>::iterator Begin_Edge_List (vertex x);   ///< returns an iterator referring to the first edge in the adjacency list of the vertex x
		list<Edge *>::iterator End_Edge_List (vertex x);     ///< returns an iterator referring to the position after the last edge in the adjacency list of the vertex x
    
    // basic operations or functions on hypergraphs
		void Init (unsigned int nb_vertices) override;  		       ///< initializes the hypergraph with n vertices and 0 edge
		virtual void Add_Vertex () override;   									   ///< adds a vertex to the hypergraph
		int Get_Max_Arity ();												        			 ///< returns the arity of the largest edge
    Edge * Get_Edge(unsigned num);                             ///< returns the edge whose number is num
		bool Is_Edge (vertex elements[], int arity);  			       ///< returns true if the vertices of elements forms an edge of the hypergraph, false otherwise
		bool Is_Edge (Edge & e);        													 ///< returns true if the edge e is an edge of the hypergraph, false otherwise
		bool Is_Edge (set<vertex> elements);											 ///< returns true if the edge e is an edge of the hypergraph, false otherwise
		void Add_Edge (vertex elements[], int arity) override;   	 ///< adds the vertices of elements as a new edge of the hypergraph
		virtual void Add_Edge (Edge & e); 											 	 ///< adds a copy of the edge e as a new edge of the hypergraph
		virtual void Add_Edge_End (vertex elements[], int arity);  ///< adds the vertices of elements as a new edge of the hypergraph (the edge is added at the end of adjacency list)
		virtual void Add_Edge_End (Edge & e);											 ///< adds a copy of the edge e as a new edge of the hypergraph (the edge is added at the end of adjacency list)
		void Remove_Edge (vertex elements[], int arity); 				   ///< removes the edge formed by the vertices of elements from the hypergraph
		void Remove_Edge (Edge & e);														   ///< removes the edge e from the hypergraph
		void Remove_Edge (unsigned int num);			  						   ///< removes the edge whose number is num
		void Add_Vertex_In_Edge (vertex v, int num);					  	 ///< adds a vertex to the edge whose number is num
    void Reset_Edge (int num);                                 ///< removes all the vertices from the edge e from the hypergraph

    // miscellaneous functions
		virtual Light_Graph * Get_Primal_Light_Graph () override;	 ///< returns the primal graph related to the graphical structures as a pointer on a Light_Graph object
		virtual Graph * Get_Primal_Graph () override;							 ///< returns the primal graph related to the graphical structures as a pointer on a Graph object
};


//-----------------------------
// inline function definitions
//-----------------------------


//-----------
// iterators
//-----------

inline map<int,Edge*>::iterator Multi_Hypergraph::Begin_Edge_List ()
// returns an iterator referring to the first edge of the hypergraph
{
  return edges.begin();
}


inline map<int,Edge*>::iterator Multi_Hypergraph::End_Edge_List ()
// returns an iterator referring to the position after the last edge of the hypergraph
{
  return edges.end();
}



inline list<Edge *>::iterator Multi_Hypergraph::Begin_Edge_List (vertex x)
// returns an iterator referring to the first edge in the adjacency list of the vertex x
{
	return adjacency_list[x].begin();
}


inline list<Edge *>::iterator Multi_Hypergraph::End_Edge_List (vertex x)
// returns an iterator referring to the position after the last edge in the adjacency list of the vertex x
{
	return adjacency_list[x].end();
}


//----------------
// basic function
//----------------


inline int Multi_Hypergraph::Get_Max_Arity ()
// returns the arity of the largest edge
{
	return max_arity;
}


inline Edge * Multi_Hypergraph::Get_Edge(unsigned num)
// returns the edge whose number is num
{
	map<int,Edge *>::iterator iter = edges.find (num);
	
	if (iter != edges.end())
    return iter->second;
  else return 0;
}

#endif
