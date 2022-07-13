#include "graphical_structure.h"
#include "light_graph.h"
#include "graph.h"


// ---------------------------
// constructor and destructor
//----------------------------

Graphical_Structure::Graphical_Structure()
// builds an empty graphical structure
{
	n = 0;
	m = 0;
	plg = 0;
	pg = 0;
}

Graphical_Structure::~Graphical_Structure()
// destructor
{
	delete plg;
	delete pg;
}


// ----------------
// basic functions
// ----------------



void Graphical_Structure::Add_Edge (set<vertex> & e)
// adds the vertices of e as a new edge of the hypergraph
{
	vertex elements [e.size()];
	int i = 0;
	
	for (set<vertex>::iterator iter = e.begin(); iter != e.end(); iter++)
	{
		elements[i] = *iter;
		i++;
	}
	
	Add_Edge (elements,e.size());
}

