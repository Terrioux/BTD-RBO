/// \file 
/// \brief Definitions related to the MAB class


#ifndef MAB_H
#define MAB_H

#include "information.h"
#include "depth_reward_function.h"

class MAB                       /// This class allows to represent Multi-Armed Bandits \ingroup heuristics
{
	protected:
    vector<unsigned int> use_number;     ///< the number of times each arm has been used
    unsigned int arm_number;             ///< the number of arms
    unsigned int current_arm;            ///< the current arm (i.e. the last chosen one)
    unsigned int choice_number;          ///< the number of made choices
    Depth_Reward_Function * reward_function;   ///< the reward function
    vector<double> reward_sum;           ///< the sum of rewards for each arm
    double exploration_degree;           ///< the degree of exploration which controls the trade-off between the rewards and the exploration
    

	public:	
		// constructor
		MAB (unsigned int nb, Depth_Reward_Function * rf, double deg = 2);	    ///< constructs a new multi-armed bandit with nb arms and a reward function rf
		virtual ~MAB ();	        ///< destructor


		// basic functions
		virtual unsigned int Choose ();	    ///< returns the index of the chosen arm
    virtual void Update_Before_Filtering (Assignment & A, CSP * pb);   ///< updates the data of the reward function on the basis of the current assignment A and for the instance pb before triggering filtering
    virtual void Update_After_Filtering (Assignment & A, CSP * pb);    ///< updates the data of the reward function on the basis of the current assignment A and for the instance pb after triggering filtering
    virtual void Reset ();      ///< resets the multi-armed bandit
};

//-----------------------------
// inline function definitions
//-----------------------------


inline void MAB::Update_Before_Filtering (Assignment & A, CSP * pb)
// updates the data of the reward function on the basis of the current assignment A and for the instance pb before triggering filtering
{
  reward_function->Update_Before_Filtering(A,pb);
}


inline void MAB::Update_After_Filtering (Assignment & A, CSP * pb)
// updates the data of the reward function on the basis of the current assignment A and for the instance pb after triggering filtering
{
  reward_function->Update_After_Filtering(A,pb);
}


inline void MAB::Reset ()
// resets the multi-armed bandit
{
  for (unsigned int i = 0; i < arm_number; i++)
  {
    reward_sum[i] = 0;
    use_number[i] = 0;
  }

  current_arm = 0;
  use_number[current_arm]++;
  choice_number = 0;

  if (reward_function != 0)
    reward_function->Reset();
}

#endif
