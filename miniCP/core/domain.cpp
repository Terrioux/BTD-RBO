#include "domain.h"

//-------------
// constructor 
//-------------

Domain::Domain (Event_Manager * evm, unsigned int num_var, unsigned int init_size): dom (init_size), position (init_size), min(init_size), max(init_size)
// constructs a domain for the variable num_var and the event manager evm
{
  var = num_var;
  event_manager = evm;
  
  initial_size = init_size; 
  size = initial_size;

  for (unsigned int i = 0; i < init_size; i++)
  {
    dom[i] = i;
    position[i] = i;
  }
  
  min[initial_size-1] = 0;
  max[initial_size-1] = initial_size-1;
}
