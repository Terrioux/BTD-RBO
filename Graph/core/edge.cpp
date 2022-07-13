#include "edge.h"

//-----------------------------
// constructors and destructor
//-----------------------------

Edge::Edge (int num_edge, vertex elements[], int arity)
// constructs an edge whose number is num_edge and which involves the vertices from the array elements
{
	num = num_edge;
	
	for (int i = 0;	i < arity; i++)
		scope.insert (elements[i]);
}


Edge::Edge (Edge & e)
// constructs an edge by copying the edge e
{
	num = e.num;
	scope = e.scope;
}
		
Edge::~Edge ()
// destructor
{
}


//----------
// operator
//----------

Edge & Edge::operator= (const Edge & e)
// operator =
{
	if (this != &e)
	{
		num = e.num;
		scope = e.scope;
	}
	return * this;	
}


bool Edge::operator== (const Edge & e)
// operator ==
{
	return scope == e.scope;
}

//-----------------------------------------
// basic operations or functions on graphs
//-----------------------------------------


vertex Edge::Get_Element (int n)
// returns the nth vertex of the scope
{
	if ((n >= 0) && ((unsigned int) n <= scope.size()))
	{
		set<vertex>::iterator iter;
		int i = 0;
		for (iter = scope.begin(); i < n; iter++)
			i++;
		return *iter;
	}
	else return -1;
}


