#include "interval_domain.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Interval_Domain::Interval_Domain (Event_Manager * evm, unsigned int num_var, long a, long b): Domain (evm,num_var,b-a+1)
// constructs a domain defined by the values of [a,b]
{
  value_min = a;
  value_max = b;
}
