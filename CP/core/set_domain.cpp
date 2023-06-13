#include "set_domain.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Set_Domain::Set_Domain (Event_Manager * evm, unsigned int num_var, set<long> & value_set): Domain (evm,num_var,value_set.size())
// constructs a domain defined by the set value_set
{
  real_value.resize(value_set.size());
  int size = 0;
  for (auto val : value_set)
  {
    real_value[size] = val;
    real_value_to_value.insert(make_pair(val,size));
    size++;
  }
}
