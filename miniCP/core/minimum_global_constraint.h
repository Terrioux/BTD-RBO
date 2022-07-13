/// \file 
/// \brief Definitions related to the Minimum_Global_Constraint class

#ifndef MINIMUM_GLOBAL_CONSTRAINT_H
#define MINIMUM_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class Minimum_Global_Constraint: public Global_Constraint      /// This class implements the minimum global constraint (x0 = min (x1,...))  \ingroup core
{
	public:
		// constructors and destructor
		Minimum_Global_Constraint (vector<Variable *> & var);        ///< constructs a new minimum global constraint
		Minimum_Global_Constraint (Minimum_Global_Constraint & c);	 ///< constructs a new constraint by copying the constraint c
				
		// basic functions
    Constraint * Duplicate () override;  		  		///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		  	///< returns true if the tuple t satisfies the constraint, false otherwise
  	bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Minimum_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Minimum_Global_Constraint (*this);
}


inline bool Minimum_Global_Constraint::Impacted_By_Last_Events (CSP * pb,timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  if ((event_manager->Exist_Event_Lbc (scope_var[0]->Get_Num(),ref)) || (ref == 0))
    return true;
  else
    {
      for (unsigned int j = 1; j < arity; j++)
      {
        if ((event_manager->Exist_Event_Lbc (scope_var[j]->Get_Num(),ref)) || (event_manager->Exist_Event_Dmc (scope_var[j]->Get_Num(),ref)))
          return true;
      }
      return false;
    }
}
#endif
