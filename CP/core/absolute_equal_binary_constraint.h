/// \file 
/// \brief Definitions related to the Absolute_Equal_Binary_Constraint class

#ifndef ABSOLUTE_EQUAL_BINARY_CONSTRAINT_H
#define ABSOLUTE_EQUAL_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Absolute_Equal_Binary_Constraint: public Binary_Constraint   /// This class implements the binary constraint imposing |x| = |y| \ingroup core
{
	pair<int,int*> ** equal;	    ///< the correspondence between equal value
	
	public:
		// constructors and destructor
		Absolute_Equal_Binary_Constraint (vector<Variable *> & var);											///< constructs a new constraint which involves the variable in var and whose relation imposes that the two variables have the same value in absolute value
		Absolute_Equal_Binary_Constraint (Absolute_Equal_Binary_Constraint & c);	///< constructs a new constraint by copying the constraint c
		~Absolute_Equal_Binary_Constraint ();													 			   					///< destructor
		
		// basic functions
		Constraint * Duplicate () override; 				  ///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 	      ///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	  ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Absolute_Equal_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Absolute_Equal_Binary_Constraint (*this);
}

#endif

