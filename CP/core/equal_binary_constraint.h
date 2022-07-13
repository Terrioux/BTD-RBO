/// \file 
/// \brief Definitions related to the Equal_Binary_Constraint class

#ifndef EQUAL_BINARY_CONSTRAINT_H
#define EQUAL_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Equal_Binary_Constraint: public Binary_Constraint   /// This class implements the constraint y = a.x + b  where x and y are variables and a and b two integers  \ingroup core
{
	protected:
    int constant_a;         ///< the constant a
    int constant_b;         ///< the constant b
		int ** equal_value;     ///< maps to each variable i and value j the value k for each of the other variables s.t. j and k correspond to the same value
	
	public:
		// constructors and destructor
		Equal_Binary_Constraint (vector<Variable *> & var, int a, int b);		///< constructs a new constraint which involves the variable in var and whose relation imposes y = a.x + b
		Equal_Binary_Constraint (Equal_Binary_Constraint & c);	          	///< constructs a new constraint by copying the constraint c
		~Equal_Binary_Constraint ();													 			   		  ///< destructor
		
		// basic functions
		Constraint * Duplicate () override;				  	///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
 		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Equal_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Equal_Binary_Constraint (*this);
}


inline bool Equal_Binary_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  return unassigned_variable_number >= arity-1;
}

#endif
