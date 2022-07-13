#include "minimal_frequency_difference_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Minimal_Frequency_Difference_Binary_Constraint::Minimal_Frequency_Difference_Binary_Constraint (vector<Variable *> & var, int cst1, int cst2): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that the product of the two frequencies is positive/negative or their difference (absolute value) is at least equal to cst1 and cst2
{
	constant1 = cst1;
	constant2 = cst2;
	
	if (constant1 > constant2)
	{
		min = constant2;
		max = constant1;
	}
	else 
		{
			min = constant1;
			max = constant2;
		}
}


Minimal_Frequency_Difference_Binary_Constraint::Minimal_Frequency_Difference_Binary_Constraint (Minimal_Frequency_Difference_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	constant1 = c.constant1;
	constant2 = c.constant2;
	min = c.min;
	max = c.max;
}


//-----------------
// basic functions
//-----------------


bool Minimal_Frequency_Difference_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	if (scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) == 0)
		return abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1])) >= max;
	else
		if (scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) > 0)
		{
			if (scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) > 0)
				return abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) - scope_var[0]->Get_Domain()->Get_Real_Value(t[0])) >= constant2;
			else return abs(abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1])) - scope_var[0]->Get_Domain()->Get_Real_Value(t[0])) >= constant1;
		}
		else 
			if (scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) > 0)
				return abs(abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1])) - abs(scope_var[0]->Get_Domain()->Get_Real_Value(t[0]))) >= constant1;
			else return abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) - scope_var[0]->Get_Domain()->Get_Real_Value(t[0])) >= constant2;
}


bool Minimal_Frequency_Difference_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	int y = 1 - x;
	Domain * dx = scope_var[x]->Get_Domain();
	Domain * dy = scope_var[y]->Get_Domain();
	
	bool modif = false;    // true if a deletion is achieved, false otherwise

	int v = dx->Get_Min();
	int real_v;
	int w;
	int real_w;
	
  int * ts;
  int t[2];
  bool has_no_support;
  bool record_support;
  
	
	do
	{
		ts = ls->Get_Support (num,x,v);
		
		if (ts == 0) 
		{
			w = dy->Get_Min();
			has_no_support = true;
			record_support = true;
		}
		else
			{
				if (dy->Is_Present(ts[y]))
				{
					has_no_support = false;
					record_support = false;
				}
				else 
					{
						has_no_support = true;
						record_support = true;
						
						if (ls->Continue_From_Scratch())
							w = dy->Get_Min();
						else w = dy->Next_Value(ts[y]);
					}
			}
		
		if ((has_no_support) && (w != -1))
		{
			real_v = dx->Get_Real_Value(v);
			
			if (real_v > 0)
			{
				while ((w != -1) && ((real_w = dy->Get_Real_Value(w)) < 0) && (abs(abs(real_w) - real_v) < constant1))
					w = dy->Next_Value(w);
				
				if (w != -1)
				{
					if (real_w < 0)
						has_no_support = false;
					else
						if (real_w == 0)
						{
							if (real_v > max)
								has_no_support = false;
							else w = dy->Next_Value(w);
						}
						
					if (has_no_support)
					{
						while ((w != -1) && (abs(dy->Get_Real_Value(w) - real_v) < constant2))
							w = dy->Next_Value(w);
						
						has_no_support = w == -1;
					}
				}
			}
			else
				if (real_v == 0)
				{
					while ((w != -1) && (abs(dy->Get_Real_Value(w)) < max))
						w = dy->Next_Value(w);
				}
				else
					{
						while ((w != -1) && ((real_w = dy->Get_Real_Value(w)) < 0) && (abs(real_w - real_v) < constant2))
							w = dy->Next_Value(w);
						
						if (w != -1)
						{
							if (real_w < 0)
								has_no_support = false;
							else
								if (real_w == 0)
								{
									if (real_v > max)
										has_no_support = false;
									else w = dy->Next_Value(w);
								}
								
							if (has_no_support)
							{
								while ((w != -1) && (abs(abs(dy->Get_Real_Value(w)) + real_v) < constant1))
									w = dy->Next_Value(w);
								
								has_no_support = w == -1;
							}
						}
					}
		}
		
		if (has_no_support)
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			modif = true;
		}
		else 
			if (record_support)
			{
				t[x] = v;
				t[y] = w;
				ds->Add_Support (num,x,v,arity,t);
				ls->Change_Support (num,x,v,t,arity);
			}
		
		v = dx->Next_Value(v);		
	}
	while (v != -1);
  
  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
	
	return modif;
}


string Minimal_Frequency_Difference_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> and(or(gt(mul(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + "),0),ge(dist(abs(" + scope_var[0]->Get_Name() + "),abs(" + scope_var[1]->Get_Name() + "))," 
        + to_string(constant1) + ")),or(lt(mul(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + "),0),ge(dist(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + "),"
        + to_string(constant2) + "))) </intension>";
}
