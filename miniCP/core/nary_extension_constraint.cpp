#include "nary_extension_constraint.h"


// ---------------------------
// constructor and destructor
// ---------------------------


Nary_Extension_Constraint::Nary_Extension_Constraint (vector<Variable *> & var): Extension_Constraint (var)
// constructs an extension constraint whose scope is defined by the variables in var
{
	relation = new AVL (var);
}


Nary_Extension_Constraint::~Nary_Extension_Constraint()
// destructor
{
	delete relation;
}


