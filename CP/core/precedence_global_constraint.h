/// \file 
/// \brief Definitions related to the Precedence_Global_Constraint class

#ifndef PRECEDENCE_GLOBAL_CONSTRAINT_H
#define PRECEDENCE_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class Precedence_Global_Constraint: public Global_Constraint      /// This class implements the precedence global constraint \ingroup core
{
  protected:
    long s; ///< the first value
    long t; ///< the second value s.t. s precedes t
  
	public:
		// constructors and destructor
		Precedence_Global_Constraint (vector<Variable *> & var, long value_s, long value_t);  ///< constructs a new precedence global constraint where value_s precedes value_t
		Precedence_Global_Constraint (Precedence_Global_Constraint & c);	           ///< constructs a new constraint by copying the constraint c
				
		// basic functions
    Constraint * Duplicate () override;          	///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	  ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Precedence_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Precedence_Global_Constraint (*this);
}


inline bool Precedence_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
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

