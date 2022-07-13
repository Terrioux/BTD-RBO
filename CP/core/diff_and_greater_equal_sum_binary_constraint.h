/// \file 
/// \brief Definitions related to the Diff_And_Greater_Equal_Sum_Binary_Constraint class

#ifndef DIFF_AND_GREATER_EQUAL_SUM_BINARY_CONSTRAINT_H
#define DIFF_AND_GREATER_EQUAL_SUM_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Diff_And_Greater_Equal_Sum_Binary_Constraint: public Binary_Constraint   /// This class implements the binary constraint imposing  x != y and x + y >=  constant \ingroup core
{
	protected:
		int * valx_to_valy[2];		///< associates a value from the first variable to a value from the second variable such that the values are equal and conversely
		int constant;             ///< the constant to which the sum is compared with
	
	public:
		// constructors and destructor
		Diff_And_Greater_Equal_Sum_Binary_Constraint (vector<Variable *> & var, int cst);						///< constructs a new constraint which involves the variable in var and which checks whether the sum of the values of the variables in var is greater than or equal to the constant cst
		Diff_And_Greater_Equal_Sum_Binary_Constraint (Diff_And_Greater_Equal_Sum_Binary_Constraint & c);	///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Diff_And_Greater_Equal_Sum_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Diff_And_Greater_Equal_Sum_Binary_Constraint (*this);
}

#endif
