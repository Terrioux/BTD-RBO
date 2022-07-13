#include "universal_constraint.h"


//--------------
// constructors 
//--------------

Universal_Constraint::Universal_Constraint (vector<Variable *> & var): Constraint (var)
// constructs a new constraint with number n which involves the variable in var whose all the tuples are allowed
{
}


Universal_Constraint::Universal_Constraint (Universal_Constraint & c): Constraint (c)
// constructs a new constraint by copying the constraint c
{
}
