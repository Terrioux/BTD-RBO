#include "depth_reward_function.h"


//-------------
// constructor 
//-------------

Depth_Reward_Function::Depth_Reward_Function (CSP * pb)
// constructor
{
  n = pb->Get_N();
  sum = 0;
  nb_conflicts = 0;
}
