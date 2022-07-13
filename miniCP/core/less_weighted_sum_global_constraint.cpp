#include "less_weighted_sum_global_constraint.h"


//--------------
// constructors 
//--------------


Less_Weighted_Sum_Global_Constraint::Less_Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, bool is_strict, int cst): Weighted_Sum_Global_Constraint (var,w,cst)
// constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var weighted by weight from w to the constant cst with respect to < or <= if the boolean is_strict is true
{	
  strict_inequality = is_strict;
}


Less_Weighted_Sum_Global_Constraint::Less_Weighted_Sum_Global_Constraint (Less_Weighted_Sum_Global_Constraint & c): Weighted_Sum_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  strict_inequality = c.strict_inequality;
}


//-----------------
// basic functions
//-----------------


bool Less_Weighted_Sum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
	int sum = 0;
	Domain * dy;

	for (unsigned int i = 0; i < arity; i++)
		if (scope_var[i]->Get_Num() != var)
		{
			dy = scope_var [i]->Get_Domain();
			if (weights[i] > 0)
				sum += dy->Get_Real_Min() * weights[i];
			else sum += dy->Get_Real_Max() * weights[i];
		}

  int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	bool modif = false;
	
	if (weights[x] > 0)
	{
		if (! Compare(sum+dx->Get_Real_Max()*weights[x]))
		{
			int i = dx->Get_Max();
			modif = true;
			ds->Add_Element (scope_var[x]);
			
			do
			{
				dx->Filter_Value (i);
				dx_size--;
					
				i = dx->Previous_Value (i);
			}
			while ((i != -1) && (! Compare (sum + dx->Get_Real_Value (i) * weights[x])));
		}
	}
	else
		if (! Compare (sum+dx->Get_Real_Min()*weights[x]))
		{
			int i = dx->Get_Min();
			modif = true;
			ds->Add_Element (scope_var[x]);
			
			do
			{
				dx->Filter_Value (i);
				dx_size--;
					
				i = dx->Next_Value (i);
			}
			while ((i != -1) && (! Compare (sum + dx->Get_Real_Value (i) * weights[x])));
		}

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
	
  return modif;
}


void Less_Weighted_Sum_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  int sum = 0;
  Domain * dy;

  for (unsigned int i = 0; i < arity; i++)  
  {
    dy = scope_var [i]->Get_Domain();
    if (weights[i] > 0)
      sum += dy->Get_Real_Min() * weights[i];
    else sum += dy->Get_Real_Max() * weights[i];
  }

  for (unsigned int i = 0 ; i < arity; i++)
  {
    Domain * dx = scope_var [i]->Get_Domain();
    
    if (weights[i] > 0)
    {
      sum -= dx->Get_Real_Min() * weights[i];  
      int v = dx->Get_Max();
      
      if (! Compare(sum+dx->Get_Real_Value (v)*weights[i]))
      {
        ds->Add_Element (scope_var[i]);
        
        do
        {
          dx->Filter_Value (v);
            
          v = dx->Previous_Value (v);
        }
        while ((v != -1) && (! Compare (sum + dx->Get_Real_Value (v) * weights[i])));
      
        if (dx->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[i],this);
          return;
        }
      }
      
      sum += dx->Get_Real_Min() * weights[i];  
    }
    else
      {
        sum -= dx->Get_Real_Max() * weights[i];
        int v = dx->Get_Min();
        
        if (! Compare (sum+dx->Get_Real_Value (v)*weights[i]))
        {
          ds->Add_Element (scope_var[i]);
          
          do
          {
            dx->Filter_Value (v);
            v = dx->Next_Value (v);
          }
          while ((v != -1) && (! Compare (sum + dx->Get_Real_Value (v) * weights[i])));
          
          if (dx->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[i],this);
            return;
          }
        }

        sum += dx->Get_Real_Max() * weights[i];
      }	
  }
}
