/// \file 
/// \brief Definitions related to the Constant_Index_Element_Global_Constraint class

#ifndef CONSTANT_INDEX_ELEMENT_GLOBAL_CONSTRAINT_H
#define CONSTANT_INDEX_ELEMENT_GLOBAL_CONSTRAINT_H


#include "global_constraint.h"


class Constant_Index_Element_Global_Constraint: public Global_Constraint      /// This class implements the element global constraint with constant value and variable index (which is the last variable of the scope) \ingroup core
{
  protected:
    int value;            ///< the constant value to which the constraint refers
    int * list_value;     ///< the number of the value corresponding to the constant value, for each variable of the scope
    
	public:
		// constructors and destructor
		Constant_Index_Element_Global_Constraint (vector<Variable *> & var, int val);               ///< constructs a new element constraint with constant val which involves the variables in var (the first variable in var represents the index variable)
		Constant_Index_Element_Global_Constraint (Constant_Index_Element_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
    ~Constant_Index_Element_Global_Constraint ();       ///< destructor
				
		// basic functions
    Constraint * Duplicate () override;  				///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	                  ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	  ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;     ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Constant_Index_Element_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Constant_Index_Element_Global_Constraint (*this);
}


inline bool Constant_Index_Element_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  if (event_manager->Exist_Event_Fix (scope_var[arity-1]->Get_Num(),ref))
    return true;
  
  for (unsigned int i = 0; i < arity-1; i++)
    if (event_manager->Exist_Event_Dmc (scope_var[i]->Get_Num(),list_value[i],ref))
      return true;
  
  return false;
}

#endif
