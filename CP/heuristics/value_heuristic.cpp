#include "value_heuristic.h"


//----------------------------
// constructor and destructor
//----------------------------


Value_Heuristic::Value_Heuristic (CSP * ref_pb)
// constructs a value heuristic for the instance ref_pb
{
  pb = ref_pb;
  last_solution = 0;
}


Value_Heuristic::~Value_Heuristic ()
// destructor
{  
}

