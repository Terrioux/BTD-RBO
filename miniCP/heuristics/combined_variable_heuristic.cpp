#include "combined_variable_heuristic.h"

//----------------------------
// constructor and destructor
//----------------------------


Combined_Variable_Heuristic::Combined_Variable_Heuristic (CSP * ref_pb, Variable_Heuristic * var_heur): Variable_Heuristic(ref_pb)
// constructs a variable heuristic for the CSP instance ref_pb
{
  vh = var_heur;
}



Combined_Variable_Heuristic::~Combined_Variable_Heuristic ()
// destructor
{
  delete vh;
}
