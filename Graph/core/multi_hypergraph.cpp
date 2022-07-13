#include "multi_hypergraph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//-----------------------------
// constructors and destructor
//-----------------------------


Multi_Hypergraph::Multi_Hypergraph (): Graphical_Structure ()
// constructs an empty hypergraph with 0 vertex and 0 edge
{
	max_arity = 0;
  recompute_primal_graph = true;
}


Multi_Hypergraph::Multi_Hypergraph (Multi_Hypergraph & h)
// constructs a hypergraph by copying the Multi_Hypergraph h
{
	// initialization of the Multi_Hypergraph with h.n vertices and 0 edge
	n = h.n;
	m = 0;
	max_arity = 0;

	adjacency_list.resize (n);
	degree.resize (n,0);
		
	// copy of the edges of the hypergraph h
  for (map<int,Edge*>::iterator iter = h.Begin_Edge_List (); iter != h.End_Edge_List (); iter++)
	  Add_Edge (*(*iter).second);

  plg = 0;
  pg = 0;
}


Multi_Hypergraph::~Multi_Hypergraph ()
// destructor
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
}


//----------
// operator
//----------

Multi_Hypergraph & Multi_Hypergraph::operator= (Multi_Hypergraph & h)
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

		delete plg;
		plg = 0;
		delete pg;
		pg = 0;
	
		// initialization of the hypergraph with h.n vertices and 0 edge
		n = h.n;
		m = 0;
		max_arity = 0;

		adjacency_list.resize(n);
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


//----------------------------------------------------
// basic operations or functions on Multi_Hypergraphs
//----------------------------------------------------


void Multi_Hypergraph::Init (unsigned int nb_vertices)
// initializes the hypergraph with n vertices and 0 edge
{
	n = nb_vertices;
	m = 0;
	
	adjacency_list.resize(n);
	degree.resize (n,0);
  
	recompute_primal_graph = true;
}


void Multi_Hypergraph::Add_Vertex ()
// adds a vertex to the hypergraph
{
	n++;
	adjacency_list.resize(n);
	degree.resize (n,0);

  recompute_primal_graph = true;
}


bool Multi_Hypergraph::Is_Edge (vertex elements[], int arity)
// returns true if the vertices of elements forms an edge of the hypergraph, false otherwise
{
	bool found = false;
	int i;
	
	vertex x = elements[0];
	for (list<Edge *>::iterator iter = adjacency_list[x].begin(); (iter != adjacency_list[x].end()) && (! found); iter++)
	{
		if (arity == (*iter)->Get_Arity())
		{
			i = 1;
			while ((i < arity) && ((*iter)->Is_Element (elements[i])))
				i++;
				
			found = (i == arity);
		}
	}
	
	return found;
}


bool Multi_Hypergraph::Is_Edge (Edge & e)
// returns true if the edge e is an edge of the hypergraph, false otherwise
{
	bool found = false;
	int arity;
	
	arity = e.Get_Arity();
	vertex x = e.Get_Element(0);
	for (list<Edge *>::iterator iter = adjacency_list[x].begin(); (iter != adjacency_list[x].end()) && (! found); iter++)
		found = (arity == (*iter)->Get_Arity()) && (**iter == e);
		
	return found;
}


bool Multi_Hypergraph::Is_Edge (set<vertex> elements)
// returns true if the edge e is an edge of the hypergraph, false otherwise
{
	bool found = false;
	int i;
	int arity = elements.size();
	
	vertex x = *(elements.begin());
	for (list<Edge *>::iterator iter = adjacency_list[x].begin(); (iter != adjacency_list[x].end()) && (! found); iter++)
	{
		if (arity == (*iter)->Get_Arity())
		{
			i = 0;
			for (set<vertex>::iterator iter2 = elements.begin(); (iter2 != elements.end()) && ((*iter)->Is_Element (*iter2)); iter2++)
					i++;
									
			found = (i == arity);
		}
	}
	
	return found;
}


void Multi_Hypergraph::Add_Edge (vertex elements[], int arity)
// adds the vertices of elements as a new edge of the hypergraph
{
	Edge * e;
	e = new Edge (m,elements,arity);
	
	for (int i = 0; i < arity; i++)
	{
		adjacency_list[elements[i]].push_front (e);
		degree[elements[i]]++;
	}
	
	edges.insert (pair<int,Edge *>(m,e));
	
	m++;
	if (arity > max_arity)
		max_arity = arity;
		
	recompute_primal_graph = true;
}


