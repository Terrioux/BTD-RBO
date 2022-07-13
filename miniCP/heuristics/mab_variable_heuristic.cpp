#include "mab_variable_heuristic.h"

//----------------------------
// constructor and destructor
//----------------------------


MAB_Variable_Heuristic::MAB_Variable_Heuristic (CSP * ref_pb, MAB * bandit, vector<Variable_Heuristic *> & possible_heur): Variable_Heuristic (ref_pb)
// constructs a variable heuristic for the CSP instance ref_pb based on a multi-armed bandit in order to select a variable heuristic among ones provided in possible_heur
{
  mab = bandit;
  possible_heuristics = possible_heur;
  current_arm = 0;
}


MAB_Variable_Heuristic::~MAB_Variable_Heuristic ()
// destructor
{
  for (vector<Variable_Heuristic *>::iterator iter = possible_heuristics.begin(); iter != possible_heuristics.end(); iter++)
    delete *iter;
  delete mab;
}
