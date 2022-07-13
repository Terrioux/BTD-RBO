/// \file 
/// \brief Definitions related to the NValues_Global_Constraint class

#ifndef NVALUES_GLOBAL_CONSTRAINT_H
#define NVALUES_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class NValues_Global_Constraint: public Global_Constraint   /// This class implements the global constraint NValues \ingroup core
{
	protected:
    set<int> all_values;		///< the set of values which belong currently to the domain of at least one variable in the constraint's scope
    set<int> fixed_values;  ///< the set of real values for singleton variables

  
	public:
		// constructors and destructor
		NValues_Global_Constraint (vector<Variable *> & var);				  ///< constructs a new constraint which involves the variable in var and whose relation imposes that var[0]=NValues(var[1...])
		NValues_Global_Constraint (NValues_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		~NValues_Global_Constraint ();	                            	///< destructor
		
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


inline Constraint * NValues_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new NValues_Global_Constraint (*this);
}


inline bool NValues_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
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
