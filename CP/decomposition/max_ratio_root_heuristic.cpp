#include "max_ratio_root_heuristic.h"

int Max_Ratio_Root_Heuristic::Choose_Root (Cluster_Tree * ct, CSP * pb)
// returns the choice of the root cluster according to the considered heuristic
{
	
	double ratio, max_ratio = -1.0;
	int num = 0;
	
	for (map<int,Cluster_Node *>::iterator iter = ct->Begin_List (); iter != ct->End_List (); iter++)
	{ 
		ratio = iter->second->Get_Nb_Edges () / (iter->second->Size() - 1.0);
		if (ratio > max_ratio)
		{
			max_ratio = ratio;
			num = iter->first;
		}
	}
	return num;
}
