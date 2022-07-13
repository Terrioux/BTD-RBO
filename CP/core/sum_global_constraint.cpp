#include "sum_global_constraint.h"


//--------------
// constructors 
//--------------


Sum_Global_Constraint::Sum_Global_Constraint (vector<Variable *> & var, int cst): Global_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var to the constant cst with respect to comp
{	
	constant = cst;
}


Sum_Global_Constraint::Sum_Global_Constraint (Sum_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	constant = c.constant;
}



//-----------------
// basic functions
//-----------------


bool Sum_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	int sum = 0;
	for (unsigned int i = 0; i < arity; i++)
		sum += scope_var[i]->Get_Domain()->Get_Real_Value (t[i]);

	return Compare(sum);
}


string Sum_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<sum>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <condition> (" + Get_Relation () + "," + to_string(constant) + ") </condition>\n";
  expr += "</sum>";
  return expr;
}
