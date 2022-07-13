#include "negative_minimal_frequency_difference_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Negative_Minimal_Frequency_Difference_Binary_Constraint::Negative_Minimal_Frequency_Difference_Binary_Constraint (vector<Variable *> & var, int cst): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that the product of the two frequencies is negative or their difference (absolute value) is at least equal to cst
{
	constant = cst;
}


Negative_Minimal_Frequency_Difference_Binary_Constraint::Negative_Minimal_Frequency_Difference_Binary_Constraint (Negative_Minimal_Frequency_Difference_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	constant = c.constant;
}


//-----------------
// basic functions
//-----------------


bool Negative_Minimal_Frequency_Difference_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	if (scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) == 0)
		return abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1])) >= constant;
	else
		if (scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) > 0)
			return (scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) < 0) || (abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) - scope_var[0]->Get_Domain()->Get_Real_Value(t[0])) >= constant);
		else return (scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) > 0) || (abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) - scope_var[0]->Get_Domain()->Get_Real_Value(t[0])) >= constant);
}


bool Negative_Minimal_Frequency_Difference_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	int y = 1 - x;
	Domain * dx = scope_var[x]->Get_Domain();
	Domain * dy = scope_var[y]->Get_Domain();
	
	bool modif = false;

	int v = dx->Get_Min();
	int real_v;
	int real_y_min = dy->Get_Real_Min();
	int real_y_max = dy->Get_Real_Max();
	int w;
	
	while (v != -1)
	{
		real_v = dx->Get_Real_Value (v);
		
		if (real_v == 0)
		{
			w = dy->Get_Min();
			while ((w != -1) && (abs(dy->Get_Real_Value(w)) < constant))
				w = dy->Next_Value(w);
			
			if (w == -1)
			{
				ds->Add_Element (scope_var[x]);
				dx->Filter_Value (v);
				modif = true;
			}
		}
		else
			if (real_v > 0)
			{
				if (real_y_min >= 0)
				{
					w = dy->Get_Min();
					while ((w != -1) && (abs(dy->Get_Real_Value(w) - real_v) < constant))
						w = dy->Next_Value(w);
					
					if (w == -1)
					{
						ds->Add_Element (scope_var[x]);
						dx->Filter_Value (v);
						modif = true;
					}
				}
			}
			else
				{
					if (real_y_max <= 0)
					{
						w = dy->Get_Min();
						while ((w != -1) && (abs(dy->Get_Real_Value(w) - real_v) < constant))
							w = dy->Next_Value(w);
						
						if (w == -1)
						{
							ds->Add_Element (scope_var[x]);
							dx->Filter_Value (v);
							modif = true;
						}
					}
				}
				
		v = dx->Next_Value(v);
	}
  
  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);

	return modif;
}


string Negative_Minimal_Frequency_Difference_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> or(lt(mult(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + "),0),ge(dist(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + ")," + to_string(constant) + ") </intension>";
}
