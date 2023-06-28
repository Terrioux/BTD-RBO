/// \file 
/// \brief Definitions related to the Less_Sum_Global_Constraint class

#ifndef LESS_SUM_GLOBAL_CONSTRAINT_H
#define LESS_SUM_GLOBAL_CONSTRAINT_H

#include "sum_global_constraint.h"
#include <algorithm>
#include <functional>

class Less_Sum_Global_Constraint: public Sum_Global_Constraint   /// This class implements the sum global constraint with condition < or <= \ingroup core
{
  protected:
    bool strict_inequality;       ///< true if the comparision relies on <, false otherwise
    
	public:
		// constructors
		Less_Sum_Global_Constraint (vector<Variable *> & var, bool strict, long cst);			///< constructs a new constraint which involves the variable in var and  compares the sum of the values of the variables in var to the constant cst with respect to < or <= if the boolean strict is true
		Less_Sum_Global_Constraint (Less_Sum_Global_Constraint & c);  	///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	                  ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
 		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	  ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_Relation () override;              ///< returns the relation used to compare the sum and the constant
    bool Is_Strict_Inequality ();                 ///< returns true if the sum must be less than the constant, false otherwise

  private:
    bool Compare (long sum);                     ///< returns true if the sum satisfies the constraint, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Less_Sum_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Less_Sum_Global_Constraint (*this);
}


inline bool Less_Sum_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  for (set<unsigned int>::iterator iter = scope.begin(); iter != scope.end(); iter++)
    if (event_manager->Exist_Event_Lbc (*iter,ref))
      return true;
  
  return false;
}


inline string Less_Sum_Global_Constraint::Get_Relation ()
// returns the relation used to compare the sum and the constant
{
  return strict_inequality ? "lt" : "le";
}


inline bool Less_Sum_Global_Constraint::Compare (long sum)
// returns true if the sum satisfies the constraint, false otherwise
{
  return strict_inequality ? sum < constant : sum <= constant;
}


inline bool Less_Sum_Global_Constraint::Is_Strict_Inequality ()
// returns true if the sum must be less than the constant, false otherwise
{
  return strict_inequality;
}

#endif
