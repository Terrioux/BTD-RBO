/// \file 
/// \brief Definitions related to the Equal_Sum_Global_Constraint class

#ifndef EQUAL_SUM_GLOBAL_CONSTRAINT_H
#define EQUAL_SUM_GLOBAL_CONSTRAINT_H

#include "sum_global_constraint.h"


class Equal_Sum_Global_Constraint: public Sum_Global_Constraint   /// This class implements the sum global constraint with condition = \ingroup core
{
	protected:
		int * values;     ///< the value of each variable

	public:
		// constructors and destructor
		Equal_Sum_Global_Constraint (vector<Variable *> & var, long cst);							///< constructs a new constraint which involves the variable in var and which checks whether the sum of the values of the variables in var is equal to the constant cst
		Equal_Sum_Global_Constraint (Equal_Sum_Global_Constraint & cb);		///< constructs a new constraint by copying the constraint c for the CSP pb (we assume that the variables in the scope of c have the same number in the CSP pb)
		~Equal_Sum_Global_Constraint ();													 			   						///< destructor
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_Relation () override;              ///< returns the relation used to compare the sum and the constant
  
  private:
    bool Compare (long sum);                     ///< returns true if the sum satisfies the constraint, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Equal_Sum_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Equal_Sum_Global_Constraint (*this);
}


inline bool Equal_Sum_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  for (unsigned int i = 0 ; i < arity; i++)
    if ((event_manager->Exist_Event_Lbc (scope_var[i]->Get_Num(),ref)) || (event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(),ref)))
      return true;
  return false;
}


inline string Equal_Sum_Global_Constraint::Get_Relation ()
// returns the relation used to compare the sum and the constant
{
  return "eq";
}


inline bool Equal_Sum_Global_Constraint::Compare (long sum)
// returns true if the sum satisfies the constraint, false otherwise
{
  return sum == constant;
}

#endif
