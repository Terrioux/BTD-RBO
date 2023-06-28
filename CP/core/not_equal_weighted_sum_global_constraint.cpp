#include "not_equal_weighted_sum_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Not_Equal_Weighted_Sum_Global_Constraint::Not_Equal_Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, long cst): Weighted_Sum_Global_Constraint (var,w,cst)
// constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var weighted by weight from w to the constant cst with respect to !=
{	
}


Not_Equal_Weighted_Sum_Global_Constraint::Not_Equal_Weighted_Sum_Global_Constraint (Not_Equal_Weighted_Sum_Global_Constraint & c): Weighted_Sum_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


//-----------------
// basic functions
//-----------------


bool Not_Equal_Weighted_Sum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
	for (unsigned int i = 0; i < arity; i++)
		if ((scope_var[i]->Get_Num() != var) && (scope_var [i]->Get_Domain()->Get_Size() != 1))
			return false;

	long sum = 0;
  unsigned int x = Get_Position (var);
	for (unsigned int i = 0; i < arity; i++)
		if (i != x)
			sum += scope_var [i]->Get_Domain()->Get_Remaining_Real_Value (0) * weights[i];

	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	int i = 0;
	bool modif = false;
	int v;
	
	do
	{
		v = dx->Get_Remaining_Real_Value (i);
		
		if (sum+v*weights[x] == constant)
		{
			v = dx->Get_Remaining_Value (i);
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			dx_size--;
			
			modif = true;
		}
		else i++;
	}
	while ((i < dx_size) && (!modif));
	
  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);
  
  return modif;
}


void Not_Equal_Weighted_Sum_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
	long sum = 0;
	unsigned int x = arity;
  unsigned int nb = 0;
  
	for (unsigned int i = 0; i < arity; i++)
		if (scope_var[i]->Get_Domain()->Get_Size() == 1)
    {
      sum += scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0) * weights[i];
      nb++;
    }
    else 
      if (x == arity)
        x = i;
      else return;    // there is more than one variable which has not a singleton domain
        

  if (x == arity)
  {
    if (sum == constant)
    {
      // we wipe out a domain in order to stop the filtering
      Domain * dx = scope_var[0]->Get_Domain();
      ds->Add_Element (scope_var[0]);
      dx->Filter_Value (dx->Get_Remaining_Value(0));
      pb->Set_Conflict (scope_var[0],this);
    }
  }
  else
    {
      Domain * dx = scope_var[x]->Get_Domain();
      
      if (weights[x] > 0)
      {
        int v = dx->Get_Min();
        
        while ((v != -1) && (sum + dx->Get_Real_Value(v) * weights[x] < constant))
          v = dx->Next_Value(v);
          
        if ((v != -1) && (sum + dx->Get_Real_Value(v) * weights[x] == constant))
        {
          ds->Add_Element (scope_var[x]);
          dx->Filter_Value (v);
        }
      }
      else
        {
          int v = dx->Get_Max();
          
          while ((v != -1) && (sum + dx->Get_Real_Value(v) * weights[x] < constant))
            v = dx->Previous_Value(v);
            
          if ((v != -1) && (sum + dx->Get_Real_Value(v) * weights[x] == constant))
          {
            ds->Add_Element (scope_var[x]);
            dx->Filter_Value (v);
          }
        }

      if (dx->Get_Size() == 0)
        pb->Set_Conflict (scope_var[x],this);
    }
}
