#include "equal_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Equal_Binary_Constraint::Equal_Binary_Constraint (vector<Variable *> & var, int a, int b): Binary_Constraint (var)		
// constructs a new constraint which involves the variable in var and whose relation imposes y = a.x + b)
{
  constant_a = a;
  constant_b = b;
  equal_value = new int * [arity];
  
  Domain * dx = scope_var[0]->Get_Domain();
  Domain * dy = scope_var[1]->Get_Domain();
  
	for (unsigned int i = 0; i < arity; i++)
	{
    equal_value[i] = new int [scope_var[i]->Get_Domain()->Get_Initial_Size()];

		for (unsigned int j = 0; j < scope_var[i]->Get_Domain()->Get_Initial_Size(); j++)
			equal_value[i][j] = -1;
	}
	
	for (int j = dy->Get_Size()-1; j >= 0; j--)
	{
    int v1 = dy->Get_Remaining_Value(j);
    int real_v1 = dy->Get_Remaining_Real_Value(j);
    int v0 = dx->Get_Value(constant_a * real_v1 + constant_b);

    if (v0 == -1)
      dy->Filter_Value (v1);
    else
      {
        equal_value[1][v1] = v0;
        equal_value[0][v0] = v1;
      }    
	}

  for (int j = dx->Get_Size()-1; j >= 0; j--)
    if (equal_value[0][dx->Get_Remaining_Value(j)] == -1)
      dx->Filter_Value(dx->Get_Remaining_Value(j));
}


Equal_Binary_Constraint::Equal_Binary_Constraint (Equal_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  constant_a = c.constant_a;
  constant_b = c.constant_b;
	equal_value = new int * [arity];
	for (unsigned int i = 0; i < arity; i++)
	{
		equal_value[i] = new int [scope_var[i]->Get_Domain()->Get_Initial_Size()];
		for (unsigned int j = 0; j < scope_var[i]->Get_Domain()->Get_Initial_Size(); j++)
			equal_value[i][j] = c.equal_value[i][j];
	}
}


Equal_Binary_Constraint::~Equal_Binary_Constraint ()
// destructor
{
  delete [] equal_value[0];
  delete [] equal_value[1];
  delete [] equal_value;
}


//-----------------
// basic functions
//-----------------


bool Equal_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) == constant_a * scope_var[1]->Get_Domain()->Get_Real_Value(t[1]) + constant_b;
}


bool Equal_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
	Domain * dy = scope_var[1-x]->Get_Domain();
	int dx_size = dx->Get_Size();
  
	int i = 0;
	bool modif = false;
	int v;
	
	do
	{
		v = dx->Get_Remaining_Value (i);
		
    if (! dy->Is_Present(equal_value[x][v]))
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			dx_size--;
			
			modif = true;
		}
		else i++;
	}
	while (i < dx_size);

  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);
	
  return modif;
}


void Equal_Binary_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  int fix_var = -1;   // a variable which has a singleton domain
  
  if (event_manager->Exist_Event_Fix (scope_var[0]->Get_Num(), ref))
    fix_var = 0;
  else
    if (event_manager->Exist_Event_Fix (scope_var[1]->Get_Num(), ref))
      fix_var = 1;
  
  timestamp current_time = event_manager->Get_Current_Time();
  for (unsigned int i = 0; (i < arity) && (fix_var == -1); i++)
  {
    int num_var = scope_var[i]->Get_Num();
    
    if (event_manager->Exist_Event_Dmc (num_var, ref))
    {
      Domain * dx = scope_var[i]->Get_Domain();

      for (unsigned int j = 0; j < dx->Get_Initial_Size(); j++)
        if ((event_manager->Exist_Event_Dmc (num_var,j,ref)) && (! event_manager->Exist_Event_Dmc (num_var,j,current_time)))
        {
          if (scope_var[1-i]->Get_Domain()->Is_Present(equal_value[i][j]))
          {
            ds->Add_Element (scope_var[1-i]);
            scope_var[1-i]->Get_Domain()->Filter_Value(equal_value[i][j]);
            if (scope_var[1-i]->Get_Domain()->Get_Size() == 0)
            {
              pb->Set_Conflict (scope_var[1-i],this);
              return;
            }
            else
              if (scope_var[1-i]->Get_Domain()->Get_Size() == 1)
                fix_var = 1-i;
          }
        }
    }
  }

  if (fix_var != -1)
  {
    assert (scope_var[fix_var]->Get_Domain()->Get_Size() == 1);
    int v = scope_var[fix_var]->Get_Domain()->Get_Remaining_Value(0);     // the remaining value of the variable scope_var[fix_var]
    int j = 1 - fix_var;
    ds->Add_Element (scope_var[j]);
    scope_var[j]->Get_Domain()->Assign(equal_value[fix_var][v]);
    if (scope_var[j]->Get_Domain()->Get_Size() == 0)
    {
      pb->Set_Conflict (scope_var[j],this);
      return;
    }
  }
}


string Equal_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> eq(" + scope_var[0]->Get_Name() + ",sqr(" + scope_var[1]->Get_Name()+")) </intension>";
}
