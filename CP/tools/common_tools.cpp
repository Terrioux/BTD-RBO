#include "common_tools.h"

// restart policies
#include "restart_policy.h"
#include "geometric_sequence.h"


// variable heuristic header files
#include "reset_restart_erwa_variable_heuristic.h"
#include "mab_variable_heuristic.h"
#include "mab.h"
#include "depth_reward_function.h"
#include "last_conflict_combined_variable_heuristic.h"

// value heuristic header files
#include "lex_value_heuristic.h"
#include "best_solution_value_heuristic.h"


#include "cop.h"


Restart_Policy * Restart_Policy_Initialization (CSP * pb)
// initialization of the restart policy
{
	return new Restart_Policy (pb,new Geometric_Sequence (50,1.05));
}


Depth_Reward_Function * Reward_Function_Initialization (CSP * pb)
{
  return new Depth_Reward_Function(pb);;
}


MAB * MAB_Initialization (CSP * pb, unsigned int nb_arms)
// returns a multi-armed bandits with nb_arms arms
{ 
  return new MAB(nb_arms,Reward_Function_Initialization (pb),1);
}


Variable_Heuristic * Variable_Heuristic_Initialization (CSP * pb)
// initialization of the variable heuristic depending on the chosen options
{
	Variable_Heuristic * var_heur = 0;
  vector<Variable_Heuristic*> heur_set;
                           
  for (unsigned int i = 1; i <= 9; i++)
    heur_set.push_back (new Reset_Restart_Erwa_Variable_Heuristic(pb,0.1 * i,0.06,0.000001,0.0001));
  var_heur = new MAB_Variable_Heuristic (pb,MAB_Initialization (pb,heur_set.size()), heur_set);

  return new Last_Conflict_Combined_Variable_Heuristic(pb,var_heur);
}


Value_Heuristic * Value_Heuristic_Initialization (CSP * pb)
// initialization of the value heuristic depending on the chosen options
{
	if (dynamic_cast<COP*>(pb) == 0)
    return new Lex_Value_Heuristic(pb);
  else
    return new Best_Solution_Value_Heuristic(pb,new Lex_Value_Heuristic(pb));
}
