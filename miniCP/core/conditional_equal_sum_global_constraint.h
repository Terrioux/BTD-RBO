/// \file 
/// \brief Definitions related to the Conditional_Equal_Sum_Global_Constraint class

#ifndef CONDITIONAL_EQUAL_SUM_GLOBAL_CONSTRAINT_H
#define CONDITIONAL_EQUAL_SUM_GLOBAL_CONSTRAINT_H

#include "sum_global_constraint.h"


class Conditional_Equal_Sum_Global_Constraint: public Global_Constraint   /// This class implements the constraint imposing  that the conditional sum is equal to the given constant \ingroup core
{
	protected:
		int * constant_var;     ///< the constant to which each condition refers to 
		int constant;           ///< the constant to which the conditional sum must be equal 

	public:
		// constructors and destructor
		Conditional_Equal_Sum_Global_Constraint (vector<Variable *> & var, int cst, int cst_var []);	///< constructs a new constraint which involves the variable in var and which checks whether the conditional sum of the values of the variables in var is equal to the constant cst
		Conditional_Equal_Sum_Global_Constraint (Conditional_Equal_Sum_Global_Constraint & c);	    	///< constructs a new constraint by copying the constraint c 
		~Conditional_Equal_Sum_Global_Constraint ();													 			   								///< destructor
		
		// basic functions
		Constraint * Duplicate () override;				  	///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		  	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Conditional_Equal_Sum_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Conditional_Equal_Sum_Global_Constraint (*this);
}

#endif
