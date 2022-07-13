/// \file 
/// \brief Definitions related to the Cardinality_Global_Constraint class

#ifndef CARDINALITY_GLOBAL_CONSTRAINT_H
#define CARDINALITY_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class Cardinality_Global_Constraint: public Global_Constraint      /// This class implements the cardinality global constraint \ingroup core
{
  protected:
    bool closed;          ///< specifies whether the variables must be assigned or no a value among the designed values 
    vector<int> values;   ///< provides the values
  
	public:
		// constructors
		Cardinality_Global_Constraint (vector<Variable *> & var, vector<int> & val, bool is_closed);    ///< constructs a new cardinality global constraint
		Cardinality_Global_Constraint (Cardinality_Global_Constraint & c);	         ///< constructs a new constraint by copying the constraint c
				
		// basic functions
    Constraint * Duplicate () override;  				  ///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Cardinality_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Cardinality_Global_Constraint (*this);
}


inline bool Cardinality_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
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

