/// \file 
/// \brief Definitions related to the Element_Global_Constraint class

#ifndef ELEMENT_GLOBAL_CONSTRAINT_H
#define ELEMENT_GLOBAL_CONSTRAINT_H


#include "global_constraint.h"


class Element_Global_Constraint: public Global_Constraint      /// This class implements the element global constraint with variable value (which is the last variable of the scope) \ingroup core
{
  protected:
    int ** list_value;      ///< the number of the value corresponding to the variable value, for each list variable of the scope and each value of the variable value
    int * watch1;           ///< the first watched variable for each value of the variable value
    int * watch2;           ///< the second watched variable for each value of the variable value
    
	public:
		// constructors and destructor
		Element_Global_Constraint (vector<Variable *> & var);         ///< constructs a new element constraint with the last variable in var as the variable value and which involves the variable in var
		Element_Global_Constraint (Element_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		~Element_Global_Constraint ();																///< destructor
				
		// basic functions
    Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 	  ///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	                 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Element_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Element_Global_Constraint (*this);
}


inline bool Element_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  if ((event_manager->Exist_Event_Fix (scope_var[arity-1]->Get_Num(),ref)) || (ref == 0))
    return true;
  else
    {
      Domain * d;
      d = scope_var[arity-1]->Get_Domain();
      if (d->Get_Size() == 1)
      {
        int v = d->Get_Remaining_Value(0);
        return (watch1[v] == -1) || (watch2[v] == -1) || (event_manager->Exist_Event_Dmc (scope_var[watch1[v]]->Get_Num(),list_value[v][watch1[v]],ref)) || (event_manager->Exist_Event_Dmc (scope_var[watch2[v]]->Get_Num(),list_value[v][watch2[v]],ref));
      }
    }
  
  return false;
}

#endif
