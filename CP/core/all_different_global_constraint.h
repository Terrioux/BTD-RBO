/// \file 
/// \brief Definitions related to the All_Different_Global_Constraint class

#ifndef ALL_DIFFERENT_GLOBAL_CONSTRAINT_H
#define ALL_DIFFERENT_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class All_Different_Global_Constraint: public Global_Constraint      /// This class implements the all-diff global constraint \ingroup core
{
  protected:
    int * val;     		   		///< the values representing a tuple
    int * real_val;  		 		///< the values representing a real tuple
    set<int> all_values;		///< the set of values which belong currently to the domain of at least one variable in the constraint's scope
    int * candidates;				///< the next domains to proceed in the method Propagate
  
	public:
		// constructors and destructor
		All_Different_Global_Constraint (vector<Variable *> & var);              ///< constructs a new all-diff global constraint
		All_Different_Global_Constraint (All_Different_Global_Constraint & c);	 ///< constructs a new constraint by copying the constraint c
		~All_Different_Global_Constraint ();														         ///< destructor
				
		// basic functions
    Constraint * Duplicate () override;  				  ///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * All_Different_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new All_Different_Global_Constraint (*this);
}


inline bool All_Different_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
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

