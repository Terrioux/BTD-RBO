#include "merge_policy.h"
#include "csp_cluster_tree.h"


//----------------------------
// constructor and destructor
//----------------------------


Merge_Policy::Merge_Policy (unsigned long lim, double sc_factor, int skip_lim, int max_cluster)
// creates a new policy with limit lim, scaling factor sc_factor and skip limit skip_lim for a decomposition having at most max_cluster
{
	skip_value_limit = skip_lim;
	cluster_nb = max_cluster;
	skip_number = new int [cluster_nb];

	limit = new unsigned long [cluster_nb];	
	conflict_number_since_last_verify = new unsigned long [cluster_nb];
	
	for (int i = 0; i  < cluster_nb; i++)
	{
    skip_number[i] = 0;
		limit[i] = lim;
		conflict_number_since_last_verify[i] = 0;
	}

  scaling_factor = sc_factor;
}


Merge_Policy::~Merge_Policy ()
// destructor
{
	delete [] limit;
	delete [] conflict_number_since_last_verify;
	delete [] skip_number;
}


//-----------------
// basic functions
//-----------------


pair<int,int> Merge_Policy::Select_Clusters_To_Merge (CSP * pb, Cluster_Tree * ct, int current_cluster_num)
// selects the current cluster and one of its neighbors in order to merge them (if the skip value exceeds the limit) returns the pair <number of the cluster merged,number of the resulting cluster>
{
	pair<int,int> chosen_pair = pair<int,int>(-1,-1);
	
	// find the max skip value of the separators with childs and parent
	int max = -1;
	int cluster_max = -1;
	int cluster_result = -1;

	// childs
	for (deque<Child_Node *>::iterator iter = ct->Begin_Neighbor(current_cluster_num); iter != ct->End_Neighbor(current_cluster_num); iter++)
	{
		// it is the child of the current cluster
		if ((*iter)->Get_Cluster()->Get_Parent_Num() == current_cluster_num)
			{
				if (skip_number[(*iter)->Get_Cluster_Num()] > max && (*iter)->Size() > 10) // separator of size less than 10 are not subject to merging
				{
					max = skip_number[(*iter)->Get_Cluster_Num()];
					cluster_max = (*iter)->Get_Cluster_Num();
					cluster_result = current_cluster_num;
				}
			}
	}
	
	if (max >= skip_value_limit)
	{	
		chosen_pair.first = cluster_max;
		chosen_pair.second = cluster_result;		
	}
	
	return chosen_pair;
}


void Merge_Policy::Merge_Clusters (Cluster_Tree * ct, int cluster_num)
// merges the cluster cluster_num with its parent
{
	dynamic_cast<CSP_Cluster_Tree *>(ct)->Merge_Clusters(ct->Get_Cluster(cluster_num));
}	


