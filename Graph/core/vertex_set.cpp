#include "vertex_set.h"
#include "multi_hypergraph.h"
#include <stack>


//-----------------------------
// constructors and destructor
//-----------------------------

Vertex_Set::Vertex_Set (Graphical_Structure * gs)
// constructs an empty vertex set related to gs
{
	g = gs;
	size = 0;
	nb_edges = 0;
	nb_partial_edges = 0;
	nb_components = 0;
}


Vertex_Set::Vertex_Set (vector<vertex> element_set, Graphical_Structure * gs)
// constructs a vertex set containing the vertices of element_set and related to gs
{
	g = gs;
	elements = element_set;
	size = elements.size();

	vertex max = -1;
	for (vector<vertex>::iterator iter = element_set.begin(); iter != element_set.end(); iter++)
		if (*iter > max)
			max = *iter;
			
	is_element = vector<bool> (max+1,false);
	
	vector<vertex>::iterator iter;
	for (iter = elements.begin(); iter != elements.end(); iter++)
	  is_element[*iter] = true;
	  
	nb_edges = 0;
	nb_partial_edges = 0;
	nb_components = 0;	
}


Vertex_Set::Vertex_Set (Vertex_Set & s)
// constructs a vertex set by copying the vertex set s
{
	g = s.g;
	size = s.size;
	elements = s.elements;
	is_element = s.is_element;
	nb_edges = s.nb_edges;
	nb_partial_edges = s.nb_partial_edges;
	nb_components = s.nb_components;
}


Vertex_Set::Vertex_Set (const Vertex_Set & s)
// constructs a vertex set by copying the vertex set s
{
	g = s.g;
	size = s.size;
	elements = s.elements;
	is_element = s.is_element;
	nb_edges = s.nb_edges;
	nb_partial_edges = s.nb_partial_edges;
	nb_components = s.nb_components;
}

Vertex_Set::~Vertex_Set ()
// destructor
{
}


//----------
// operator
//----------

Vertex_Set & Vertex_Set::operator= (const Vertex_Set & s)
// operator =
{
	if (this != &s)
	{
		g = s.g;
		size = s.size;
		elements = s.elements;
		is_element = s.is_element;
		nb_edges = s.nb_edges;
		nb_partial_edges = s.nb_partial_edges;
		nb_components = s.nb_components;
	}
	return *this;
}


//------------------------------
// basic function on vertex set
//------------------------------


void Vertex_Set::Add_Vertex (vertex v)
// adds the vertex v to the current vertex set
{
	if (v >= is_element.size())
	{
		elements.push_back (v);
		is_element.resize (v+1,false);
		is_element[v] = true;
		size++;
	}
	else
		if (! is_element[v])
		{
			elements.push_back (v);
			is_element[v] = true;
			size++;
		}
}


void Vertex_Set::Remove_Vertex (vertex v)
// removes the vertex v from the current vertex set
{
	if (Is_Element(v))
	{
    vector<vertex>::iterator iter;
    
    iter = elements.begin();
    while (*iter != v)
      iter++;
      
	  elements.erase (iter);
		is_element[v] = false;
		size--;
	}
}

int Vertex_Set::Get_Nb_Edges ()
// returns the number of edges included in the vertex set
{
	if ((nb_partial_edges == 0) && (nb_edges == 0))
	{
		if (dynamic_cast<Light_Graph*>(g) != 0)
		{
			Light_Graph * lg = dynamic_cast<Light_Graph*>(g);
			Light_Graph::Neighbor_iterator iter2;
			
			for (vector<vertex>::iterator iter = elements.begin(); iter != elements.end(); iter++)
				for (iter2 = lg->Begin_Neighbor(*iter); iter2 != lg->End_Neighbor(*iter); iter2++)
					if (Is_Element(*iter2))  nb_edges++;
					else nb_partial_edges++;
			
			nb_edges /= 2;
		}
		else
			{
				Multi_Hypergraph * mh = dynamic_cast<Multi_Hypergraph*>(g);
				list<Edge *>::iterator iter2;
				set<vertex>::iterator iter3;
				int nb_elements;
				
				for (vector<vertex>::iterator iter = elements.begin(); iter != elements.end(); iter++)
					for (iter2 = mh->Begin_Edge_List(*iter); iter2 != mh->End_Edge_List(*iter); iter2++)			
					{
						// we look for the first vertex in the edge which also belongs to elements
						iter3 = (*iter2)->Begin();
						while ((iter3 != (*iter2)->End()) && (! Is_Element(*iter3)))
							iter3++;
						
						if (*iter3 == *iter)
						{
							nb_elements = 0;
							while ((iter3 != (*iter2)->End()) && (Is_Element(*iter3)))
							{
								nb_elements++;
								iter3++;
							}

							if (nb_elements == (*iter2)->Get_Arity())
								nb_edges++;
							else nb_partial_edges++;
						}
					}
			}
	}
	
	return nb_edges;
}


