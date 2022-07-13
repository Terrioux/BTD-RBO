#include "diff_and_greater_equal_sum_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Diff_And_Greater_Equal_Sum_Binary_Constraint::Diff_And_Greater_Equal_Sum_Binary_Constraint (vector<Variable *> & var, int cst): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and which checks whether the sum of the values of the variables in var is greater than or equal to the constant cst
{
	constant = cst;
	
	valx_to_valy[0] = new int [var[0]->Get_Domain()->Get_Initial_Size()];
	for (unsigned int i = 0; i < var[0]->Get_Domain()->Get_Initial_Size(); i++)
		valx_to_valy[0][i] = -1;
		
	valx_to_valy[1] = new int [var[1]->Get_Domain()->Get_Initial_Size()];
	for (unsigned int i = 0; i < var[1]->Get_Domain()->Get_Initial_Size(); i++)
	{
		unsigned int j = 0; 
		int real_value = var[1]->Get_Domain()->Get_Real_Value(i);
		while ((j  < var[0]->Get_Domain()->Get_Initial_Size()) &&  (real_value != var[0]->Get_Domain()->Get_Real_Value(j)))
			j++;
		
		if (j  < var[0]->Get_Domain()->Get_Initial_Size())
		{
			valx_to_valy[1][i] = j;
			valx_to_valy[0][j] = i;
		}
		else valx_to_valy[1][i] = -1;
	}

}


Diff_And_Greater_Equal_Sum_Binary_Constraint::Diff_And_Greater_Equal_Sum_Binary_Constraint (Diff_And_Greater_Equal_Sum_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


//-----------------
// basic functions
//-----------------


bool Diff_And_Greater_Equal_Sum_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	int val1 = scope_var[0]->Get_Domain()->Get_Real_Value (t[0]);
	int val2 = scope_var[1]->Get_Domain()->Get_Real_Value (t[1]);
	
	return  (val1 != val2) && (val1 + val2 >= constant);
}


bool Diff_And_Greater_Equal_Sum_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
	int x = Get_Position (var);
	int y = 1 - x;
	
	Domain * dy = scope_var [y]->Get_Domain();
	Domain * dx = scope_var [x]->Get_Domain();
	bool modif = false;
	
	int v;
	int w = dy->Get_Max();
	int wp = dy->Previous_Value(w);
	int y_max = dy->Get_Real_Value(w);
	
	v = dx->Get_Min();
	
	while ((v != -1) && (dx->Get_Real_Value(v) + y_max < constant))
	{
		ds->Add_Element (scope_var[x]);
		dx->Filter_Value (v);
		modif = true;
				
		v = dx->Next_Value(v);
	}

	v = valx_to_valy[y][w];
	if ((v != -1) && (dx->Is_Present(v)) && (dx->Get_Real_Value(v) + y_max >= constant) && ((wp == -1) || (dx->Get_Real_Value(v) + dy->Get_Real_Value(wp) < constant)))
	{
		ds->Add_Element (scope_var[x]);
		dx->Filter_Value (v);
		modif = true;
	}

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
	
  return modif;
}


string Diff_And_Greater_Equal_Sum_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> and(ne(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + "),ge(sum(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + ")," + to_string (constant) +")) </intension>";
}
