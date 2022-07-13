#include "hypergraph_auxiliary_functions.h"
#include "graph_auxiliary_functions.h"


int Nb_Connected_Components (Multi_Hypergraph * h)
// returns the number of connected components of the hypergraph h
{
	return Nb_Connected_Components(h->Get_Primal_Graph ());
}

