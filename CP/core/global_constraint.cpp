#include "global_constraint.h"

//--------------
// constructors
//--------------


Global_Constraint::Global_Constraint (): Constraint ()
// constructs a new global constraint which initially involves no variable
{
}

Global_Constraint::Global_Constraint (vector<Variable *> & var): Constraint (var)
// constructs a new global constraint with number n which involves the variable in var
{
}
		
		
Global_Constraint::Global_Constraint (Global_Constraint & c): Constraint (c)
// constructs a new constraint by copying the global constraint c for the CSP pb (we assume that the variables in the scope of c have the same number in the CSP pb)
{
	num = c.num;
	arity = c.arity;
	scope = c.scope;
	scope_var = c.scope_var;
		
	unassigned_variable_number = c.unassigned_variable_number;
	position = c.position;
}


