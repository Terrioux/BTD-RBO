#include "min_size_separator_cluster_heuristic.h"


//----------------
// basic function
//----------------


Cluster_Node * Min_Size_Separator_Cluster_Heuristic::Choose_Next_Cluster (Cluster_Tree * ct, Cluster_Node * cl, CSP * pb, bool is_good[])
// returns the next cluster to process after the cluster cl
{
	Cluster_Node * c = 0;
	int min = pb->Get_N();
	
  int num_cl = cl->Get_Num();
	
  for (deque<Child_Node *>::iterator iter = ct->Begin_Neighbor(num_cl); iter != ct->End_Neighbor (num_cl); iter++)
  {	
    if ((*iter)->Get_Cluster_Num() != cl->Get_Parent_Num())		// we consider only the separator with a cluster different from the parent cluster
    {
      Cluster_Node * child = (*iter)->Get_Cluster();
      if ((! is_good[child->Get_Num()]) && (child->Size() < min))
      {
        min = child->Size();
        c = child;
      }
    }						
  }
		
	return c;
}
