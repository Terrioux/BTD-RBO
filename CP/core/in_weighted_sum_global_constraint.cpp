#include "in_weighted_sum_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


In_Weighted_Sum_Global_Constraint::In_Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, long cst1, long cst2): Weighted_Sum_Global_Constraint (var,w,cst1)
// constructs a new constraint which involves the variable in var and which checks whether the weighted sum (weighted by weight from w) of the values of the variables in var in the interval [cst1, cst2]
{	
  constant2 = cst2;
}


In_Weighted_Sum_Global_Constraint::In_Weighted_Sum_Global_Constraint (In_Weighted_Sum_Global_Constraint & c): Weighted_Sum_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  constant2 = c.constant2;
}


//-----------------
// basic functions
//-----------------


bool In_Weighted_Sum_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	long sum = 0;
	for (unsigned int i = 0; i < arity; i++)
		sum += scope_var[i]->Get_Domain()->Get_Real_Value (t[i]) * weights[i];

	return (constant <= sum) && (sum <= constant2);
}


bool In_Weighted_Sum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
	long sum_min = 0;
	long sum_max = 0;
  unsigned int x = Get_Position (var);
  Domain * dx = scope_var [x]->Get_Domain();
	Domain * dy;
  
	for (unsigned int i = 0; i < arity; i++)
		if (i != x)
		{
			dy = scope_var [i]->Get_Domain();
	
      if (weights[i] > 0)
      {
        sum_min += weights[i] * dy->Get_Real_Min();
        sum_max += weights[i] * dy->Get_Real_Max();
      }
      else
        {
          sum_min += weights[i] * dy->Get_Real_Max();
          sum_max += weights[i] * dy->Get_Real_Min();
        }
		}

	bool modif = false;
	int v;
	
	v = dx->Get_Min();
	
	while ((v != -1) && (sum_max + dx->Get_Real_Value(v) * weights[x] < constant))
	{
		ds->Add_Element (scope_var[x]);
		dx->Filter_Value (v);
		modif = true;
		
		v = dx->Next_Value(v);
	}

  if (dx->Get_Size() == 0)
  {
    pb->Set_Conflict (scope_var[x],this);
    return true;
  }

	v = dx->Get_Max();
	
	while ((v != -1) && (sum_min + dx->Get_Real_Value(v) * weights[x] > constant2))
	{
		ds->Add_Element (scope_var[x]);
		dx->Filter_Value (v);
			
		modif = true;

		v = dx->Previous_Value(v);
	}

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
	
  return modif;
}


string In_Weighted_Sum_Global_Constraint::Get_XCSP3_Expression()
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
  expr += "  <condition> (in," + to_string(constant) + ".." + to_string(constant2) + ") </condition>\n";
  expr += "</sum>";
  return expr;
}
