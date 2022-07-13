/// \file 
/// \brief Definitions related to the Extension_Constraint class

#ifndef EXTENSION_CONSTRAINT_H
#define EXTENSION_CONSTRAINT_H

#include "constraint.h"
#include "assignment.h"

class Extension_Constraint : public Constraint    /// This class allows to represent constraints in extension \ingroup core
{		
  protected:
		double tightness;													///< the tighness of the constraint (i.e. the number of forbidden tuples)
		double allowed_tuple_number;							///< the number of allowed tuples

  
	public:			
		// constructor
		Extension_Constraint (vector<Variable *> & var);				///< constructs an extension constraint whose scope is defined by the variables in var
		Extension_Constraint (Extension_Constraint & c);				///< constructs a new constraint by copying the constraint c

	
		// basic functions
		virtual double Get_Tightness ();   				   						///< returns the tightness of the constraint
		virtual double Get_Allowed_Tuple_Number ();	 						///< returns the number of allowed tuples
		virtual void Allow_Tuple (int * t)=0;   								///< allows the tuple t
		virtual void Forbid_Tuple (int * t)=0;  								///< forbids the tuple t
};


inline double Extension_Constraint::Get_Tightness ()
// returns the tightness of the constraint
{
	return tightness;	
}


inline double Extension_Constraint::Get_Allowed_Tuple_Number ()
// returns the number of allowed tuples
{
	return allowed_tuple_number;	
}


inline bool Is_Valid (vector<Variable *> & var, int * values)
// returns true if the tuple is valid w.r.t. the domains of the variables in var
{
	bool ok = true;
	int i = 0;
	for (vector<Variable *>::iterator iter = var.begin(); (ok) && (iter != var.end()); iter++)
	{
		if (values[i] == -1)  ok = false;
		else
			if ((*iter)->Get_Domain()->Is_Present (values[i])) i++;
			else ok = false;
	}
	return ok;
}

#endif
