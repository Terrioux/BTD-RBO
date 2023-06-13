/// \file 
/// \brief Sources related to the COP class

#include "cop.h"
#include "equal_binary_constraint.h"
#include <climits>


//-----------------------------
// constructors and destructor
//-----------------------------


COP::COP (Event_Manager * em, string pb_name): CSP (em,pb_name)
// constructs an empty COP whose name is pb_name and for which the events are managed by em
/// \param[in] em the event manager
/// \param[in] pb_name  the name of the COP instance
{
  objective_function = 0;
  lower_bound_variable = new Variable (LONG_MIN,0,"lower_bound",em);
  variables.push_back(lower_bound_variable);
	h->Add_Vertex();

  upper_bound_variable = new Variable (LONG_MAX,1,"upper_bound",em);
  variables.push_back(upper_bound_variable);
	h->Add_Vertex();  
}


COP::~COP ()
// destructor
{
}


//-----------------
// basic functions
//-----------------


void COP::Set_Objective_Function (Objective_Function * obj)
//< sets the objective function
{
  objective_function = obj;
  objective_function->Update_Problem(this);
  
  // we update the lower and upper bounds
  pair<long,long> min_max = objective_function->Get_Cost_Interval();
  
  Update_Lower_Bound (min_max.first);
  Update_Upper_Bound (min_max.second);
}

