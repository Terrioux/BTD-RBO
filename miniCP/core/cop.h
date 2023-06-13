/// \file 
/// \brief Definitions related to the COP class

#ifndef COP_H
#define COP_H

#include "objective_function.h"
#include "bound_domain.h"

class COP: public CSP      /// This class allows to represent COP instances \ingroup core
{
	protected:
    Objective_Function * objective_function;        ///< the objective function 
    Variable * lower_bound_variable;                ///< the variable representing the lower bound of the objective
    Variable * upper_bound_variable;                ///< the variable representing the upper bound of the objective
    
	public:
		// constructors and destructor
		COP (Event_Manager * em=0, string pb_name="");  ///< constructs an empty CSP whose name is pb_name and for which the events are managed by em
		~COP ();	        				                      ///< destructor
		
		// basic functions
    void Set_Objective_Function (Objective_Function * obj);   ///< sets the objective function
    Objective Get_Objective ();                     ///< returns the objective we want to reach
    string Get_Criterion_Information ();            ///< returns the information about the objective function
    long Get_Cost (Assignment & A);                 ///< returns the value of the objective function for the assignment A
    
    Variable * Get_Lower_Bound_Variable ();         ///< returns the lower bound variable
    Variable * Get_Upper_Bound_Variable ();         ///< returns the upper bound variable
    void Update_Lower_Bound (long val);             ///< updates the value of the lower bound
    void Update_Upper_Bound (long val);             ///< updates the value of the upper bound
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Objective COP::Get_Objective ()
// returns the objective we want to reach
{
  if (objective_function == 0)
    throw ("Error: the objective function is not set");
  return objective_function->Get_Objective();
}


inline string COP::Get_Criterion_Information ()
// returns the information about the objective function
{
  if (objective_function == 0)
    throw ("Error: the objective function is not set");
  return objective_function->Get_Criterion_Information();
}


inline long COP::Get_Cost (Assignment & A)
// returns the value of the objective function for the assignment A
{
  if (objective_function == 0)
    throw ("Error: the objective function is not set");
  return objective_function->Get_Cost(A);  
}

inline Variable * COP::Get_Lower_Bound_Variable ()
// returns the lower bound variable
{
  return lower_bound_variable;
}


inline Variable * COP::Get_Upper_Bound_Variable ()
// returns the upper bound variable
{
  return upper_bound_variable;
}


inline void COP::Update_Lower_Bound (long val)
// updates the value of the lower bound
{
  dynamic_cast<Bound_Domain*>(lower_bound_variable->Get_Domain())->Update_Value(val);
  event_manager->Add_Event_Fix (lower_bound_variable->Get_Num(), 0);
  objective_function->Update_From_Bounds ();
}


inline void COP::Update_Upper_Bound (long val)
// updates the value of the upper bound
{
  dynamic_cast<Bound_Domain*>(upper_bound_variable->Get_Domain())->Update_Value(val);
  event_manager->Add_Event_Fix (upper_bound_variable->Get_Num(), 0);
  objective_function->Update_From_Bounds ();
}

#endif
