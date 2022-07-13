#include "event_manager.h"
#include "domain.h"


//-------------
// constructor
//-------------


Event_Manager::Event_Manager ()
// constructor
{
  current_time = 1;
  level_time = 1;
}



void Event_Manager::Add_Variable (Variable * x)
// adds the variable x to the set of variables for which events are managed
/// \param[in] x the variable we want to add to the event manager
{
  unsigned int num = x->Get_Num();
  
  if (fix.size() <= num)
    fix.resize(num+1,0);
  
  if (fixed_value.size() <= num)
    fixed_value.resize(num+1,0);
  
  if (dmc.size() <= num)
    dmc.resize(num+1,0);
    
  if (dmc_val.size() <= num)
    dmc_val.resize(num+1);
  dmc_val[num].resize (x->Get_Domain()->Get_Initial_Size(),0);
  
  if (lbc.size() <= num)
    lbc.resize(num+1,0);
  
  if (ubc.size() <= num)
    ubc.resize(num+1,0);
    
  if (last_modif.size() <= num)  
    last_modif.resize (num+1,make_pair(0,NOEV));
}
