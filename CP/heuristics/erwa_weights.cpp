#include "erwa_weights.h"


//-------------
// constructor
//-------------


Erwa_Weights::Erwa_Weights (CSP * ref_pb, double alpha_val, double beta_val, double epsilon_val): weights (ref_pb->Get_M()), conflict(ref_pb->Get_M(),0)
// constructs the ERWA weights for the constraints of the CSP instance ref_pb
{
  pb = ref_pb;
  alpha_init = alpha_val;
  alpha = alpha_val;
  beta = beta_val;
  epsilon = epsilon_val;
  
  for (unsigned int i = 0; i < pb->Get_M(); i++)
  {
    Constraint * c = pb->Get_Constraint (i);
    if ((dynamic_cast<Nogood_Base_Global_Constraint *>(c) == 0) && (dynamic_cast<Unary_Nogood_Base_Global_Constraint *>(c) == 0))
      weights[i] = -1;
    else weights[i] = 0;
  }
  
  conflict_number = 0;
}
