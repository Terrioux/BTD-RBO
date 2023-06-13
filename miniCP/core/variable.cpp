#include "variable.h"
#include "event_manager.h"
#include "interval_domain.h"
#include "set_domain.h"
#include "bound_domain.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Variable::Variable (set<long> & values, unsigned int num_var, string ch, Event_Manager * event_manager, bool is_init, bool is_aux)
// constructs an enumerated variable whose domain is the set values, num is num_var and name is ch, is_init is set to true if the variable appears initially in the problem, is_aux is set to true if the variable is auxiliary
{
  num = num_var;
  domain = new Set_Domain (event_manager, num, values);
  name = ch;
  is_initial = is_init;
  if (is_initial)
    is_auxiliary = false;
  else
    is_auxiliary = is_aux;
  event_manager->Add_Variable (this);  
}


Variable::Variable (long a, long b, unsigned int num_var, string ch, Event_Manager * event_manager, bool is_init, bool is_aux)
// constructs an enumerated variable whose domain is [a,b], num is num_var and name is ch, is_init is set to true if the variable appears initially in the problem,  is_aux is set to true if the variable is auxiliary
{
  num = num_var;
  domain = new Interval_Domain (event_manager, num, a, b);
  name = ch;
  is_initial = is_init;
  if (is_initial)
    is_auxiliary = false;
  else
    is_auxiliary = is_aux;
  event_manager->Add_Variable (this);  
}


Variable::Variable (long val, unsigned int num_var, string ch, Event_Manager * event_manager)
// constructs an auxiliary variable whose domain is restricted to val, num is num_var and name is ch (this variable is used to represent bound variable)
{
  num = num_var;
  domain = new Bound_Domain (event_manager, num, val);
  name = ch;
  is_initial = false;
  is_auxiliary = true;
  event_manager->Add_Variable (this);  
}


Variable::~Variable()
// destructor
{
	delete domain;
}