void Multi_Hypergraph::Add_Edge (Edge & e)
// adds a copy of the edge e as a new edge of the hypergraph
{
	Edge * e2;
	e2 = new Edge (e);
	e2->Set_Num (m);
	
	for (set<vertex>::iterator iter = e2->Begin(); iter != e2->End(); iter++)
	{
		adjacency_list[*iter].push_front (e2);
		degree[*iter]++;
	}
	
	edges.insert (pair<int,Edge *>(m,e2));
	
	m++;
	int arity = e2->Get_Arity();
	if (arity > max_arity)
		max_arity = arity;
		
	recompute_primal_graph = true;
}


void Multi_Hypergraph::Add_Edge_End (vertex elements[], int arity)
// adds the vertices of elements as a new edge of the hypergraph (the edge is added at the end of adjacency list)
{
	Edge * e;
	e = new Edge (m,elements,arity);
	
	for (int i = 0; i < arity; i++)
	{
		adjacency_list[elements[i]].push_back (e);
		degree[elements[i]]++;
	}
	
	edges.insert (pair<int,Edge *>(m,e));
	
	m++;
	if (arity > max_arity)
		max_arity = arity;
		
	recompute_primal_graph = true;
}


void Multi_Hypergraph::Add_Edge_End (Edge & e)
// adds a copy of the edge e as a new edge of the hypergraph (the edge is added at the end of adjacency list)
{
	Edge * e2;
	e2 = new Edge (e);
	e2->Set_Num (m);
	
	for (set<vertex>::iterator iter = e2->Begin(); iter != e2->Begin(); iter++)
	{
		adjacency_list[*iter].push_front (e2);
		degree[*iter]++;
	}
	
	edges.insert (pair<int,Edge *>(m,e2));
	
	m++;
	int arity = e2->Get_Arity();
	if (arity > max_arity)
		max_arity = arity;
		
	recompute_primal_graph = true;
}


void Multi_Hypergraph::Remove_Edge (vertex elements[], int arity)
// removes the edge formed by the vertices of elements from the hypergraph
{
	bool found = true;
	int i,j;
	vertex x;
	list<Edge *>::iterator iter;
	Edge * to_be_deleted = 0;
	
	for (j = 0; (j < arity) && (found); j++)
	{
		found = false;
		x = elements[j];
		iter = adjacency_list[x].begin(); 
		while ((iter != adjacency_list[x].end()) && (! found))
		{
			if (arity == (*iter)->Get_Arity())
			{
				i = 0;
				while ((i < arity) && ((*iter)->Is_Element (elements[i])))
					i++;
				
				if (i == arity)  found = true;
				else iter++;
			}
			else iter++;
		}
	
		if (found)
		{
			to_be_deleted = *iter;
			adjacency_list[x].erase (iter);
			degree[x]--;
		}
	}

	if (found)
	{
		edges.erase (to_be_deleted->Get_Num());
		
		delete to_be_deleted;
		
		m--;
		recompute_primal_graph = true;
	}
}


void Multi_Hypergraph::Remove_Edge (Edge & e)
// removes the edge e from the hypergraph
{
	bool found = true;
	int i;
	vertex x;
	list<Edge *>::iterator iter;
	set<vertex>::iterator iter2, iter_e;
	int arity = e.Get_Arity();
	Edge * to_be_deleted = 0;
	
	for (iter_e = e.Begin(); (iter_e != e.End()) && (found); iter_e++)
	{
		found = false;
		x = *iter_e;
		iter = adjacency_list[x].begin(); 
		while ((iter != adjacency_list[x].end()) && (! found))
		{
			if (arity == (*iter)->Get_Arity())
			{
				iter2 = e.Begin();
				i = 0;
				while ((i < arity) && ((*iter)->Is_Element (*iter2)))
				{
					i++;
					iter2++;
				}
				
				if (i == arity)  found = true;
				else iter++;
			}
			else iter++;
		}
	
		if (found)
		{
			to_be_deleted = *iter;
			adjacency_list[x].erase (iter);
			degree[x]--;
		}
	}

	if (found)
	{
		edges.erase (to_be_deleted->Get_Num());
		
		delete to_be_deleted;
		m--;
		
		recompute_primal_graph = true;
	}
}


