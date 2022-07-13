#include "light_graph.h"
#include "graph.h"
#include <stack>

//-----------------------------
// constructors and destructor
//-----------------------------

Light_Graph::Light_Graph (): Graphical_Structure()
// constructs an empty graph with 0 vertex and 0 edge
{
}


Light_Graph::Light_Graph (Light_Graph & g)
// constructs an graph by copying the graph g
{
	// initialization of the graph with g.n vertices and 0 edge
	n = g.n;
	m = 0;
	plg = 0;
	pg = 0;

	adjacency_list.resize (n);
	degree.resize (n,0);
	  
	// copy of the edges of the graph g
	
	for (vertex x = 0; x < n; x++)
  {
    for (Neighbor_iterator iter = g.adjacency_list[x].begin(); iter != g.adjacency_list[x].end(); iter++)
      if (x < *iter)
        Add_Edge (x,*iter);
  }
}


Light_Graph::~Light_Graph ()
// destructor
{
}


//----------
// operator
//----------


Light_Graph & Light_Graph::operator= (Light_Graph & g)
// operator =
{
  if (this != &g)
  {
		// initialization of the graph with g.n vertices and 0 edge
		n = g.n;
		m = 0;
		plg = 0;
		delete pg;
		pg = 0;

		adjacency_list.resize(n);
		degree.resize (n,0);

			
		// copy of the edges of the graph g
		
		for (vertex x = 0; x < n; x++)
		{
			for (Neighbor_iterator iter = g.adjacency_list[x].begin(); iter != g.adjacency_list[x].end(); iter++)
				if (x < *iter)
					Add_Edge (x,*iter);
		} 
	}
	
	return *this;
}



//----------------------------
// basic operations on graphs
//----------------------------


void Light_Graph::Init (unsigned int nb_vertices)
// initializes the graph with n vertices and 0 edge
{
	n = nb_vertices;
	m = 0;
	adjacency_list.resize (n);
	degree.resize (n,0);
}


void Light_Graph::Add_Vertex ()
// adds a vertex to the graph
{
	n++;
	adjacency_list.resize (n);
	degree.resize (n,0);
}


bool Light_Graph::Is_Edge (vertex x, vertex y)
// returns true if {x,y} is an edge of the graph, false otherwise
{
	Neighbor_iterator iter;
	for (iter = adjacency_list[x].begin(); (iter != adjacency_list[x].end()) && (*iter != y); iter++);
	
  return iter != adjacency_list[x].end();
}


void Light_Graph::Add_Edge (vertex elements[], int arity)
// adds the vertices of elements as a new edge of the graph if the value of arity is 2, otherwise adds these vertices as a new clique of the graph
{
	int i,j;
	for (i=0; i < arity; i++)
		for (j=i+1; j < arity; j++)
			Add_Edge (elements[i],elements[j]);
}


void Light_Graph::Add_Edge (vertex x, vertex y)
// adds the edge {x,y} to the graph
{
	if (! Is_Edge (x,y))
	{
		adjacency_list[x].push_front (y);
		adjacency_list[y].push_front (x);
		degree[x]++;
		degree[y]++;
		m++;
		plg = 0;
		delete pg;
		pg = 0;
	}
}


void Light_Graph::Add_Edge_End (vertex x, vertex y)
// adds the edge {x,y} to the graph at the end of adjacency list
{
	if (! Is_Edge (x,y))
	{
		adjacency_list[x].push_back (y);
		adjacency_list[y].push_back (x);
		degree[x]++;
		degree[y]++;
		m++;
		plg = 0;
		delete pg;
		pg = 0;
	}
}


void Light_Graph::Remove_Edge (vertex x, vertex y)  
// removes the edge {x,y} to the graph
{
	if (Is_Edge (x,y))
	{
		adjacency_list[x].remove (y);
		adjacency_list[y].remove (x);
		degree[x]--;
		degree[y]--;
		m--;
		plg = 0;
		delete pg;
		pg = 0;
	}
}




//------------------------
// miscellaneous functions
//------------------------


Graph * Light_Graph::Get_Primal_Graph ()
// returns the primal graph related to the graphical structures as a pointer on a Graph object
{
	if (pg == 0)
		pg =  new Graph (*this);
		
	return pg;
}
