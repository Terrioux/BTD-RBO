#include "sep_k_neighbors_next_vertices_heuristic.h"
#include <iostream>
#include "connected_component.h"

//-------------
// constructor
//-------------


Sep_K_Neighbors_Next_Vertices_Heuristic::Sep_K_Neighbors_Next_Vertices_Heuristic(Light_Graph * ref_g, int sep_k)
// constructs the heuristic  
{
  g = ref_g;
	k = sep_k;
}


//----------------
// basic function
//----------------

void Sep_K_Neighbors_Next_Vertices_Heuristic::Choose (pair<int,vertex *> & vs, Cluster_Node * cl, pair<int,vertex *> & sep, int current_w, bool is_available[])
// returns as next vertices all neighbors of separator until a certain distance d where all the connected components induced have separator <= k (CC are isolated as soon as possible)
{
	pair<int, vertex *> chosen_v;
	chosen_v.first = 0;
	chosen_v.second = new vertex [g->N()];
	pair<int, int> * belong_to_chosen_v = new pair<int,int> [g->N()];
	
	pair<int, vertex *> sep_k_1; // level k+1
	sep_k_1.first = 0;
	sep_k_1.second = new vertex [g->N()];
	
	pair<int, vertex *> sep_k; // level k
	sep_k.first = sep.first;
	sep_k.second = new vertex [g->N()];
	for(int i=0; i< sep.first; i++)
		sep_k.second[i] = sep.second[i];
	
	for (unsigned int i=0; i<g->N(); i++)
		belong_to_chosen_v[i] = make_pair(-1,-1); // not taken into account
	
	for (int i=0; i<vs.first; i++)
		belong_to_chosen_v[vs.second[i]] = make_pair(0,i); // taken into account but not chosen yet
	
	bool good_sep_k = false;
	
	while (!good_sep_k)
	{
		sep_k_1.first = 0; // level k+1
		
		for (int i = 0; i < sep_k.first; i++) //level k
		{
			// stepping one level
			for (Light_Graph::Neighbor_iterator iter_v = g->Begin_Neighbor (sep_k.second[i]); iter_v != g->End_Neighbor (sep_k.second[i]); iter_v++)
			{
				if(belong_to_chosen_v[*iter_v].first == 0)
				{
					// add the vertex to the chosen set
					chosen_v.second[chosen_v.first] = *iter_v;
					chosen_v.first++;
					
					belong_to_chosen_v[*iter_v].first = 1;
					
					cl->Add_Vertex (*iter_v);
					
					is_available[*iter_v] = false;
				
					if (vs.first > 0)
					{
						vs.first--;
						vs.second[belong_to_chosen_v[*iter_v].second] = vs.second[vs.first];
						// useful if the vertex is taken into account
						belong_to_chosen_v[vs.second[vs.first]].second = belong_to_chosen_v[*iter_v].second;
					}
				
					sep_k_1.second[sep_k_1.first] = *iter_v;
					sep_k_1.first++;
				
				}
			}
		
		}
		
		if (sep_k_1.first <= k)
			good_sep_k = true;
		else
		{
			Connected_Component CC (g);
			
			list<pair<int,vertex *> > cc_list;
			list<pair<int,vertex *> >::iterator iter_list;

			cc_list = CC.Get_Connected_Components (vs,chosen_v);
			
			int sep = 0;
			bool all_eliminated = true;	
			// search of separators in level k+1
			for (iter_list = cc_list.begin(); iter_list != cc_list.end(); iter_list++)
			{	
				bool * belong_to_cc = new bool [g->N()];
			
				for (unsigned int i=0; i<g->N(); i++)
					belong_to_cc[i] = false; 
	
				for (int i=0; i<(*iter_list).first; i++)
					belong_to_cc[(*iter_list).second[i]] = true; 
				
				sep = 0; 
				for (int j=0; j< sep_k_1.first; j++)
				{
					bool found = false;
					for (Light_Graph::Neighbor_iterator iter_v = g->Begin_Neighbor (sep_k_1.second[j]); iter_v != g->End_Neighbor (sep_k_1.second[j]) && !found; iter_v++)
						if (belong_to_cc[*iter_v])
						{	
							sep ++;
							found = true;
						}
				}
				
				if (sep <= k)
				{
					// CC is no more taken into account
					for (int i=0; i<(*iter_list).first; i++)
						belong_to_chosen_v[(*iter_list).second[i]].first = -1; 
				}
				else
					all_eliminated = false;
				
				delete [] belong_to_cc;	
				delete [] (*iter_list).second;
			}
			
			if (all_eliminated)
				good_sep_k = true;
			
			sep_k.first = sep_k_1.first;
			for(int i=0; i< sep_k_1.first; i++)
				sep_k.second[i] = sep_k_1.second[i];
		}
	}
	
	delete [] chosen_v.second;
	delete [] belong_to_chosen_v;
	delete [] sep_k.second;
	delete [] sep_k_1.second;	
}