void Multi_Hypergraph::Remove_Edge (unsigned int num)
// removes the edge whose number is num
{
  map<int,Edge *>::iterator iter_e = edges.find(num);
  
  if (iter_e != edges.end())
  {
    for (set<vertex>::iterator iter_v = iter_e->second->Begin(); iter_v != iter_e->second->End(); iter_v++)
    {
      vertex x = *iter_v;
      list<Edge *>::iterator iter = adjacency_list[x].begin(); 
      while ((iter != adjacency_list[x].end()) && (num != (unsigned) (*iter)->Get_Num()))
        iter++;

      adjacency_list[x].erase (iter);
      degree[x]--;
    }

    delete iter_e->second;

    edges.erase (iter_e);
    
    m--;
    
    recompute_primal_graph = true;
	}
}


void Multi_Hypergraph::Add_Vertex_In_Edge (vertex v, int num)
// adds a vertex to the edge whose number is num
{
	map<int,Edge *>::iterator iter = edges.find (num);
	
	if (iter != edges.end())
	{
		if (! iter->second->Is_Element(v))
		{
			// we add the vertex
			iter->second->Add_Vertex (v);		
			
			// we update the adjacency list of the vertex v and its degree
			adjacency_list[v].push_front (iter->second);		
			degree[v]++;
      
      recompute_primal_graph = true;
		}
	}
	else throw ("Error: the edge does not exist");
}


void Multi_Hypergraph::Reset_Edge (int num)
// removes all the vertices from the edge e from the hypergraph
{
	map<int,Edge *>::iterator iter = edges.find (num);
	
	if (iter != edges.end())
  {
    vertex x;
    list<Edge *>::iterator iter_list;
    set<vertex>::iterator iter_e;

    for (iter_e = (*iter).second->Begin(); iter_e != (*iter).second->End(); iter_e++)
    {
      x = *iter_e;
      
      iter_list = adjacency_list[x].begin(); 
      while ((iter_list != adjacency_list[x].end()) && (num != (*iter_list)->Get_Num()))
        iter_list++;
    
      if (iter_list != adjacency_list[x].end())
      {
        adjacency_list[x].erase (iter_list);
        degree[x]--;
      }
    }
    
    (*iter).second->Reset();

    recompute_primal_graph = true;
  }
  else throw ("Error: the edge does not exist");
}


//-------------------------
// miscellaneous functions
//-------------------------


Light_Graph * Multi_Hypergraph::Get_Primal_Light_Graph ()
// returns the primal graph related to the graphical structures as a pointer on a Light_Graph object
{
  if ((recompute_primal_graph) || (plg == 0))
  {
    delete plg;
    delete pg;
    
    pg = 0;
    
    // initialization
    plg = new Light_Graph();
    
    plg->Init (N());
    
    // addition of each hyperedge as a clique of the graph
    list<Edge *>::iterator iter;
    set<vertex>::iterator iter2,iter3;
    vertex x;
    
    for (x = 0; x < n; x++)
    {
      for (iter = adjacency_list[x].begin(); iter != adjacency_list[x].end(); iter++)
        if (x == (*iter)->Get_Element(0))
        {
          for (iter2 = (*iter)->Begin(); iter2 != (*iter)->End(); iter2++)
            for (iter3=iter2, iter3++; iter3 != (*iter)->End(); iter3++)
              plg->Add_Edge(*iter2,*iter3);
        }
    }
    
    recompute_primal_graph = false;
  }
	
	return plg;
}


Graph * Multi_Hypergraph::Get_Primal_Graph ()
// returns the primal graph related to the graphical structures as a pointer on a Graph object
{
	if ((recompute_primal_graph) || (pg == 0))
	{
    delete pg;
    delete plg;
    
    plg = 0;
    
		// initialization
		pg = new Graph();
		
		pg->Init (N());
		
		// addition of each hyperedge as a clique of the graph
		list<Edge *>::iterator iter;
		set<vertex>::iterator iter2,iter3;
		vertex x;
		
		for (x = 0; x < n; x++)
		{
			for (iter = adjacency_list[x].begin(); iter != adjacency_list[x].end(); iter++)
				if (x == (*iter)->Get_Element(0))
				{
					for (iter2 = (*iter)->Begin(); iter2 != (*iter)->End(); iter2++)
						for (iter3=iter2, iter3++; iter3 != (*iter)->End(); iter3++)
							pg->Add_Edge(*iter2,*iter3);
				}
		}
    
    recompute_primal_graph = false;
	}
	
	return pg;
}
