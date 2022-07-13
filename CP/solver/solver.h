/// \file 
/// \brief Definitions related to the Solver class

#ifndef MAC_BTD_SOLVER_MERGE_H
#define MAC_BTD_SOLVER_MERGE_H

using namespace std;

#include "value_heuristic.h"
#include "variable_heuristic.h"
#include "ac.h"
#include <cfloat>

#include "root_heuristic.h"
#include "cluster_heuristic.h"
#include <tree_decomposition.h>
#include "csp_tree_decomposition.h"
#include "restart_policy.h"
#include "ac.h"
#include "csp_child_node.h"
#include "merge_policy.h"

enum Solver_State ///< type for representing the state of solver
{
  NOT_RUN,                      ///< the solver has not been run yet
  HAS_PROVED_CONSISTENCY,       ///< the solver has been run and has proved the consistency of the instance
  HAS_PROVED_INCONSISTENCY,     ///< the solver has been run and has proved the inconsistency of the instance
  STOPPED                       ///< the solver has been run and has been stopped
};     


class Solver	    	/// this class implements the algorithm BTD-MAC-Merge for solving CSP instances  \ingroup solver
{
	protected:
		Solver_State state;													  ///< the current state of the solver
		Variable_Heuristic * variable_heuristic;			///< the variable heuristic used in the solver for ordering the variables and so for choosing the next variable to be assigned
		Value_Heuristic * value_heuristic;						///< the value heuristic used in the solver for ordering the values of each variable
		Deletion_Stack * ds;													///< the stack of value deletion
		double time_limit;														///< the time limit 
		Assignment A;																	///< the current assignment of the solver
		CSP * pb;																			///< the instance to solve

  
		CSP_Tree_Decomposition * td;											///< the tree-decomposition used for the solving
		Root_Heuristic * root_heuristic;									///< the root heuristic used in the solver for choosing the first cluster
		Cluster_Heuristic * cluster_heuristic;						///< the cluster heuristic used in the solver for choosing the next cluster to study (we consider an heuristic instance per cluster)
    vector<Variable*> * candidates;                   ///< the set of variables which can be chosen by the variable heuristic for each cluster

		AC * ac;																		///< the AC filtering
		Restart_Policy * restart_policy;						///< the global restart policy
		Restart_Policy * local_restart_policy;			///< the local restart policy
		Root_Heuristic * restart_root_heuristic;		///< the heuristic used at each restart for choosing the new root cluster

    int max_value_number; 		        ///< the maximal number of values for a given cluster tree
    int max_ct_size;                  ///< the maximal number of variables in a cluster tree
  	bool * is_negative_node;	    		///< true if the node is a negative node, false otherwise
    Variable ** variable_node;		    ///< variable_node[i] is the number of the variable for the node i
    int * value_node;   							///< value_node[i] is the value assigned to the variable of the node i
    int ** reset_good; 				        ///< the clusters for which the good must be reset when the variable is unassigned
    int * nb_reset_good;      				///< the number of cluster to reset for each variable
    int * cluster_begin_depth;	      ///< the depth at which the exploration of a cluster has been started
   	pair<int,int> * nogood;       		///< represents a nogood
    Domain ** empty_domain;				    ///< list of empty domain
    int * unassigned_variable_number;	///< the number of unassigned variables in each cluster	
    bool * is_good;		                ///< true if the current assignment for the given cluster is a good, false otherwise
    
  	int current_cluster_num;					///< the number of the current cluster
    Cluster_Node * current_cluster;		///< the current cluster
    int depth;	    				        	///< the depth of the current branch in the search tree	
    Multi_Hypergraph * h;         		///< the considered constraint hypergraph
    
    Merge_Policy * merge_policy; 										///< the policy used for merging clusters
			
		CSP_Tree_Decomposition * original_td_merge; 		///< the orignal considered tree-decomposition (i.e. before any merging)
		

	public:
		// constructor and destructor
		Solver (CSP * inst, Variable_Heuristic * var_heur, Value_Heuristic * val_heur, AC * cc, Deletion_Stack * stack, Tree_Decomposition * TD, Root_Heuristic * root_heur, Cluster_Heuristic * cluster_heur, Restart_Policy *rp, Restart_Policy * local_rp, Root_Heuristic * rs_root_heur, Merge_Policy * local_merge_p,  double limit=DBL_MAX);	///< constructs a solver in order to solve the instance inst by using the variable heuristic var_heur, the value heuristic val_heur, the consistency check,the deletion stack stack, the tree-decomposition TD, the root cluster heuristic root_heur, the cluster heuristic cluster_heur, the global restart policy rp, the local restart policy local_rp, the cluster root heuristic rs_root_heur for choosing the root cluster at each restart and the policy local_merge_p for merging clusters. The runtime is limited to limit. Child nodes rely on lexicographical trees.
		~Solver ();			///< destructor
		

		// basic functions
    void Init ();                             ///< initializes general data-structure
		Solver_State Solve ();        					  ///< solves the instance and returns the solver state when the solving finishes by proving (in)consistency or by reaching the timeout
		Assignment & Get_Solution ();         	  ///< returns the solution if the CSP is consistent or an empty assignment if the CSP is inconsistent or has not been solved yet		
		Tree_Decomposition * Get_Initial_TD ();		///< returns the initial tree-decomposition 
		Tree_Decomposition * Get_TD ();						///< returns the last tree-decomposition used by the algorithm, 0 if no tree-decomposition has been computed

};

inline Tree_Decomposition * Solver::Get_Initial_TD ()
// returns the last tree-decomposition used by the algorithm, 0 if no tree-decomposition has been computed
{
	return original_td_merge;
}



inline Tree_Decomposition * Solver::Get_TD ()
// returns the last tree-decomposition used by the algorithm, 0 if no tree-decomposition has been computed
{
	return td;
}

#endif

