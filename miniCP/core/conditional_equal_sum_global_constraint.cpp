#include "conditional_equal_sum_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Conditional_Equal_Sum_Global_Constraint::Conditional_Equal_Sum_Global_Constraint (vector<Variable *> & var, int cst, int cst_var []): Global_Constraint (var)
// constructs a new constraint which involves the variable in var and which checks whether the conditional sum of the values of the variables in var is equal to the constant cst
{	
	constant = cst;
	constant_var = new int[arity];
	for (unsigned int i = 0; i < arity; i++)
		constant_var[i] = cst_var[i];
}


Conditional_Equal_Sum_Global_Constraint::Conditional_Equal_Sum_Global_Constraint (Conditional_Equal_Sum_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	constant = c.constant;
	constant_var = new int[arity];
	for (unsigned int i = 0; i < arity; i++)
		constant_var[i] = c.constant_var[i];	
}


Conditional_Equal_Sum_Global_Constraint::~Conditional_Equal_Sum_Global_Constraint ()
// destructor
{
	delete [] constant_var;
}


//-----------------
// basic functions
//-----------------


bool Conditional_Equal_Sum_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	int sum = 0;
	for (unsigned int i = 0; i < arity; i++)
		if (scope_var[i]->Get_Domain()->Get_Real_Value (t[i]) >= constant_var[i])
			sum++;

	return sum == constant;
}


bool Conditional_Equal_Sum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
	
	int nb_positive = 0;
	int nb_negative = 0;
	
	bool modif = false;
	
	for (unsigned int i = 0; i < arity; i++)
		if (i != x)
		{
			if (scope_var[i]->Get_Domain()->Get_Real_Min() >= constant_var[i])
				nb_positive++;
			else
				if (scope_var[i]->Get_Domain()->Get_Real_Max() < constant_var[i])
					nb_negative++;
		}
		

	if ((nb_positive > constant) || (nb_negative > (signed) arity - constant))
	{
		// we remove all the values
		int v = dx->Get_Min();
		while (v != -1)
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			
			v = dx->Next_Value(v);
		}

		modif = true;
	}
	else
		if (nb_positive == constant)
		{
			// we remove all the values satisfying the condition
			int v = dx->Get_Max();
			while ((v != -1) && (dx->Get_Real_Value(v) >= constant_var[x]))
			{
				ds->Add_Element (scope_var[x]);
				dx->Filter_Value (v);
				modif = true;
				
				v = dx->Previous_Value(v);
			}		
		}
		else
				if (nb_negative == (signed) arity - constant)
				{
					// we remove all the values not satisfying the condition
					int v = dx->Get_Min();
					while ((v != -1) && (dx->Get_Real_Value(v) < constant_var[x]))
					{
						ds->Add_Element (scope_var[x]);
						dx->Filter_Value (v);
						modif = true;
						
						v = dx->Next_Value(v);
					}
				}
  
  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
  
  return modif;
}


string Conditional_Equal_Sum_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<sum>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  "ge("+ scope_var[i]->Get_Name() + "," + to_string (constant_var[i]) + ") ";
  expr += "</list>\n";
  expr += "  <condition> (eq," + to_string(constant) + ") </condition>\n";
  expr += "</sum>";
  return expr;
}
