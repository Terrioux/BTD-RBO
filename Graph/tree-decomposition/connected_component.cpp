#include "connected_component.h"
#include "multi_hypergraph.h"
#include <typeinfo>


//----------------------------
// constructor and destructor
//----------------------------

Connected_Component::Connected_Component (Graphical_Structure * gs)
// constructs a related to the graphical structure gs
{
	if (typeid(*gs) == typeid (Light_Graph))  g = dynamic_cast<Light_Graph*>(gs);
	else 
		if (typeid(*gs) == typeid (Graph))  g = dynamic_cast<Graph*>(gs);
		else g = gs->Get_Primal_Graph();;

	
}


Connected_Component::~Connected_Component ()
// destructor
{
}


//-----------------				
// basic functions
//-----------------


bool Connected_Component::Is_Connected (pair<int,vertex *> & vs)
// returns true if the graphical substructure induced by the vertex set vs is connected, false otherwise. vs is defined as a pair (number of vertices,array of vertices)
{
	if (vs.first <= 1)
		return true;
	
	unsigned int n = g->N();
  bool * visited;  	// true if the vertex x has been visited, false otherwise
  visited = new bool [n];
  int nb_visited = 0; // the number of visited vertices
  vertex x;
  
  // initializations
  for (x =0; x < n; x++)
		visited[x] = true;
  
  for (int i = 0; i < vs.first; i++)
		visited[vs.second[i]] = false;
		
  stack<vertex> s;
  vertex v;

  // the computation is achieved by a depth-first search of the graph from the first vertex of vs
  x = vs.second[0];
	visited[x] = true;
	nb_visited++;

	s.push(x);

	while (! s.empty())
	{
		v = s.top();
		s.pop();

		for (Light_Graph::Neighbor_iterator iter=g->Begin_Neighbor (v); iter != g->End_Neighbor (v); iter++)
			if (! visited[*iter])
			{
				s.push (*iter);
				visited[*iter] = true;
				nb_visited++;
			}
	}
  
  delete [] visited;
  return nb_visited == vs.first;
}


list<pair<int,vertex *> > Connected_Component::Get_Connected_Components ()
// returns the list of connected components of the related graphical structure
{
	unsigned int n = g->N();
  bool * visited;  	// true if the vertex x has been visited, false otherwise
  visited = new bool [n];
  int nb_remaining_vertices = n; 	// the number of remaining vertices
  vertex x;
  list<pair<int,vertex *> > cc_list;		// the list of built connected components
  
  // initializations
  for (x =0; x < n; x++)
		visited[x] = false;
  
  stack<vertex> s;
  vertex v;

  // the computation is achieved by a depth-first search of the graph
  x = 0;
  vertex * t;	// the vertices of the connected component of x
  t = new vertex [n];

  while (nb_remaining_vertices > 0)
  {
    if (! visited[x])
    {
			// traversal of the connected component of the vertex x
			int card = 1;		// the cardinal of the vertex set corresponding to the connected component of x
			
			visited[x] = true;
			t[0] = x;
			nb_remaining_vertices--;

			s.push(x);

			while (! s.empty())
			{
				v = s.top();
				s.pop();

				for (Light_Graph::Neighbor_iterator iter=g->Begin_Neighbor (v); iter != g->End_Neighbor (v); iter++)
					if (! visited[*iter])
					{
						s.push (*iter);
						visited[*iter] = true;
						t[card] = *iter;
						card++;
						nb_remaining_vertices--;
					}
			}
			
			vertex * t2;
			t2 = new vertex [card];
			for (int i = 0; i < card; i++)
				t2[i] = t[i];
			cc_list.push_back (make_pair(card,t2));
		}
		x++;
  }

	delete [] t;
	delete [] visited;
  return cc_list;	
}


