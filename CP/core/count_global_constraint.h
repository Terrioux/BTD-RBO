/// \file 
/// \brief Definitions related to the Count_Global_Constraint class

#ifndef COUNT_GLOBAL_CONSTRAINT_H
#define COUNT_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class Count_Global_Constraint: public Global_Constraint   /// This class implements the global constraint Count \ingroup core
{
	protected:
    set<int> values;       ///< the set of values to which the variables must belong
  
	public:
		// constructors and destructor
		Count_Global_Constraint (vector<Variable *> & var, set<int> & value_set);				  ///< constructs a new constraint which involves the variable in var and whose relation imposes that var[0]=|var[1...] in value_set|
		Count_Global_Constraint (Count_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		
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


inline Constraint * Count_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Count_Global_Constraint (*this);
}


inline bool Count_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();

  if (scope_var[0]->Get_Domain()->Get_Size() == 1)
    return true;

  for (unsigned int i = 1 ; i < arity; i++)
    if ((event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(),ref)) || (event_manager->Exist_Event_Dmc (scope_var[i]->Get_Num(),ref)))
      return true;
  
  return false;
}

#endif
