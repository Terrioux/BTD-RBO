/// \file 
/// \brief Definitions related to the Positive_Minimal_Frequency_Difference_Binary_Constraint class

#ifndef POSITIVE_MINIMAL_FREQUENCY_DIFFERENCE_BINARY_CONSTRAINT_H
#define POSITIVE_MINIMAL_FREQUENCY_DIFFERENCE_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Positive_Minimal_Frequency_Difference_Binary_Constraint: public Binary_Constraint     /// This class implements the binary constraint imposing  x.y > 0 or ||x| - |y|| >= constant \ingroup core
{
	int constant;		///< the minimal difference between two frequencies
	
	public:
		// constructors and destructor
		Positive_Minimal_Frequency_Difference_Binary_Constraint (vector<Variable *> & var, int cst);				  									///< constructs a new constraint which involves the variable in var and whose relation imposes that the product of the two frequencies is positive or their difference (absolute value) is at least equal to cst
		Positive_Minimal_Frequency_Difference_Binary_Constraint (Positive_Minimal_Frequency_Difference_Binary_Constraint & c);	///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		  	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		int Get_Constant ();												  ///< returns the constant
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Positive_Minimal_Frequency_Difference_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Positive_Minimal_Frequency_Difference_Binary_Constraint (*this);
}


inline int Positive_Minimal_Frequency_Difference_Binary_Constraint::Get_Constant ()
// returns the constant
{
	return constant;
}
#endif

