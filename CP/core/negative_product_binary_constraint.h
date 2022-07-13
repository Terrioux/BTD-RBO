/// \file 
/// \brief Definitions related to the Negative_Product_Binary_Constraint class

#ifndef NEGATIVE_PRODUCT_BINARY_CONSTRAINT_H
#define NEGATIVE_PRODUCT_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Negative_Product_Binary_Constraint: public Binary_Constraint     /// This class implements the binary constraint imposing  x.y <= 0 \ingroup core
{
	public:
		// constructors and destructor
		Negative_Product_Binary_Constraint (vector<Variable *> & var);										  		///< constructs a new constraint which involves the variable in var and whose relation imposes that the product of the two numbers is negative or zero
		Negative_Product_Binary_Constraint (Negative_Product_Binary_Constraint & c);		///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 	  	 	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Negative_Product_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Negative_Product_Binary_Constraint (*this);
}


inline bool Negative_Product_Binary_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  return (event_manager->Exist_Event_Lbc (scope_var[0]->Get_Num(),ref)) || (event_manager->Exist_Event_Ubc (scope_var[0]->Get_Num(),ref)) ||
         (event_manager->Exist_Event_Lbc (scope_var[1]->Get_Num(),ref)) || (event_manager->Exist_Event_Ubc (scope_var[1]->Get_Num(),ref)) || (ref == 0);
}

#endif