list<pair<int,vertex *> > Connected_Component::Get_Connected_Components (pair<int,vertex *> & vs, pair<int,vertex *> & except_vs)
// returns the list of connected components of the related graphical substructure induced by the vertex set vs - except_vs
{
	unsigned int n = g->N();
  bool * visited;  	// true if the vertex x has been visited, false otherwise
  visited = new bool [n];
  int nb_remaining_vertices = vs.first; 	// the number of remaining vertices
  vertex x;
  list<pair<int,vertex *> > cc_list;		// the list of built connected components

  // initializations
  for (x =0; x < n; x++)
		visited[x] = true;
  
  for (int i = 0; i < vs.first; i++)
		visited[vs.second[i]] = false;

  for (int i = 0; i < except_vs.first; i++)
		if (! visited[except_vs.second[i]])
		{
			visited[except_vs.second[i]] = true;
			nb_remaining_vertices--;
		}
		
  stack<vertex> s;
  vertex v;

  // the computation is achieved by a depth-first search of the graph
  x = 0;
  vertex * t;	// the vertices of the connected component of x
  t = new vertex [n];

  int nb = 0;
  while (nb_remaining_vertices > 0)
  {
    if (! visited[x])
    {
			nb++;
			// traversal of the connected component of the vertex x
			int card = 1;		// the cardinal of the vertex set corresponding to the connected component of x
			
			visited[x] = true;
			t[0] = x;
			nb_remaining_vertices--;

			s.push(x);

			while (! s.empty())
			{
				v = s.top();
				s.pop();

				for (Light_Graph::Neighbor_iterator iter=g->Begin_Neighbor (v); iter != g->End_Neighbor (v); iter++)
					if (! visited[*iter])
					{
						s.push (*iter);
						visited[*iter] = true;
						t[card] = *iter;
						card++;
						nb_remaining_vertices--;
					}
			}
			
			vertex * t2;
			t2 = new vertex [card];
			for (int i = 0; i < card; i++)
				t2[i] = t[i];
			cc_list.push_back (make_pair(card,t2));
		}
		x++;
  }

	delete [] t;
	delete [] visited;
  return cc_list;
}


void Connected_Component::Get_Connected_Components_With_Neighborhood (pair<int,vertex *> & vs, Cluster_Node * except_vs, stack<tuple<pair<int,vertex *>,Cluster_Node *,pair<int,vertex *>>> & S)
// puts in the stack S the list of both connected components of the related graphical substructure induced by the vertex set vs - except_vs, and their related neighborhood in except_vs
{
	unsigned int n = g->N();
  int*  visited;  	// 0 if the vertex has not been visited yet, 1 if if the vertex is in except_vs and has not been reached yet from the current vertex x, 2 otherwise
  visited = new int [n];
  int nb_remaining_vertices = vs.first; 	// the number of remaining vertices
  vertex x;

  // initializations
  for (x =0; x < n; x++)
		visited[x] = 2;
  
  for (int i = 0; i < vs.first; i++)
		visited[vs.second[i]] = 0;

  for (vector<vertex>::iterator iter_v = except_vs->begin(); iter_v != except_vs->end(); iter_v++)
  {
		if (visited[*iter_v] == 0)
			nb_remaining_vertices--;
		visited[*iter_v] = 1;
	}
		
  stack<vertex> s;
  vertex v;

  // the computation is achieved by a depth-first search of the graph
  x = 0;
  vertex * t;	// the vertices of the connected component of x
  t = new vertex [nb_remaining_vertices];
  
  vertex * V;	// the neighborhood 
  V = new vertex [except_vs->Size()];

  while (nb_remaining_vertices > 0)
  {
    if (visited[x] == 0)
    {
			// traversal of the connected component of the vertex x
			int card = 1;			// the cardinal of the vertex set corresponding to the connected component of x
			int card_V = 0;		// the cardinal of the neighborhood
			
			visited[x] = 2;
			t[0] = x;
			nb_remaining_vertices--;

			s.push(x);

			while (! s.empty())
			{
				v = s.top();
				s.pop();

				for (Light_Graph::Neighbor_iterator iter=g->Begin_Neighbor (v); iter != g->End_Neighbor (v); iter++)
					if (visited[*iter] == 0)
					{
						s.push (*iter);
						visited[*iter] = 2;
						t[card] = *iter;
						card++;
						nb_remaining_vertices--;
					}
					else
						if (visited[*iter] == 1)
						{
							// we found a vertex in the neighborhood
							V[card_V] = *iter;
							card_V++;
							visited[*iter] = 2;
						}
			}
			
			vertex * t2;
			t2 = new vertex [card];
			for (int i = 0; i < card; i++)
				t2[i] = t[i];

			vertex * t3;
			t3 = new vertex [card_V];
			for (int i = 0; i < card_V; i++)
				t3[i] = V[i];
			
			S.push (make_tuple(make_pair(card,t2), except_vs, make_pair(card_V,t3)));

			if (nb_remaining_vertices > 0)
			{
				for (vector<vertex>::iterator iter_v = except_vs->begin(); iter_v != except_vs->end(); iter_v++)
					visited[*iter_v] = 1;
			}
		}
		x++;
  }

	delete [] visited;
	delete [] t;
	delete [] V;
}


