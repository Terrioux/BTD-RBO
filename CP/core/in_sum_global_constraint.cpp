#include "in_sum_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


In_Sum_Global_Constraint::In_Sum_Global_Constraint (vector<Variable *> & var, long cst1, long cst2): Sum_Global_Constraint (var,cst1)
// constructs a new constraint which involves the variable in var and which checks whether the sum of the values of the variables in var in the interval [cst1, cst2]
{	
  constant2 = cst2;
}


In_Sum_Global_Constraint::In_Sum_Global_Constraint (In_Sum_Global_Constraint & c): Sum_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  constant2 = c.constant2;
}


//-----------------
// basic functions
//-----------------


bool In_Sum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
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
			sum_min += dy->Get_Real_Min();
			sum_max += dy->Get_Real_Max();
		}
	
	bool modif = false;
	int v;

	v = dx->Get_Min();
	
	while ((v != -1) && (sum_max + dx->Get_Real_Value(v) < constant))
	{
		ds->Add_Element (scope_var[x]);
		dx->Filter_Value (v);
		modif = true;
		
		v = dx->Next_Value(v);
	}

  if (dx->Get_Size() == 0)
    v = -1;
  else v = dx->Get_Max();
	
	while ((v != -1) && (sum_min + dx->Get_Real_Value(v) > constant2))
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


string In_Sum_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<sum>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <condition> (in," + to_string(constant) + ".." + to_string(constant2) + ") </condition>\n";
  expr += "</sum>";
  return expr;
}
