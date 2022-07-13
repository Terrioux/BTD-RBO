/// \file 
/// \brief Definitions related to the Cumulative_Global_Constraint class

#ifndef CUMULATIVE_GLOBAL_CONSTRAINT_H
#define CUMULATIVE_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class Cumulative_Global_Constraint: public Global_Constraint   /// This class implements the global constraint Cumulative \ingroup core
{
	protected:
    unsigned int task_number;     ///< the number of tasks
    int min_time;                 ///< the minimum value for the time
    int max_time;                 ///< the maximum value for the time
    set<int> to_check;            ///< the value of the time we have to check
	
	public:
		// constructors and destructor
		Cumulative_Global_Constraint (vector<Variable *> & var);				    ///< constructs a new constraint which involves the variable in var and whose relation imposes that all the variables are equal
		Cumulative_Global_Constraint (Cumulative_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override;				  	///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
 		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Cumulative_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Cumulative_Global_Constraint (*this);
}


inline bool Cumulative_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  if ((ref == 0) || (event_manager->Exist_Event_Ubc (scope_var[arity-1]->Get_Num(), ref)))
    return true;

  for (unsigned int i = 0; i < task_number; i++)
    if ((event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(),ref)) ||(event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(), ref)) || (event_manager->Exist_Event_Lbc (scope_var[i+task_number]->Get_Num(), ref)) || (event_manager->Exist_Event_Lbc (scope_var[i+2*task_number]->Get_Num(), ref)))
      return true;
  
  return false;
}

#endif
