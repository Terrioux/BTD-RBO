#include "weighted_sum_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Weighted_Sum_Global_Constraint::Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, int cst): Sum_Global_Constraint (var,cst)
// constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var weighted by weight from w to the constant cst with respect to comp
{	
	weights = new int [arity];
	for (unsigned int i = 0; i < arity; i++)
		weights[i] = w[i];
}


Weighted_Sum_Global_Constraint::Weighted_Sum_Global_Constraint (Weighted_Sum_Global_Constraint & c): Sum_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	weights = new int [arity];
	for (unsigned int i = 0; i < arity; i++)
		weights[i] = c.weights[i];
}


Weighted_Sum_Global_Constraint::~Weighted_Sum_Global_Constraint ()
// destructor
{
	delete [] weights;
}


//-----------------
// basic functions
//-----------------


bool Weighted_Sum_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	int sum = 0;
	for (unsigned int i = 0; i < arity; i++)
		sum += scope_var[i]->Get_Domain()->Get_Real_Value (t[i]) * weights[i];

	return Compare(sum);
}


string Weighted_Sum_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<sum>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <coeffs> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  to_string(weights[i])+ " ";
  expr += "</coeffs>\n";
  expr += "  <condition> (" + Get_Relation () + "," + to_string(constant) + ") </condition>\n";
  expr += "</sum>";
  return expr;
}
