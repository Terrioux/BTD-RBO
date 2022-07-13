/// \file 
/// \brief Definitions related to the Not_Equal_Absolute_Distance_Binary_Constraint class

#ifndef NOT_EQUAL_ABSOLUTE_DISTANCE_BINARY_CONSTRAINT_H
#define NOT_EQUAL_ABSOLUTE_DISTANCE_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Not_Equal_Absolute_Distance_Binary_Constraint: public Binary_Constraint   /// This class implements the binary constraint imposing ||x| - |y|| != constant \ingroup core
{
	int constant;							///< the constant to which the distance must be equal
	pair<int,int*> ** equal;	///< the correspondence between equal value
	
	public:
		// constructors and destructor
		Not_Equal_Absolute_Distance_Binary_Constraint (vector<Variable *> & var, int cst);															///< constructs a new constraint which involves the variable in var and whose relation imposes that the absolute distance between the two variables is equal to cst
		Not_Equal_Absolute_Distance_Binary_Constraint (Not_Equal_Absolute_Distance_Binary_Constraint & c);	///< constructs a new constraint by copying the constraint c
		~Not_Equal_Absolute_Distance_Binary_Constraint ();													 			   														///< destructor
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		  	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Not_Equal_Absolute_Distance_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Not_Equal_Absolute_Distance_Binary_Constraint (*this);
}

#endif

