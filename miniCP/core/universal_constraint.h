/// \file 
/// \brief Definitions related to the Universal_Constraint class

#ifndef UNIVERSAL_CONSTRAINT_H
#define UNIVERSAL_CONSTRAINT_H

#include "constraint.h"
#include "assignment.h"


class Universal_Constraint: public Constraint     /// This class implements the universal constraint \ingroup core
{
	public:
		// constructors
		Universal_Constraint (vector<Variable *> & var); 		///< constructs a new constraint which involves the variable in var whose all the tuples are allowed
		Universal_Constraint (Universal_Constraint & c);	 	///< constructs a new constraint by copying the constraint c
		
		// operator
		Universal_Constraint & operator= (Universal_Constraint & c);		///< operator =
		
		// basic functions
		Constraint * Duplicate () override; 										///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 					  	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Universal_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Universal_Constraint (*this);
}


inline bool Universal_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return true;
}


inline bool Universal_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
	return false;
}


#endif
