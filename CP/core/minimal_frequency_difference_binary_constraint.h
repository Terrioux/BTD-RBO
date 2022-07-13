/// \file 
/// \brief Definitions related to the Minimal_Frequency_Difference_Binary_Constraint class

#ifndef MINIMAL_FREQUENCY_DIFFERENCE_BINARY_CONSTRAINT_H
#define MINIMAL_FREQUENCY_DIFFERENCE_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Minimal_Frequency_Difference_Binary_Constraint: public Binary_Constraint    /// This class implements the binary constraint imposing  (x.y > 0 or ||x|-|y|| >= constant1) and (x.y < 0 or |x-y| >= constant2) \ingroup core
{
	int constant1;		///< the minimal difference between two frequencies (with absolute value)
	int constant2;		///< the minimal difference between two frequencies
	int min;					///< the minimal value between constant1 and constant2
	int max;					///< the maximal value between constant1 and constant2
	
	public:
		// constructors and destructor
		Minimal_Frequency_Difference_Binary_Constraint (vector<Variable *> & var, int cst1, int cst2);				///< constructs a new constraint which involves the variable in var and whose relation imposes that the product of the two frequencies is positive/negative or their difference (absolute value) is at least equal to cst1 and cst2
		Minimal_Frequency_Difference_Binary_Constraint (Minimal_Frequency_Difference_Binary_Constraint & c);	///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	  		 	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Minimal_Frequency_Difference_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Minimal_Frequency_Difference_Binary_Constraint (*this);
}

#endif

