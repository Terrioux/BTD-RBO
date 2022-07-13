#include "less_sum_global_constraint.h"


//--------------
// constructors 
//--------------


Less_Sum_Global_Constraint::Less_Sum_Global_Constraint (vector<Variable *> & var, bool strict, int cst): Sum_Global_Constraint (var,cst)
// constructs a new constraint which involves the variable in var and  compares the sum of the values of the variables in var to the constant cst with respect to < or <= if the boolean strict is true
{
  strict_inequality = strict;
}


Less_Sum_Global_Constraint::Less_Sum_Global_Constraint (Less_Sum_Global_Constraint & c): Sum_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  strict_inequality = c.strict_inequality;
}


//-----------------
// basic functions
//-----------------

bool Less_Sum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
	int sum = 0;
	Domain * dy;

	for (unsigned int i = 0; i < arity; i++)
		if (scope_var[i]->Get_Num() != var)
		{
			dy = scope_var [i]->Get_Domain();
			sum += dy->Get_Real_Min();
		}

  int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
	
	bool modif = false;
	int v;
	
	v = dx->Get_Max();
	
	while ((v != -1) && (! Compare(sum + dx->Get_Real_Value(v))))
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


void Less_Sum_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  int sum = 0;
  Domain * dy;

  for (unsigned int i = 0; i < arity; i++)
  {
    dy = scope_var [i]->Get_Domain();
    sum += dy->Get_Real_Min();
  }
  
  for (unsigned int i = 0 ; i < arity; i++)
  {
    Domain * dx = scope_var [i]->Get_Domain();
    
    sum -= dx->Get_Real_Min();
    
    int v = dx->Get_Max();
    
    if (! Compare(sum + dx->Get_Real_Value(v)))
    {      
      ds->Add_Element (scope_var[i]);
      do
      {
        dx->Filter_Value (v);
        v = dx->Previous_Value(v);
      }
      while ((v != -1) && (! Compare(sum + dx->Get_Real_Value(v))));
      
      if (dx->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[i],this);
        return;
      }
    }
    
    sum += dx->Get_Real_Min();
  }
}
