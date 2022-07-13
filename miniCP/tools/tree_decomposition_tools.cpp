#include "tree_decomposition_tools.h"


// tree-decomposition header files
#include <clique_first_cluster_heuristic.h>
#include <enhanced_heuristic_tree_decomposition.h>
#include "sep_k_neighbors_next_vertices_heuristic.h"


// root cluster heuristic header files
#include "max_ratio_root_heuristic.h"
#include "max_weight_sum_root_heuristic.h"

// separator heuristic header files
#include "min_size_separator_cluster_heuristic.h"

Root_Heuristic * Root_Heuristic_Initialization (int num, Variable_Heuristic *vh)
// returns the root cluster heuristic corresponding to the CSP  the number num
{
  Root_Heuristic * root_cluster_heur = 0;

  switch (num)
  {
  case 6: root_cluster_heur = new Max_Ratio_Root_Heuristic;
          break;
  case 7: root_cluster_heur = new Max_Weight_Sum_Root_Heuristic (vh->Get_Weights());
						break;
	}
	
	return root_cluster_heur;
}


Cluster_Heuristic * Next_Cluster_Heuristic_Initialization ()
// returns the next cluster heuristic corresponding to number num
{
	return new Min_Size_Separator_Cluster_Heuristic;
}


Tree_Decomposition * Tree_Decomposition_Initialization (CSP * pb)
// initialization of the tree-decomposition
{
	// computation of the tree-decomposition related to the CSP pb
	Graphical_Structure * gs = pb->Get_Structure();
	
	Tree_Decomposition * TD = 0;
		
  Clique_First_Cluster_Heuristic * fch = new Clique_First_Cluster_Heuristic(gs);

  Light_Graph * g = gs->Get_Primal_Graph ();
  
  Sep_K_Neighbors_Next_Vertices_Heuristic * nvh = new Sep_K_Neighbors_Next_Vertices_Heuristic (g, 1000000);
  
  TD = new Enhanced_Heuristic_Tree_Decomposition (gs,fch,nvh);
  
	return TD;
}
