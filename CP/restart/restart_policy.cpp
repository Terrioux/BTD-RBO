#include "restart_policy.h"

//----------------------------
// constructor and destructor
//----------------------------


Restart_Policy::Restart_Policy (CSP * ref_pb, Geometric_Sequence * seq)
// constructs a restart policy based on the sequence seq
{
  sequence = seq;
  limit = sequence->Get_Current_Term();
  conflict_number_since_last_restart = 0;
  pb = ref_pb;
}



Restart_Policy::~Restart_Policy ()
// destructor
{
  delete sequence;
}
