#include "max_weight_sum_root_heuristic.h"


//-------------
// constructor
//-------------


Max_Weight_Sum_Root_Heuristic::Max_Weight_Sum_Root_Heuristic (Erwa_Weights * w)
// constructs the heuristics with its related weights w
{
  weights = w;
}


//----------------
// basic function
//----------------


int Max_Weight_Sum_Root_Heuristic::Choose_Root (Cluster_Tree * ct, CSP * pb)
// chooses a root cluster which maximizes the sum of the weights of the contraint whose scope intersects the cluster
{
	double max_sum = -1;
	int max_sum_num = -1;
	Multi_Hypergraph * h = pb->Get_Structure();
  double sum;
	
	for (map<int,Cluster_Node *>::iterator iter_cl = ct->Begin_List (); iter_cl != ct->End_List(); iter_cl++)
	{
		vector<vertex>::iterator iter;
		list<Edge *>::iterator iter2;
		set<vertex>::iterator iter3;
		sum = 0;
		
		for (iter = iter_cl->second->begin(); iter != iter_cl->second->end(); iter++)
			for (iter2 = h->Begin_Edge_List(*iter); iter2 != h->End_Edge_List(*iter); iter2++)			
			{
				// we look for the first vertex in the edge which also belongs to elements
				iter3 = (*iter2)->Begin();
				while ((iter3 != (*iter2)->End()) && (! iter_cl->second->Is_Element(*iter3)))
					iter3++;
				
				if (*iter3 == *iter)
					sum += weights->Get_Constraint_Weight((*iter2)->Get_Num());
			}
		
		if (max_sum < sum)
		{
			max_sum = sum;
			max_sum_num = iter_cl->first;
		}
	}
	
	return max_sum_num;
}
