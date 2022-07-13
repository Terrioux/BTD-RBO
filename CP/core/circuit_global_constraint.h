/// \file 
/// \brief Definitions related to the Circuit_Global_Constraint class

#ifndef CIRCUIT_GLOBAL_CONSTRAINT_H
#define CIRCUIT_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class Circuit_Global_Constraint: public Global_Constraint   /// This class implements the global constraint Circuit \ingroup core
{
	protected:
	
	public:
		// constructors and destructor
		Circuit_Global_Constraint (vector<Variable *> & var);											///< constructs a new constraint which involves the variable in var and whose relation imposes that all the variables are equal
		Circuit_Global_Constraint (Circuit_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		~Circuit_Global_Constraint ();													 			   						///< destructor
		
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


inline Constraint * Circuit_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Circuit_Global_Constraint (*this);
}


inline bool Circuit_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  if (ref == 0)
    return true;
  
  Event_Manager * event_manager = pb->Get_Event_Manager();
  for (set<unsigned int>::iterator iter = scope.begin(); iter != scope.end(); iter++)
    if ((event_manager->Exist_Event_Dmc (*iter,ref)) || (event_manager->Exist_Event_Fix (*iter,ref)))
      return true;
  
  return false;
}

#endif
