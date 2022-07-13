/// \file 
/// \brief Definitions related to the Nogood_Constraint class

#ifndef NOGOOD_CONSTRAINT_H
#define NOGOOD_CONSTRAINT_H

#include "constraint.h"
#include "assignment.h"

class Nogood_Constraint: public Constraint      /// This class implements the nogood constraint \ingroup core
{
	protected:
		int * forbidden_tuple;			///< the tuple forbidden by the nogood constraint
	
	public:
		// constructors and destructor
		Nogood_Constraint (vector<Variable *> & var, int * ft, int n = 0); 	///< constructs a new constraint with number n which involves the variable in var whose all the tuples are allowed except the tuple ft
		Nogood_Constraint (Nogood_Constraint & c);		 	///< constructs a new constraint by copying the constraint c 
		~Nogood_Constraint();				///< destructor
		
		
		// operator
		Nogood_Constraint & operator= (Nogood_Constraint & c);		///< operator =
		
		
		// basic functions
		Constraint * Duplicate () override; 										///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 					  	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		void Get_Allowed_Tuple_List (list<int *> & list) override;  ///< constructs the list of tuples allowed by the constraint
		int * Get_Forbidden_Tuple ();												  	///< returns the tuple forbidden by the constraint
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Nogood_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Nogood_Constraint (*this);
}


inline int * Nogood_Constraint::Get_Forbidden_Tuple ()
// returns the tuple forbidden by the constraint
{
	return forbidden_tuple;
}

#endif
