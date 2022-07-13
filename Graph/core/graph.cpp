#include "graph.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stack>
#include <set>
using namespace std;

//-----------------------------
// constructors and destructor
//-----------------------------

Graph::Graph (): Light_Graph()
// constructs an empty graph with 0 vertex and 0 edge
{
}


Graph::Graph (Graph & g)
// constructs an graph by copying the graph g
{
	// initialization of the graph with g.n vertices and 0 edge
	n = g.n;
	m = 0;
	plg = 0;
	pg = 0;

	adjacency_list.resize (n);
	adjacency_matrix.resize (n);
	degree.resize (n,0);
	
	vertex x;
	
	for (x = 0; x < n; x++)
	  adjacency_matrix[x].resize (n,false);
	  
	// copy of the edges of the graph g
	for (x = 0; x < n; x++)
  {
    for (Neighbor_iterator iter = g.adjacency_list[x].begin(); iter != g.adjacency_list[x].end(); iter++)
      if (x < *iter)
        Add_Edge (x,*iter);
  }
}


Graph::Graph (Light_Graph & g)
// constructs an graph by copying the graph g
{
	// initialization of the graph with g.n vertices and 0 edge
	n = g.N();
	m = 0;
	plg = 0;
	pg = 0;

	adjacency_list.resize (n);
	adjacency_matrix.resize (n);
	degree.resize (n,0);
	
	vertex x;
	
	for (x = 0; x < n; x++)
	  adjacency_matrix[x].resize (n,false);
	  
	// copy of the edges of the graph g
	for (x = 0; x < n; x++)
  {
    for (Neighbor_iterator iter = g.Begin_Neighbor (x); iter != g.End_Neighbor(x); iter++)
      if (x < *iter)
        Add_Edge (x,*iter);
  }	
}


Graph::~Graph ()
// destructor
{
}


//----------
// operator
//----------


Graph & Graph::operator= (Graph & g)
// operator =
{
  if (this != &g)
  {
		// initialization of the graph with g.n vertices and 0 edge
		n = g.n;
		m = 0;
		delete plg;
		plg = 0;
		pg = 0;

		adjacency_list.resize(n);
		adjacency_matrix.resize(n);
		degree.resize (n,0);
		
		vertex x;
		for (x = 0; x < n; x++)
			adjacency_matrix[x].resize (n,false);
			
		// copy of the edges of the graph g
		
		for (x = 0; x < n; x++)
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


void Graph::Init (unsigned int nb_vertices)
// initializes the graph with n vertices and 0 edge
{
	vertex x;
	n = nb_vertices;
	m = 0;
	
	adjacency_list.resize (n);
	adjacency_matrix.resize(n);
	degree.resize (n,0);
	
	for (x = 0; x < nb_vertices; x++)
	  adjacency_matrix[x].resize (n,false);
}

void Graph::Add_Vertex ()
// adds a vertex to the graph
{
	n++;
	adjacency_list.resize(n);
	adjacency_matrix.resize(n);
	degree.resize (n,0);
	
	for (vertex x = 0; x < n-1; x++)
	  adjacency_matrix[x].resize (n,false);
	adjacency_matrix[n-1].resize (n,false);
	delete plg;
	plg = 0;
	pg = 0;
}


void Graph::Add_Edge (vertex x, vertex y)
// adds the edge {x,y} to the graph
{
	if (! adjacency_matrix[x][y])
	{
		adjacency_list[x].push_front (y);
		adjacency_list[y].push_front (x);
		adjacency_matrix[x][y] = true;
		adjacency_matrix[y][x] = true;
		degree[x]++;
		degree[y]++;
		delete plg;
		plg = 0;
		pg = 0;
		m++;
	}
}


void Graph::Add_Edge_End (vertex x, vertex y)
// adds the edge {x,y} to the graph at the end of adjacency list
{
	if (! adjacency_matrix[x][y])
	{
		adjacency_list[x].push_back (y);
		adjacency_list[y].push_back (x);
		adjacency_matrix[x][y] = true;
		adjacency_matrix[y][x] = true;
		degree[x]++;
		degree[y]++;
		delete plg;
		plg = 0;
		pg = 0;
		m++;
	}
}


void Graph::Remove_Edge (vertex x, vertex y)  
// removes the edge {x,y} to the graph
{
	if (adjacency_matrix[x][y])
	{
		adjacency_list[x].remove (y);
		adjacency_list[y].remove (x);
		adjacency_matrix[x][y] = false;
		adjacency_matrix[y][x] = false;
		degree[x]--;
		degree[y]--;
		delete plg;
		plg = 0;
		pg = 0;
		m--;
	}
}
