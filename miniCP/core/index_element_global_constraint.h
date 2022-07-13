/// \file 
/// \brief Definitions related to the Index_Element_Global_Constraint class

#ifndef INDEX_ELEMENT_GLOBAL_CONSTRAINT_H
#define INDEX_ELEMENT_GLOBAL_CONSTRAINT_H


#include "global_constraint.h"


class Index_Element_Global_Constraint: public Global_Constraint      /// This class implements the element global constraint with variable index (which is the penultimate variable in the scope) and variable value (which is the last variable of the scope) \ingroup core
{
  protected:
    int ** list_value;     ///< the number of the value corresponding to the variable value, for each list variable of the scope and each value of the variable value
    
	public:
		// constructors and destructor
		Index_Element_Global_Constraint (vector<Variable *> & var);         		 	///< constructs a new element constraint with the first variable in var represents the index variable and the last variable in var as the variable value, and which involves the variable in var
		Index_Element_Global_Constraint (Index_Element_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		~Index_Element_Global_Constraint ();																			///< destructor
				
		// basic functions
    Constraint * Duplicate () override;		  		  ///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 	  	 	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	                   ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Index_Element_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint w.r.t. the CSP pb
{
	return new Index_Element_Global_Constraint (*this);
}


inline bool Index_Element_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  if ((event_manager->Exist_Event_Fix (scope_var[arity-2]->Get_Num(),ref)) || (event_manager->Exist_Event_Fix (scope_var[arity-1]->Get_Num(),ref)) || (ref == 0))
    return true;
  else
    {
      for (unsigned int i = 0; i < arity-2; i++)
        if (event_manager->Exist_Event_Fix (scope_var[arity-2]->Get_Num(),ref))
          return true;

      Domain * d_value = scope_var[arity-1]->Get_Domain();

      for (unsigned int j = 0; j < d_value->Get_Initial_Size(); j++)
      {
        int v = d_value->Get_Remaining_Value(j);
        if (list_value[v] != 0)
        {
          for (unsigned int i = 0; i < arity-2; i++)
            if ((list_value[v][i] != -1) && (event_manager->Exist_Event_Dmc (scope_var[i]->Get_Num(),list_value[v][i],ref)))
              return true;
        }
      }
    }
  
  return false;  
}
#endif

