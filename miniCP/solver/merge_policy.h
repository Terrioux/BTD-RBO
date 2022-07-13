/// \file 
/// \brief Definitions related to the Merge_Policy class


#ifndef MERGE_POLICY_H
#define MERGE_POLICY_H

#include "csp.h"
#include "cluster_tree.h"

class Merge_Policy				/// this class allows to represent policy for merging dynamically clusters  \ingroup solver
{
	protected:
		int cluster_nb;				    ///< the maximum number of clusters
		int * skip_number;		    ///< this value indicates how much BTD would like to skip this separator and consult variables of the child cluster
		int skip_value_limit;	    ///< this values from which two clusters are merged
    double scaling_factor;		///< the scaling factor used to increase the number of allowed conflicts after each verification
 		unsigned long * limit;		///< the number of allowed conflicts before the next merge for each cluster
		unsigned long * conflict_number_since_last_verify;		///< the number of conflicts since the last merge for each cluster

	
	public:
		// constructor and destructor
		Merge_Policy (unsigned long lim, double sc_factor, int skip_lim, int max_cluster);		///< creates a new policy with limit lim, scaling factor sc_factor and skip limit skip_lim for a decomposition having at most max_cluster
		virtual ~Merge_Policy ();												///< destructor
		
		// basic functions
		virtual bool Verify (int cluster_num);	 	    	///< returns true if the solver is allowed to merge some clusters  and reset all information required by the policy, false otherwise
		virtual void Reset (int cluster_num);	     		  ///< resets the policy parameter (i.e. here the number of backtrack since the last merge)
		void Increment_Skip_Number (int cluster_num);		///< increments by one the value of the skip number for the cluster cluster_num
		void Reset_Skip_Number ();											///< resets the skip number of each cluster
		
		
		virtual pair<int,int> Select_Clusters_To_Merge (CSP * pb, Cluster_Tree * ct, int current_cluster_num);		///< selects the current cluster and one of its neighbors in order to merge them (if the skip value exceeds the limit) returns the pair <number of the cluster merged,number of the resulting cluster>
		void Merge_Clusters (Cluster_Tree * ct, int cluster_num);			///< merges the cluster cluster_num with its parent
};


//--------------------------------
// definition of inline functions
//--------------------------------


inline void Merge_Policy::Reset_Skip_Number ()
// resets the skip number of each cluster
{
	for (int i = 0; i < cluster_nb; i++)
		skip_number[i] = 0;
}


inline void Merge_Policy::Increment_Skip_Number (int cluster_num)
// increments by one the value of the skip number
{
	skip_number[cluster_num]++;
}

inline bool Merge_Policy::Verify (int cluster_num)
// returns true if the solver is allowed to merge some clusters  and reset all information required by the policy, false otherwise
{
	conflict_number_since_last_verify[cluster_num]++;

	if (limit[cluster_num] == 	conflict_number_since_last_verify[cluster_num])
	{
		limit[cluster_num] *= scaling_factor;
		conflict_number_since_last_verify[cluster_num] = 0;
		return true;
	}
	else return false;
}

inline void Merge_Policy::Reset (int cluster_num)
// resets the policy parameter (i.e. here the number of backtrack since the last merge)
{
	conflict_number_since_last_verify[cluster_num] = 0;
}

#endif
