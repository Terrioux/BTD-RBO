#include "positive_product_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Positive_Product_Binary_Constraint::Positive_Product_Binary_Constraint (vector<Variable *> & var): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that the product of the two numbers is positive or zero
{
}


Positive_Product_Binary_Constraint::Positive_Product_Binary_Constraint (Positive_Product_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


//-----------------
// basic functions
//-----------------


bool Positive_Product_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	if (scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) == 0)
		return true;
	else
		if (scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) > 0)
			return scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) >= 0;
		else return scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) <= 0;
}


bool Positive_Product_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	int y = 1 - x;
	Domain * dx = scope_var[x]->Get_Domain();
	Domain * dy = scope_var[y]->Get_Domain();
	
	bool modif = false;
	
	if (dy->Get_Real_Min() > 0)
	{
		int v = dx->Get_Min();
		while ((v != -1) && (dx->Get_Real_Value(v) < 0))
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);

			modif = true;

			v = dx->Next_Value(v);
		}
	}
	else
		if (dy->Get_Real_Max() < 0)
		{
			int v = dx->Get_Max();
			while ((v != -1) && (dx->Get_Real_Value(v) > 0))
			{
				ds->Add_Element (scope_var[x]);
				dx->Filter_Value (v);

				modif = true;

				v = dx->Previous_Value(v);
			}
		}

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);

	return modif;
}


void Positive_Product_Binary_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  bool again;   // true if a modification may trigger other modifications, false otherwise
  
  do
  {
    again = false;
    for (unsigned int i = 0; i < arity; i++)
    {
      if ((event_manager->Exist_Event_Lbc (scope_var[i]->Get_Num(),ref)) || (ref == 0))
      {
        Domain * dx = scope_var[i]->Get_Domain();
        if (dx->Get_Real_Min() > 0)
        {
          Domain * dy = scope_var[1-i]->Get_Domain();
          
          int v = dy->Get_Min();
          if (dy->Get_Real_Value(v) < 0)
          {
            ds->Add_Element (scope_var[1-i]);
            while ((v != -1) && (dy->Get_Real_Value(v) < 0))
            {
              dy->Filter_Value (v);
              v = dy->Next_Value(v);
            }
            
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

      if ((event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(),ref)) || (ref == 0))
      {
        Domain * dx = scope_var[i]->Get_Domain();
        if (dx->Get_Real_Max() < 0)
        {
          Domain * dy = scope_var[1-i]->Get_Domain();
          
          int v = dy->Get_Max();
          if (dy->Get_Real_Value(v) > 0)
          {
            ds->Add_Element (scope_var[1-i]);
            while ((v != -1) && (dy->Get_Real_Value(v) > 0))
            {
              dy->Filter_Value (v);
              v = dy->Previous_Value(v);
            }
            
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
  }
  while (again);
}


string Positive_Product_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> ge(mul(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name() + "),0) </intension>";
}
