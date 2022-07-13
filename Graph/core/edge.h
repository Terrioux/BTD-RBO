/// \file 
/// \brief Definitions related to the Edge class

#ifndef EDGE_H
#define EDGE_H

#include <set>
#include "graphical_structure.h"
using namespace std;

class Edge        /// This class allows to represent the edges of hypergraphs  \ingroup core
{
	private :
		int num;        				///< number of the edge
		set<vertex> scope;			///< the vertices which belong to the edge
		
	public :
		// constructors and destructor
		Edge (int num_edge, vertex elements[], int arity);  ///< constructs an edge whose number is num_edge and which involves the vertices from the array elements
		Edge (Edge & e);                    		            ///< constructs an edge by copying the edge e
		~Edge ();		 					  		  					 						///< destructor

		// operator
		Edge & operator= (const Edge & e); 									///< operator =
		bool operator== (const Edge & e); 									///< operator ==
		

		// iterators
		set<vertex>::iterator Begin();										///< returns an iterator referring to the first vertex of the vertex set
		set<vertex>::iterator End();										  ///< returns an iterator referring to the position after the vertex of the vertex set

		// basic operations or functions on graphs
		int Get_Num ();									///< returns the number of the edge
		void Set_Num (int n);						///< sets the number of the edge to the value n
		int Get_Arity();								///< returns the arity
		bool Is_Element(vertex x);			///< returns true if x belongs to the edge, false otherwise
		vertex Get_Element (int n);  		///< returns the nth vertex of the scope
		set<vertex> & Get_Scope();		  ///< returns the scope of the edge
		void Add_Vertex (vertex x);			///< adds the vertex x to the edge
		void Remove_Vertex (vertex x);	///< removes the vertex x from the edge
		void Reset ();	                ///< removes all the vertices of the edge
};


//-----------------------------
// inline function definitions
//-----------------------------


//-----------
// iterators
//-----------

inline set<vertex>::iterator Edge::Begin()
// returns an iterator referring to the first vertex of the vertex set
{
	return scope.begin();	
}


inline set<vertex>::iterator Edge::End()
// returns an iterator referring to the position after the vertex of the vertex set
{
	return scope.end();
}


//-----------------------------------------
// basic operations or functions on graphs
//-----------------------------------------


inline int Edge::Get_Num ()
// returns the number of the edge
{
	return num;
}


inline void Edge::Set_Num (int n)
// sets the number of the edge to the value n
{
	num = n;
}


inline int Edge::Get_Arity()
// returns the arity
{
	return scope.size();
}


inline bool Edge::Is_Element(vertex x)
// returns true if x belongs to the edge, false otherwise
{
	return scope.find(x) != scope.end();	
}


inline set<vertex> & Edge::Get_Scope()
// returns the scope of the edge
{
	return scope;
}


inline void Edge::Add_Vertex (vertex x)
// adds the vertex x to the edge
{
	scope.insert (x);
}
		
		
inline void Edge::Remove_Vertex (vertex x)
// removes the vertex x from the edge
{
	scope.erase (x);
}


inline void Edge::Reset ()
// removes all the vertices of the edge
{
	scope.clear ();
}


#endif

