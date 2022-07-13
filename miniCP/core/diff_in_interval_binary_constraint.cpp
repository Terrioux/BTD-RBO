#include "diff_in_interval_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Diff_In_Interval_Binary_Constraint::Diff_In_Interval_Binary_Constraint (vector<Variable *> & var, int ref_min, int ref_max): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that the difference X1 - X0 belongs to the integer interval [ref_min,ref_max]
{	
	min = ref_min;
	max = ref_max;	
}


Diff_In_Interval_Binary_Constraint::Diff_In_Interval_Binary_Constraint (Diff_In_Interval_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	min = c.min;
	max = c.max;
}



//-----------------
// basic functions
//-----------------


bool Diff_In_Interval_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	int diff = scope_var[0]->Get_Domain()->Get_Real_Value (t[0]) - scope_var[1]->Get_Domain()->Get_Real_Value (t[1]);
	return (min <= diff) && (diff <= max);
}


bool Diff_In_Interval_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
	Domain * dy = scope_var [1-x]->Get_Domain();
	int dx_size = dx->Get_Size();

	int i = 0;
	bool modif = false;
	int v;
	int val;
	
	if (x == 0)
	{
		int int_min = dy->Get_Real_Min() + min;
		int int_max = dy->Get_Real_Max() + max;

		do
		{
			val = dx->Get_Remaining_Real_Value (i);
			
			if ((val < int_min ) && (val > int_max))
			{
				v = dx->Get_Remaining_Value (i);
				ds->Add_Element (scope_var[x]);
				dx->Filter_Value (v);
				dx_size--;
				
				modif = true;
			}
			else i++;
		}
		while (i < dx_size);
	}
	else	
		{
			int int_min = dy->Get_Real_Max() - min;
			int int_max = dy->Get_Real_Min() - max;
			
			do
			{
				val = dx->Get_Remaining_Real_Value (i);
				
				if ((val < int_min) && (val > int_max))
				{
					v = dx->Get_Remaining_Value (i);
					ds->Add_Element (scope_var[x]);
					dx->Filter_Value (v);
					dx_size--;
					
					modif = true;
				}
				else i++;
			}
			while (i < dx_size);
		}
	
  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);
  
  return modif;
}


void Diff_In_Interval_Binary_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  bool again;     // true if a modification may trigger other modifications, false otherwise
  do
  {
    again = false;
    for (unsigned int i = 0; i < arity; i++)
    {
      if ((event_manager->Exist_Event_Lbc (scope_var[i]->Get_Num(),ref)) || (ref == 0))
      {
        Domain * dx = scope_var [i]->Get_Domain();
        Domain * dy = scope_var [1-i]->Get_Domain();
        
        int val;
        if (i == 0) 
          val = dx->Get_Real_Min() - max;
        else val = dy->Get_Real_Min() + min;
        
        int v = dy->Get_Min();
        if (dy->Get_Real_Value(v) < val)
        {
          ds->Add_Element (scope_var[1-i]);
          do
          {
            dy->Filter_Value (v);
            v = dy->Next_Value(v);
          }
          while ((v != -1) && (dy->Get_Real_Value(v) < val));
          
          if (dy->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[1-i],this);
            return;
          }

          if (i == 1)
            again = true;
        }
      }
    
      if ((event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(),ref)) || (ref == 0))
      {
        Domain * dx = scope_var [i]->Get_Domain();
        Domain * dy = scope_var [1-i]->Get_Domain();
             
        int val;
        if (i == 0)
          val = dx->Get_Real_Max() - min;
        else val = dx->Get_Real_Max() + max;
        
        int v = dy->Get_Max();
        if (dy->Get_Real_Value(v) > val)
        {
          ds->Add_Element (scope_var[1-i]);

          do
          {
            dy->Filter_Value (v); 
            v = dy->Previous_Value(v);
          }
          while ((v != -1) && (dy->Get_Real_Value(v) > val));
     
          if (dy->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[1-i],this);
            return;
          }
          
          if (i == 1)
            again = true;
        }
      }
    }
  }
  while (again);
}


string Diff_In_Interval_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> and(le(" + to_string (min) + ", sub(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + ")),le(sub(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + ")," + to_string(max)  + ")) </intension>";
}