void Connected_Component::Get_Connected_Components_From_Set (Cluster_Node * initial_vs, stack<tuple<pair<int,vertex *>,Cluster_Node *, pair<int,vertex *>>> & S)	
// puts in the stack S the list of both connected components of the related graphical substructure induced from the vertex set initial_vs, and their related neighborhood in initial_vs
{
	unsigned int n = g->N();
  int * visited;  	// 0 if the vertex has not been visited yet, 1 if if the vertex is in initial_vs and has not been reached yet from the current vertex x, 2 otherwise
  visited = new int [n];
  vertex x;

  // initializations
  for (x =0; x < n; x++)
		visited[x] = 0;

  for (vector<vertex>::iterator iter_v = initial_vs->begin(); iter_v != initial_vs->end(); iter_v++)
		visited[*iter_v] = 1;
 
  stack<vertex> s;
  vertex v;

  // the computation is achieved by a depth-first search of the graph
  x = 0;
  vertex * t;	// the vertices of the connected component of x
  t = new vertex [n-initial_vs->Size()];
  
  vertex * V;	// the neighborhood 
  V = new vertex [initial_vs->Size()];

	for (x = 0; x < n; x++)
    if (visited[x] == 0)
    {
			// we check whether x is a neighbor of a vertex in initial_vs
			vector<vertex>::iterator iter_v = initial_vs->begin(); 

			while ((iter_v != initial_vs->end()) && (! g->Is_Edge(x,*iter_v)))
				iter_v++;
			
			if (iter_v != initial_vs->end())
			{			
				// traversal of the connected component of the vertex x
				int card = 1;		// the cardinal of the vertex set corresponding to the connected component of x
				int card_V = 0;	// the cardinal of the neighborhood
					
				visited[x] = 2;
				t[0] = x;

				s.push(x);

				while (! s.empty())
				{
					v = s.top();
					s.pop();

					for (Light_Graph::Neighbor_iterator iter=g->Begin_Neighbor (v); iter != g->End_Neighbor (v); iter++)
						if (visited[*iter] == 0)
						{
							// we add the new vertex in t 
							s.push (*iter);
							visited[*iter] = 2;
							t[card] = *iter;
							card++;
						}
						else
							if (visited[*iter] == 1)
							{
								// we found a vertex in the neighborhood
								V[card_V] = *iter;
								card_V++;
								visited[*iter] = 2;
							}
				}
				
				vertex * t2;
				t2 = new vertex [card];
				for (int i = 0; i < card; i++)
					t2[i] = t[i];
					
				vertex * t3;
				t3 = new vertex [card_V];
				for (int i = 0; i < card_V; i++)
					t3[i] = V[i];

				S.push (make_tuple(make_pair(card,t2), initial_vs, make_pair(card_V,t3)));

				for (vector<vertex>::iterator iter_v = initial_vs->begin(); iter_v != initial_vs->end(); iter_v++)
					visited[*iter_v] = 1;
			}
		}
	
	delete [] visited;
	delete [] t;
	delete [] V;
}


