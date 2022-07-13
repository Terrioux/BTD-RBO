#include "mab.h"
#include <cmath>

//----------------------------
// constructor and destructor
//----------------------------


MAB::MAB (unsigned int nb, Depth_Reward_Function * rf, double deg): use_number(nb,0), reward_sum (nb,0)
// constructs a new multi-armed bandit with nb arms and a reward function rf
{
  reward_function = rf;
  arm_number = nb;
  exploration_degree = deg;
  
  current_arm = 0;
  use_number[0] = 1;
  choice_number = 1;
}


MAB::~MAB ()
// destructor
{
  delete reward_function;
}


//-----------------
// basic functions
//-----------------


unsigned int MAB::Choose ()
// returns the index of the chosen arm
{
  // we first reward the previous arm
  reward_sum[current_arm] += reward_function->Get_Reward();
  
  // we choose the next arm  
  if (choice_number >= arm_number)
  {
    double max = -1;
    
    for (unsigned int i = 0; i < arm_number; i++)
    {
      double val = reward_sum[i] / use_number[i] + sqrt ((exploration_degree*log(choice_number) / use_number[i]));

      if (val > max)
      {
        max = val;
        current_arm = i;
      }
    }
  }
  else current_arm = (current_arm + 1) % arm_number;

  use_number[current_arm]++;
  choice_number++;
  
  return current_arm;
}
