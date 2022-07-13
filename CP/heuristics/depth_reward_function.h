/// \file 
/// \brief Definitions related to the Depth_Reward_Function class


#ifndef DEPTH_REWARD_FUNCTION_H
#define DEPTH_REWARD_FUNCTION_H

#include "assignment.h"
#include <cmath>

class Depth_Reward_Function                    /// This class implements a reward function for Multi-Armed Bandits based on the depth of the search-tree \ingroup heuristics
{
  protected:
    double sum;         ///< the sum of depth where a wipe-out occurs
    double n;           ///< the number of variables
    int nb_conflicts;   ///< the number of conflicts
  
	public:	
		// constructor
    Depth_Reward_Function (CSP * pb);   ///< constructor

		// basic functions
    void Update_Before_Filtering (Assignment & A, CSP * pb);   ///< updates the data of the reward function on the basis of the current assignment A and for the instance pb before triggering filtering
    void Update_After_Filtering (Assignment & A, CSP * pb);    ///< updates the data of the reward function on the basis of the current assignment A and for the instance pb after triggering filtering
    double Get_Reward ();    ///< returns the reward
    void Reset ();           ///< resets the reward function
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Depth_Reward_Function::Update_Before_Filtering (Assignment & A, CSP * pb)
// updates the data of the reward function on the basis of the current assignment A and for the instance pb before triggering filtering
{
}


inline void Depth_Reward_Function::Update_After_Filtering (Assignment & A, CSP * pb)
// updates the data of the reward function on the basis of the current assignment A and for the instance pb after triggering filtering
{
  if (pb->Get_Last_Conflict_Variable () != 0)
  {
    sum += 1 - A.Get_Size() / n;
    nb_conflicts++;
  }
}

//~ #include<iostream>
inline double Depth_Reward_Function::Get_Reward ()
// returns the reward
{
  //~ cout << sum << " " << nb_conflicts << endl;
  double reward;
  if (nb_conflicts == 0)
    reward = 0;
  else reward = sum / nb_conflicts;
  sum = 0;
  nb_conflicts = 0;
  return reward;
}


inline void Depth_Reward_Function::Reset ()
// resets the reward function
{
  nb_conflicts = 0;
  sum = 0;
}


#endif
