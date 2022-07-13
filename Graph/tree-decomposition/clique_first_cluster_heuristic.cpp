#include "clique_first_cluster_heuristic.h"
#include "graph.h"

//----------------------------
// constructor and destructor
//----------------------------

Clique_First_Cluster_Heuristic::Clique_First_Cluster_Heuristic (Graphical_Structure * gs)
// constructs a heuristic which chooses a clique (of the primal graph of the graphical structure gs) containing the vertex having the largest degree
{
	g = gs;
	with_initial_v = false;
	initial_v = 0;
}


Clique_First_Cluster_Heuristic::Clique_First_Cluster_Heuristic (Graphical_Structure * gs, vertex init_v)
// constructs a heuristic which chooses a clique (of the primal graph of the graphical structure gs) containing the vertex init_v
{
	g = gs;
	with_initial_v = true;
	initial_v = init_v;
}



//-----------------
// basic functions
//-----------------

pair<int,vertex *> Clique_First_Cluster_Heuristic::Choose ()
// returns the first cluster for a tree-decomposition according to a heuristic method. This cluster must be connected
{
	Light_Graph * pg;
	
	if (dynamic_cast<Graph *>(g) != 0)
		pg = dynamic_cast<Graph *>(g);
	else
		if (dynamic_cast<Light_Graph *>(g) != 0)
			pg = dynamic_cast<Light_Graph *>(g);
		else pg = g->Get_Primal_Graph();
		
	unsigned int n = pg->N();
	vertex v;
	int deg, max_degree;
	
	if (! with_initial_v)
	{
		// as there is no initial vertex, we choose as initial vertex the vertex which has the largest degree in the graphical structure g			
		max_degree = -1;
		for (v = 0; v < n; v++)
		{
			deg = g->Degree (v);
			if (deg > max_degree)
			{
				max_degree = deg;
				initial_v = v;
			}
		}
	}
		
	// we look for the clique

	pair<int,vertex *> possible_clique;		// the set of possible vertex of the clique
	
	// we add the initial vertex to the clique
	int card = 1;
	possible_clique.first = 1;
	possible_clique.second = new vertex [pg->Degree(initial_v)+1];
	possible_clique.second[0] = initial_v;
	
	// we add to the possible clique each vertex in the neighborhood of the initial vertex
	for (Light_Graph::Neighbor_iterator iter_v = pg->Begin_Neighbor (initial_v); iter_v != pg->End_Neighbor (initial_v); iter_v++)
	{
		possible_clique.second[possible_clique.first] = *iter_v;
		possible_clique.first++;
	}
	
	int i,j;
	int max_index;		// the index of the next vertex of the clique in the array in possible_clique
	bool again;
		
	do
	{
		max_degree = -1;
		max_index = -1;
		again = false;
		for (i = card; i < possible_clique.first; )
		{
			j = 0; 
			while ((j < card) && (pg->Is_Edge (possible_clique.second[i],possible_clique.second[j])))
				j++;
			
			if (j == card)		// we have a possible new vertex for the clique
			{
				deg = pg->Degree(possible_clique.second[i]);
				if (deg > max_degree)
				{
					max_degree = deg;
					max_index = i;
				}
				i++;
			}
			else
				{
					// we remove the vertex possible_clique.second[i] since it cannot belong to the current clique in possible_clique
					possible_clique.first--;
					possible_clique.second[i] = possible_clique.second[possible_clique.first];
				}
		}
		
		if (max_index != -1)
		{
			v = possible_clique.second[card];
			possible_clique.second[card] = possible_clique.second[max_index];
			possible_clique.second[max_index] = v;
			card++;
			again = true;
		}
	}
	while (again);
	
	possible_clique.first = card;
	
	return possible_clique;
}


Cluster_Node * Clique_First_Cluster_Heuristic::Choose (bool is_available[])
// returns the first cluster for a tree-decomposition according to a heuristic method. This cluster must be connected and its vertices must be chosen among the vertices for which the flag is_available is set to true. For each selected vertex, is_available is set to false
{
	Light_Graph * pg;
	
	if (dynamic_cast<Graph *>(g) != 0)
		pg = dynamic_cast<Graph *>(g);
	else
		if (dynamic_cast<Light_Graph *>(g) != 0)
			pg = dynamic_cast<Light_Graph *>(g);
		else pg = g->Get_Primal_Graph();
	
	unsigned int n = pg->N();
	vertex v;
	int deg, max_degree;
	
	vertex starting_v = 0;
	
	if (with_initial_v)
	{
		starting_v = initial_v;
		if (! is_available[starting_v])
			throw ("Error: the initial vertex is not available");
	}
	else
	{
		// as there is no initial vertex, we choose as initial vertex the vertex which has the largest degree in the graphical structure g			
		max_degree = -1;
		for (v = 0; v < n; v++)
			if (is_available[v])
			{
				deg = g->Degree (v);
				if (deg > max_degree)
				{
					max_degree = deg;
					starting_v = v;
				}
			}
	}
		
	// we look for the clique

	pair<int,vertex *> possible_clique;		// the set of possible vertex of the clique
	Cluster_Node * cluster;								// the cluster we are computing
	
	// we add the initial vertex to the clique
	int card = 0;
	possible_clique.first = 0;
	possible_clique.second = new vertex [pg->Degree(starting_v)];
	cluster = new Cluster_Node (g);
	cluster->Add_Vertex (starting_v);
	is_available[starting_v] = false;
	
	// we add to the possible clique each vertex in the neighborhood of the initial vertex
	for (Light_Graph::Neighbor_iterator iter_v = pg->Begin_Neighbor (starting_v); iter_v != pg->End_Neighbor (starting_v); iter_v++)
		if (is_available[*iter_v])
		{
			possible_clique.second[possible_clique.first] = *iter_v;
			possible_clique.first++;
		}
	
	int i,j;
	int max_index;		// the index of the next vertex of the clique in the array in possible_clique
	bool again;
		
	do
	{
		max_degree = -1;
		max_index = -1;
		again = false;
		for (i = card; i < possible_clique.first; )
		{
			j = 0; 
			while ((j < card) && (pg->Is_Edge (possible_clique.second[i],possible_clique.second[j])))
				j++;
			
			if (j == card)		// we have a possible new vertex for the clique
			{
				deg = pg->Degree(possible_clique.second[i]);
				if (deg > max_degree)
				{
					max_degree = deg;
					max_index = i;
				}
				i++;
			}
			else
				{
					// we remove the vertex possible_clique.second[i] since it cannot belong to the current clique in possible_clique
					possible_clique.first--;
					possible_clique.second[i] = possible_clique.second[possible_clique.first];
				}
		}
		
		if (max_index != -1)
		{
			v = possible_clique.second[card];
			possible_clique.second[card] = possible_clique.second[max_index];
			possible_clique.second[max_index] = v;
			card++;
			
			cluster->Add_Vertex (v);
			is_available[v] = false;
			
			again = true;
		}
	}
	while (again);
	
	delete [] possible_clique.second;
	
	return cluster;
}
