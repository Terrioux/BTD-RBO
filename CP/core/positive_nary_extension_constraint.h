/// \file 
/// \brief Definitions related to the Positive_Nary_Extension_Constraint class

#ifndef POSITIVE_NARY_EXTENSION_CONSTRAINT_H
#define POSITIVE_NARY_EXTENSION_CONSTRAINT_H

#include "nary_extension_constraint.h"


class Positive_Nary_Extension_Constraint: public Nary_Extension_Constraint      /// This class implements n-ary constraints in extension with list of allowed tuples \ingroup core
{
	public:
		// constructors 
		Positive_Nary_Extension_Constraint (vector<Variable *> & var);      			 	 ///< constructs a new constraint which involves the variable in var whose all the tuples are forbidden
		Positive_Nary_Extension_Constraint (vector<Variable *> & var, int ** tuples, int nb_tuples); ///< constructs a new constraint which involves the variable in var whose the nb_tuples tuples of tuples A are allowed
		Positive_Nary_Extension_Constraint (Positive_Nary_Extension_Constraint & c); ///< constructs a new constraint by copying the positive constraint c
		
		
		// operator
		Positive_Nary_Extension_Constraint & operator= (Positive_Nary_Extension_Constraint & c);		///< operator =
		
		
		// basic functions
		Constraint * Duplicate () override;					        	///< returns a pointer on a copy of the constraint
		void Merge (Positive_Nary_Extension_Constraint * c);	///< merges the current constraint with the constraint c
		void Allow_Tuple (int * t) override; 		  			 	 		///< allows the tuple t
		void Forbid_Tuple (int * t) override;								  ///< forbids the tuple t
		bool Is_Satisfied (int * t) override;	 		 						///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		void Get_Allowed_Tuple_List (list<int *> & list) override;	 ///< constructs the list of tuples allowed by the constraint
    string Get_XCSP3_Expression() override;               ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Positive_Nary_Extension_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Positive_Nary_Extension_Constraint (*this);
}


inline bool Positive_Nary_Extension_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return relation->Is_Present (t);
}

#endif
