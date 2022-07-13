/// \file 
/// \brief Definitions related to the Binary_Extension_Constraint class

#ifndef BINARY_EXTENSION_CONSTRAINT_H
#define BINARY_EXTENSION_CONSTRAINT_H

#include "extension_constraint.h"

class Binary_Extension_Constraint: public Extension_Constraint  /// This class implements the binary constraints in extension with boolean matrix \ingroup core
{
	protected:
		bool ** relation;		///< the relation related to the constraint
		
	public:
		// constructors and destructor
		Binary_Extension_Constraint (vector<Variable *> & var, bool default_value = false); 			///< constructs a binary extension new constraint whose all the tuples are forbidden if default_value is false, allowed otherwise
		Binary_Extension_Constraint (vector<Variable *> & var, bool positive, int ** tuple_list, int nb_tuples); 	///< constructs a new binary extension constraint whose all the tuples of tuple_list are allowed if positive is false, forbidden otherwise
		Binary_Extension_Constraint (Binary_Extension_Constraint & c);														///< constructs a new constraint by copying the constraint c
		~Binary_Extension_Constraint ();																								 					///< destructor
		
		// operator
		Binary_Extension_Constraint & operator= (Binary_Extension_Constraint & c);		///< operator =
		
		// basic functions
		Constraint * Duplicate () override; 						///< returns a pointer on a copy of the constraint
		void Merge (Binary_Extension_Constraint * c);		///< merges the current constraint with the constraint c
		void Allow_Tuple (int * t) override; 		  			///< allows the tuple t
		void Allow_Tuple (int val1, int val2); 	  	 		///< allows the tuple t
		void Forbid_Tuple (int * t); 			 	 				 		///< forbids the tuple t
		void Forbid_Tuple (int val1, int val2);  		 		///< allows the tuple t
		bool Is_Satisfied (int * t) override;	 		 			///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Is_Satisfied (Tuple &t) override;				  ///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		void Get_Allowed_Tuple_List (list<int *> & list) override;	///< constructs the list of tuples allowed by the constraint
    string Get_XCSP3_Expression() override;         ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Binary_Extension_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Binary_Extension_Constraint (*this);
}


inline bool Binary_Extension_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return relation[t[0]][t[1]];
}


inline bool Binary_Extension_Constraint::Is_Satisfied (Tuple &t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return relation[t[0]][t[1]];
}

#endif
