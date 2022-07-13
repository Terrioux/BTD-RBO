#include "extension_constraint.h"


// ------------
// constructor 
// ------------


Extension_Constraint::Extension_Constraint (vector<Variable *> & var): Constraint (var)
// constructs an extension constraint whose scope is defined by the variables in var
{
	allowed_tuple_number = -1;
	tightness = -1;
}


Extension_Constraint::Extension_Constraint (Extension_Constraint & c): Constraint (c)
// constructs a new constraint by copying the constraint c
{
 	allowed_tuple_number = c.allowed_tuple_number;
	tightness = c.tightness;
}
