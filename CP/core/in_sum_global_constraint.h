/// \file 
/// \brief Definitions related to the In_Sum_Global_Constraint class

#ifndef IN_SUM_GLOBAL_CONSTRAINT_H
#define IN_SUM_GLOBAL_CONSTRAINT_H

#include "sum_global_constraint.h"


class In_Sum_Global_Constraint: public Sum_Global_Constraint    /// This class implements the sum global constraint with condition in \ingroup core
{
  protected:
    long constant2;      ///< the upper bound value of the interval
  
	public:
		// constructors and destructor
		In_Sum_Global_Constraint (vector<Variable *> & var, long cst1, long cst2);		///< constructs a new constraint which involves the variable in var and which checks whether the sum of the values of the variables in var in the interval [cst1, cst2]
		In_Sum_Global_Constraint (In_Sum_Global_Constraint & c);	              	  ///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 		 ///< returns a pointer on a copy of the constraint
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    string Get_Relation () override;         ///< returns the relation used to compare the sum and the constant
    string Get_XCSP3_Expression() override;  ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
    long Get_Constant2 ();										 ///< returns the second constant to which the sum is compared
    
  private:
    bool Compare (long sum);                     ///< returns true if the sum satisfies the constraint, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * In_Sum_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new In_Sum_Global_Constraint (*this);
}


inline string In_Sum_Global_Constraint::Get_Relation ()
// returns the relation used to compare the sum and the constant
{
  return "in";
}


inline bool In_Sum_Global_Constraint::Compare (long sum)
// returns true if the sum satisfies the constraint, false otherwise
{
  return (constant <= sum) && (sum <= constant2);
}


inline long In_Sum_Global_Constraint::Get_Constant2 ()
// returns the second constant to which the sum is compared
{
  return constant2;
}

#endif
