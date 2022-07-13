/// \file 
/// \brief Definitions related to the Greater_Weighted_Sum_Global_Constraint class

#ifndef GREATER_WEIGHTED_SUM_GLOBAL_CONSTRAINT_H
#define GREATER_WEIGHTED_SUM_GLOBAL_CONSTRAINT_H

#include "weighted_sum_global_constraint.h"


class Greater_Weighted_Sum_Global_Constraint: public Weighted_Sum_Global_Constraint     /// This class implements the weighted sum global constraint with condition > or >= \ingroup core
{
  protected:
    bool strict_inequality;       ///< true if the comparision relies on >, false otherwise

	public:
		// constructors
		Greater_Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, bool is_strict, int cst);	///< constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var weighted by weight from w to the constant cst with respect to > or >= if the boolean is_strict is true
		Greater_Weighted_Sum_Global_Constraint (Greater_Weighted_Sum_Global_Constraint & c);              	///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;      ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
 		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_Relation () override;              ///< returns the relation used to compare the sum and the constant
    bool Is_Strict_Inequality ();                 ///< returns true if the sum must be greater than the constant, false otherwise
  
  private:
    bool Compare (int sum);             ///< returns true if the sum satisfies the constraint, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Greater_Weighted_Sum_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Greater_Weighted_Sum_Global_Constraint (*this);
}


inline bool Greater_Weighted_Sum_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  for (unsigned int i = 0 ; i < arity; i++)
    if (weights[i] > 0)
    {
      if (event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(),ref))
        return true;
    }
    else
      if (event_manager->Exist_Event_Lbc (scope_var[i]->Get_Num(),ref))
        return true;

  return false;
}


inline string Greater_Weighted_Sum_Global_Constraint::Get_Relation ()
// returns the relation used to compare the sum and the constant
{
  return strict_inequality ? "gt" : "ge";
}


inline bool Greater_Weighted_Sum_Global_Constraint::Compare (int sum)
// returns true if the sum satisfies the constraint, false otherwise
{
  return strict_inequality ? sum > constant : sum >= constant;
}


inline bool Greater_Weighted_Sum_Global_Constraint::Is_Strict_Inequality ()
// returns true if the sum must be greater than the constant, false otherwise
{
  return strict_inequality;
}
#endif