int Vertex_Set::Get_Nb_Partial_Edges ()
// returns the number of edges which intersect the vertex set
{
	if ((nb_partial_edges == 0) && (nb_edges == 0))
	{
		if (dynamic_cast<Light_Graph*>(g) != 0)
		{
			Light_Graph * lg = dynamic_cast<Light_Graph*>(g);
			Light_Graph::Neighbor_iterator iter2;
			
			for (vector<vertex>::iterator iter = elements.begin(); iter != elements.end(); iter++)
				for (iter2 = lg->Begin_Neighbor(*iter); iter2 != lg->End_Neighbor(*iter); iter2++)
					if (Is_Element(*iter2))  nb_edges++;
					else nb_partial_edges++;
			
			nb_edges /= 2;
		}
		else
			{
				Multi_Hypergraph * mh = dynamic_cast<Multi_Hypergraph*>(g);
				list<Edge *>::iterator iter2;
				set<vertex>::iterator iter3;
				int nb_elements;
				
				for (vector<vertex>::iterator iter = elements.begin(); iter != elements.end(); iter++)
					for (iter2 = mh->Begin_Edge_List(*iter); iter2 != mh->End_Edge_List(*iter); iter2++)			
					{
						// we look for the first vertex in the edge which also belongs to elements
						iter3 = (*iter2)->Begin();
						while ((iter3 != (*iter2)->End()) && (! Is_Element(*iter3)))
							iter3++;
						
						if (*iter3 == *iter)
						{
							nb_elements = 0;
							while ((iter3 != (*iter2)->End()) && (Is_Element(*iter3)))
							{
								nb_elements++;
								iter3++;
							}

							if (nb_elements == (*iter2)->Get_Arity())
								nb_edges++;
							else nb_partial_edges++;
						}
					}
			}		
	}
		
	return nb_partial_edges;
}


int Vertex_Set::Get_Nb_Connected_Components ()
// returns the number of connected components in the vertex set
{
	if (nb_components == 0)
	{
		Light_Graph * pg;
		
		if (dynamic_cast<Graph *>(g) != 0)
			pg = dynamic_cast<Graph *>(g);
		else
			if (dynamic_cast<Light_Graph *>(g) != 0)
				pg = dynamic_cast<Light_Graph *>(g);
			else pg = g->Get_Primal_Graph();

		vector<bool> visited = vector<bool> (g->N(),false);  // true is the vertex x has been visited, false otherwise
		int nb_visited = 0;      	// number of visited vertices
		stack<vertex> s;
		vertex v;
		
		// the computation is achieved by a depth-first search of the graph
		vector<vertex>::iterator iter = elements.begin();
		do
		{
			if (! visited[*iter])
			{
				// traversal of the connected component of the vertex x
				visited[*iter] = true;
				nb_visited++;

				s.push(*iter);

				while (! s.empty())
				{
					v = s.top();
					s.pop();

					for (Light_Graph::Neighbor_iterator iter2 = pg->Begin_Neighbor (v); iter2 != pg->End_Neighbor (v); iter2++)
						if ((Is_Element(*iter2)) && (! visited[*iter2]))
						{
							s.push (*iter2);
							visited[*iter2] = true;
							nb_visited++;
						}
				}

				nb_components++;
			}
			iter++;
		}
		while (iter != elements.end());		
	}
	
	return nb_components;
}


bool Vertex_Set::Is_Connected()
// returns true if the cluster has a single connected component, false otherwise
{
	Light_Graph * pg;
	
	if (dynamic_cast<Graph *>(g) != 0)
		pg = dynamic_cast<Graph *>(g);
	else
		if (dynamic_cast<Light_Graph *>(g) != 0)
			pg = dynamic_cast<Light_Graph *>(g);
		else pg = g->Get_Primal_Graph();
	
	vector<bool> visited = vector<bool> (g->N(),false);  // true is the vertex x has been visited, false otherwise
	int nb_visited = 0;      	// number of visited vertices
	stack<vertex> s;
	vertex v;
	
	// the computation is achieved by a depth-first search of the graph
	
	// traversal of the connected component of the vertex element[0]
	v = elements[0];
	visited[v] = true;
	nb_visited++;

	s.push(v);

	while (! s.empty())
	{
		v = s.top();
		s.pop();

		for (Light_Graph::Neighbor_iterator iter2 = pg->Begin_Neighbor (v); iter2 != pg->End_Neighbor (v); iter2++)
			if ((Is_Element(*iter2)) && (! visited[*iter2]))
			{
				s.push (*iter2);
				visited[*iter2] = true;
				nb_visited++;
			}
	}
	
	return nb_visited == size;
}


void Vertex_Set::Merge (Vertex_Set * s)
// adds the vertex set s to the current vertex set.
{
	if ((this != s) && (s->g == g))
	{
		vector<vertex>::iterator iter;
		
		for (iter = s->elements.begin(); iter != s->elements.end(); iter++)
			Add_Vertex (*iter);

		nb_edges = 0;
		nb_partial_edges = 0;
		nb_components = 0;
	}
}
