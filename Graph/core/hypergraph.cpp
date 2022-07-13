#include "hypergraph.h"
#include <cstdlib>
#include <list>

//-----------------------------
// constructors and destructor
//-----------------------------


Hypergraph::Hypergraph () : Multi_Hypergraph()
// constructs an empty hypergraph with 0 vertex and 0 edge
{
}


Hypergraph::Hypergraph (Hypergraph & h)  : Multi_Hypergraph(h)
// constructs a hypergraph by copying the hypergraph h
{
}


Hypergraph::~Hypergraph ()
// destructor
{
}


//----------
// operator
//----------

Hypergraph & Hypergraph::operator= (Hypergraph & h)
// operator =
{
	if (this != &h)
  {
		vertex x;
		list<Edge *> to_be_deleted;
		
		for (x = 0; x < n; x++)
		{
			for (list<Edge *>::iterator iter = adjacency_list[x].begin(); iter != adjacency_list[x].end(); iter++)
				if (x == (*iter)->Get_Element(0))
					to_be_deleted.push_front(*iter);
		}
		
		for (list<Edge *>::iterator iter = to_be_deleted.begin(); iter != to_be_deleted.end(); iter++)
			delete *iter;

		for (x = 0; x < n; x++)
			adjacency_list[x].clear();
		
	
		// initialization of the hypergraph with h.n vertices and 0 edge
		n = h.n;
		m = 0;
		max_arity = 0;

		adjacency_list.resize (n);
		degree.resize (n,0);
					
		// copy of the edges of the hypergraph h
			
		for (x = 0; x < n; x++)
		{
			for (list<Edge *>::iterator iter = h.adjacency_list[x].begin(); iter != h.adjacency_list[x].end(); iter++)
      	if (x == (*iter)->Get_Element(0))
					Add_Edge (**iter);
		} 
	}
	
	return *this;
}


//----------------------------------------------
// basic operations or functions on hypergraphs
//----------------------------------------------


void Hypergraph::Add_Edge (vertex elements[], int arity)
// adds the vertices of elements as a new edge of the hypergraph
{
	if (! Is_Edge (elements,arity))
		Multi_Hypergraph::Add_Edge (elements,arity);
}


void Hypergraph::Add_Edge (Edge & e)
// adds a copy of the edge e as a new edge of the hypergraph
{
	if (! Is_Edge (e))
		Multi_Hypergraph::Add_Edge(e);
}


void Hypergraph::Add_Edge_End (vertex elements[], int arity)
// adds the vertices of elements as a new edge of the hypergraph (the edge is added at the end of adjacency list)
{
	if (! Is_Edge (elements,arity))
		Multi_Hypergraph::Add_Edge_End (elements,arity);
}


void Hypergraph::Add_Edge_End (Edge & e)
// adds a copy of the edge e as a new edge of the hypergraph (the edge is added at the end of adjacency list)
{
	if (! Is_Edge (e))
		Multi_Hypergraph::Add_Edge_End (e);
}
