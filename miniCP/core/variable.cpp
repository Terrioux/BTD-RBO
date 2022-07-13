#include "variable.h"
#include "event_manager.h"
#include "interval_domain.h"
#include "set_domain.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Variable::Variable (set<int> & values, unsigned int num_var, string ch, Event_Manager * event_manager, bool is_aux)
// constructs an enumerated variable whose domain is the set values, num is num_var and name is ch, is_aux is set to true if the variable is auxiliary
{
  num = num_var;
  domain = new Set_Domain (event_manager, num, values);
  name = ch;
  is_auxiliary = is_aux;
  event_manager->Add_Variable (this);  
}


Variable::Variable (int a, int b, unsigned int num_var, string ch, Event_Manager * event_manager, bool is_aux)
// constructs an enumerated variable whose domain is [a,b], num is num_var and name is ch, is_aux is set to true if the variable is auxiliary
{
  num = num_var;
  domain = new Interval_Domain (event_manager, num, a, b);
  name = ch;
  is_auxiliary = is_aux;
  event_manager->Add_Variable (this);  
}


Variable::~Variable()
// destructor
{
	delete domain;
}