void Connected_Component::Get_Connected_Components_From_Set (Cluster_Node * initial_vs, stack<tuple<pair<int,vertex *>,Cluster_Node *, pair<int,vertex *>>> & S, bool is_untreated [])	
// puts in the stack S the list of both connected components of the related graphical substructure induced from the vertex set initial_vs, and their related neighborhood in initial_vs
{
	unsigned int n = g->N();
  int * visited;  	// 0 if the vertex has not been visited yet, 1 if if the vertex is in initial_vs and has not been reached yet from the current vertex x, 2 otherwise
  visited = new int [n];
  vertex x;

  // initializations
  for (x =0; x < n; x++)
		if (is_untreated[x])
			visited[x] = 0;
		else visited[x] = 2;

  for (vector<vertex>::iterator iter_v = initial_vs->begin(); iter_v != initial_vs->end(); iter_v++)
		visited[*iter_v] = 1;
 
  stack<vertex> s;
  vertex v;

  // the computation is achieved by a depth-first search of the graph
  x = 0;
  vertex * t;	// the vertices of the connected component of x
  t = new vertex [n-initial_vs->Size()];
  
  vertex * V;	// the neighborhood 
  V = new vertex [initial_vs->Size()];

	for (x = 0; x < n; x++)
    if (visited[x] == 0)
    {
			// we check whether x is a neighbor of a vertex in initial_vs
			vector<vertex>::iterator iter_v = initial_vs->begin(); 

			while ((iter_v != initial_vs->end()) && (! g->Is_Edge(x,*iter_v)))
				iter_v++;
			
			if (iter_v != initial_vs->end())
			{			
				// traversal of the connected component of the vertex x
				int card = 1;		// the cardinal of the vertex set corresponding to the connected component of x
				int card_V = 0;	// the cardinal of the neighborhood
					
				visited[x] = 2;
				t[0] = x;

				s.push(x);

				while (! s.empty())
				{
					v = s.top();
					s.pop();

					for (Light_Graph::Neighbor_iterator iter=g->Begin_Neighbor (v); iter != g->End_Neighbor (v); iter++)
						if (visited[*iter] == 0)
						{
							// we add the new vertex in t 
							s.push (*iter);
							visited[*iter] = 2;
							t[card] = *iter;
							card++;
						}
						else
							if (visited[*iter] == 1)
							{
								// we found a vertex in the neighborhood
								V[card_V] = *iter;
								card_V++;
								visited[*iter] = 2;
							}
				}
				
				vertex * t2;
				t2 = new vertex [card];
				for (int i = 0; i < card; i++)
					t2[i] = t[i];
					
				vertex * t3;
				t3 = new vertex [card_V];
				for (int i = 0; i < card_V; i++)
					t3[i] = V[i];

				S.push (make_tuple(make_pair(card,t2), initial_vs, make_pair(card_V,t3)));

				for (vector<vertex>::iterator iter_v = initial_vs->begin(); iter_v != initial_vs->end(); iter_v++)
					visited[*iter_v] = 1;
			}
		}
	
	delete [] visited;
	delete [] t;
	delete [] V;
}


bool Connected_Component::Induce_Bounded_Neighborhood (pair<int,vertex *> & vs, Cluster_Node * except_vs, int k)
// returns true if all the connected components of the related graphical substructure induced by the vertex set vs - except_vs has a neighborhood in except_vs whose size does not exceed k; false otherwise
{
	unsigned int n = g->N();
  int * visited;  	// 0 if the vertex has not been visited yet, 1 if if the vertex is in except_vs and has not been reached yet from the current vertex x, 2 otherwise
  visited = new int [n];
  int nb_remaining_vertices = vs.first; 	// the number of remaining vertices
  vertex x;

  // initializations
  for (x =0; x < n; x++)
		visited[x] = 2;
  
  for (int i = 0; i < vs.first; i++)
		visited[vs.second[i]] = 0;

  for (vector<vertex>::iterator iter_v = except_vs->begin(); iter_v != except_vs->end(); iter_v++)
  {
		if (visited[*iter_v] == 0)
			nb_remaining_vertices--;
		visited[*iter_v] = 1;
	}
		
  stack<vertex> s;
  vertex v;

  // the computation is achieved by a depth-first search of the graph
  x = 0;
  
  while (nb_remaining_vertices > 0)
  {
    if (visited[x] == 0)
    {
			// traversal of the connected component of the vertex x
			int card_V = 0;		// the cardinal of the neighborhood
			
			visited[x] = 2;
			nb_remaining_vertices--;

			s.push(x);

			while (! s.empty())
			{
				v = s.top();
				s.pop();

				for (Light_Graph::Neighbor_iterator iter=g->Begin_Neighbor (v); iter != g->End_Neighbor (v); iter++)
					if (visited[*iter] == 0)
					{
						s.push (*iter);
						visited[*iter] = 2;
						nb_remaining_vertices--;
					}
					else
						if (visited[*iter] == 1)
						{
							// we found a vertex in the neighborhood
							card_V++;
							visited[*iter] = 2;
						}
			}
			
			if (card_V > k)
			{
				delete [] visited;
				return false;
			}

			if (nb_remaining_vertices > 0)
			{
				for (vector<vertex>::iterator iter_v = except_vs->begin(); iter_v != except_vs->end(); iter_v++)
					visited[*iter_v] = 1;
			}
		}
		x++;
  }
  delete [] visited;
  return true;
}
