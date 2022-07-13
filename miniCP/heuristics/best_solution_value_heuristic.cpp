#include "best_solution_value_heuristic.h"


//----------------------------
// constructor and destructor
//----------------------------


Best_Solution_Value_Heuristic::Best_Solution_Value_Heuristic (CSP * ref_pb, Value_Heuristic * vh): Value_Heuristic (ref_pb)
// construct a value heuristic based on the best found solution for the instance ref_pb and exploiting vh when no solution is found yet
{
  auxiliary_heuristic = vh;
}


Best_Solution_Value_Heuristic::~Best_Solution_Value_Heuristic ()
// destructor
{
  delete auxiliary_heuristic;
}

