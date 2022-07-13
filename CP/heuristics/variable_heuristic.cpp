#include "variable_heuristic.h"

//----------------------------
// constructor and destructor
//----------------------------


Variable_Heuristic::Variable_Heuristic (CSP * ref_pb)
// constructs a variable heuristic for the CSP instance ref_pb
{
  pb = ref_pb;
  weights = 0;
  has_original_weights = true;
}



Variable_Heuristic::~Variable_Heuristic ()
// destructor
{
  if (has_original_weights)
    delete weights;
}
