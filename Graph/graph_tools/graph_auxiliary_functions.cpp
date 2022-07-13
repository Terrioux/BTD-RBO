#include "graph_auxiliary_functions.h"
#include <vector>
#include <stack>


int Width (Light_Graph * g)
// returns the width of the graph g (see [Freuder,1982])
{ 
	set<vertex> remaining_vertices;       // the set of the remaining vertices 
  vertex v;
	vector<unsigned int> current_degree (g->N());	// the current degree of each vertex in the graph after the removal of some vertices
 
  for (v = 0; v < g->N(); v++)
  {
    current_degree[v]= g->Degree(v);
    if (current_degree[v] > 0)  remaining_vertices.insert (v);
  }
  
  // computation of the minimal width
  unsigned int width = 0;
  set<vertex>::iterator iter_set;
  Light_Graph::Neighbor_iterator iter;
  bool modified;
  
  while (remaining_vertices.size() != 0)
  {
    width++;
       
    do
    {
      modified = false;
			
      iter_set = remaining_vertices.begin(); 
      while (iter_set != remaining_vertices.end())
      {
        if (current_degree[*iter_set] <= width)
        {
					v = *iter_set;
					iter_set++;
          remaining_vertices.erase (v);
          modified = true;

					for (iter = g->Begin_Neighbor(v); iter != g->End_Neighbor(v); iter++)
					  current_degree[*iter]--;
        }
        else iter_set++;
      }
    }
    while (modified);
  }

  return width;
}


int Nb_Connected_Components (Light_Graph * g)
// returns the number of connected components of the graph g
{
  unsigned int nb_components = 0;   	// number of connected components
  vector<bool> visited = vector<bool> (g->N(),false);  // true is the vertex x has been visited, false otherwise
  unsigned int nb_visited = 0;      	// number of visited vertices
  stack<vertex> s;
  vertex v,x;

  // the computation is achieved by a depth-first search of the graph
  x = 0;
  do
  {
    if (! visited[x])
    {
			// traversal of the connected component of the vertex x
			visited[x] = true;
			nb_visited++;

			s.push(x);

			while (! s.empty())
			{
				v = s.top();
				s.pop();

				for (Light_Graph::Neighbor_iterator iter = g->Begin_Neighbor(v); iter != g->End_Neighbor(v); iter++)
					if (! visited[*iter])
					{
						s.push (*iter);
						visited[*iter] = true;
						nb_visited++;
					}
			}

			nb_components++;
		}
		x++;
  }
  while (nb_visited != g->N());

  return nb_components;
}


bool Is_Acyclic (Light_Graph * g)
// returns true if the graph g is acyclic, false otherwise
{
  if (g->M() <= g->N()-1)
  {
    vector<bool> visited = vector<bool> (g->N(),false);  // true is the vertex x has been visited, false otherwise
	  unsigned int nb_visited = 0;      	// number of visited vertices
		stack<vertex> s;
		vertex v,x;

    for (v = 0; v < g->N(); v++)
      if (g->Degree(v) == 0)
      {
        visited[v] = true;
				nb_visited++;
      }
    
    unsigned int nb_vertices;   // number of vertices of the connected component
		unsigned int nb_edges;      // number of edges of the connected component
		
		x = 0;
		
    while (nb_visited != g->N())
    {
			if (! visited[x])
			{
				// traversal of the connected component of the vertex x
				nb_vertices = 0;
				nb_edges = 0;

				visited[x] = true;
				nb_visited++;

				s.push(x);

				while (! s.empty())
				{
					v = s.top();
					s.pop();
					nb_vertices++;

					for (Light_Graph::Neighbor_iterator iter = g->Begin_Neighbor(v); iter != g->End_Neighbor(v); iter++)
					{
						if (! visited[*iter])
						{
							s.push (*iter);
							visited[*iter] = true;
							nb_visited++;
						}
						nb_edges++;
					}
				}
				
				/* checks whether the connected component is a tree */
				if (nb_edges / 2 != nb_vertices - 1)
					return false;
			}
			x++;
    }

    return true;
  }
  else return false;
}
