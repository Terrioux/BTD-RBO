/// \file 
/// \brief Definitions related to the Absolute_Not_Equal_Binary_Constraint class

#ifndef ABSOLUTE_NOT_EQUAL_BINARY_CONSTRAINT_H
#define ABSOLUTE_NOT_EQUAL_BINARY_CONSTRAINT_H

#include "binary_constraint.h"


class Absolute_Not_Equal_Binary_Constraint: public Binary_Constraint     /// This class implements the binary constraint imposing |x| != |y| \ingroup core
{
	pair<int,int*> ** equal;	    ///< the correspondence between equal value
	
	public:
		// constructors and destructor
		Absolute_Not_Equal_Binary_Constraint (vector<Variable *> & var);												  ///< constructs a new constraint which involves the variable in var and whose relation imposes that the two variables have different values in absolute value
		Absolute_Not_Equal_Binary_Constraint (Absolute_Not_Equal_Binary_Constraint & c);	///< constructs a new constraint by copying the constraint c
		~Absolute_Not_Equal_Binary_Constraint ();													 			   							///< destructor
		
		// basic functions
		Constraint * Duplicate () override; 				  ///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t)  override;		   	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Absolute_Not_Equal_Binary_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Absolute_Not_Equal_Binary_Constraint (*this);
}


inline bool Absolute_Not_Equal_Binary_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  return (event_manager->Exist_Event_Fix (scope_var[0]->Get_Num(),ref)) || ((scope_var[0]->Get_Domain()->Get_Size() == 2) && (scope_var[0]->Get_Domain()->Get_Remaining_Real_Value(0) == - scope_var[0]->Get_Domain()->Get_Remaining_Real_Value(1))) ||
         (event_manager->Exist_Event_Fix (scope_var[1]->Get_Num(),ref)) || ((scope_var[1]->Get_Domain()->Get_Size() == 2) && (scope_var[1]->Get_Domain()->Get_Remaining_Real_Value(0) == - scope_var[1]->Get_Domain()->Get_Remaining_Real_Value(1)));
}


#endif

