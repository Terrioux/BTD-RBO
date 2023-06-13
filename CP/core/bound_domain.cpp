#include "bound_domain.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Bound_Domain::Bound_Domain (Event_Manager * evm, unsigned int num_var, long val): Domain (evm,num_var,1)
// constructs a domain defined by the values of [a,b]
{
  value = val;
}
