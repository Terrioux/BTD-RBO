#include "all_equal_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


All_Equal_Global_Constraint::All_Equal_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that all the variables are equal
{
  equal_value = new int ** [arity];
	for (unsigned int i = 0; i < arity; i++)
	{
		equal_value[i] = new int * [scope_var[i]->Get_Domain()->Get_Initial_Size()];
		for (unsigned int j = 0; j < scope_var[i]->Get_Domain()->Get_Initial_Size(); j++)
			equal_value[i][j] = 0;
	}
	
	for (unsigned int j = 0; j < scope_var[0]->Get_Domain()->Get_Initial_Size(); j++)
	{
		int * t = new int [arity];
		t[0] = j;
		int val = scope_var[0]->Get_Domain()->Get_Real_Value(j);
    bool ok = true;
		for (unsigned int i = 1; i < arity; i++)
		{
			t[i] = scope_var[i]->Get_Domain()->Get_Value(val);
      if (t[i] == -1)
        ok = false;
		}
		
		if (ok)
		{
			for (unsigned int i = 0; i < arity; i++)
				equal_value[i][t[i]] = t;
		}
		else 
      {
        for (unsigned int i = 0; i < arity; i++)
          if (t[i] != -1)
            scope_var[i]->Get_Domain()->Filter_Value (t[i]);
        delete [] t;
      }
	}

  for (unsigned int i = 1; i < arity; i++)
    for (unsigned int j = 0; j < scope_var[i]->Get_Domain()->Get_Initial_Size(); j++)
      if (equal_value[i][j] == 0)
        scope_var[i]->Get_Domain()->Filter_Value (j);
}


All_Equal_Global_Constraint::All_Equal_Global_Constraint (All_Equal_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	equal_value = new int ** [arity];
	for (unsigned int i = 0; i < arity; i++)
	{
		equal_value[i] = new int * [scope_var[i]->Get_Domain()->Get_Initial_Size()];
		for (unsigned int j = 0; j < scope_var[i]->Get_Domain()->Get_Initial_Size(); j++)
			equal_value[i][j] = 0;
	}
	
	for (unsigned int j = 0; j < scope_var[0]->Get_Domain()->Get_Initial_Size(); j++)
		if (c.equal_value[0][j] != 0)
		{
			int * t = new int [arity];
			for (unsigned int i = 0; i < arity; i++)
				t[i] = c.equal_value[0][j][i];

			for (unsigned int i = 0; i < arity; i++)
				equal_value[i][t[i]] = t;
		}
}


All_Equal_Global_Constraint::~All_Equal_Global_Constraint ()
// destructor
{
	for (unsigned int j = 0; j < scope_var[0]->Get_Domain()->Get_Initial_Size(); j++)
		delete [] equal_value[0][j];
	
	for (unsigned int i = 0; i < arity; i++)
		delete [] equal_value[i];
	
	delete [] equal_value;
}


//-----------------
// basic functions
//-----------------


bool All_Equal_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	unsigned int i = 1;
	int val = scope_var[0]->Get_Domain()->Get_Real_Value (t[0]);
	while ((i < arity) && (scope_var[i]->Get_Domain()->Get_Real_Value (t[i]) == val))
		i++;

	return i == arity;
}


bool All_Equal_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();

	int i = 0;
	bool modif = false;
	int v;
	
	do
	{
		v = dx->Get_Remaining_Value (i);
		
		unsigned int j = 0;
		if (equal_value[x][v] != 0)
		{
			while ((j < arity) && (scope_var[j]->Get_Domain()->Is_Present(equal_value[x][v][j])))
				j++;
		}
		
		if (j < arity)
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


void All_Equal_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  int fix_var = -1;   // a variable which has a singleton domain
  
  for (unsigned int i = 0; (i < arity) && (fix_var == -1); i++)
    if (event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(), ref))
      fix_var = i;
  
  timestamp current_time = event_manager->Get_Current_Time();
  for (unsigned int i = 0; (i < arity) && (fix_var == -1); i++)
  {
    int num_var = scope_var[i]->Get_Num();
    
    if (event_manager->Exist_Event_Dmc (num_var, ref))
    {
      Domain * dx = scope_var[i]->Get_Domain();

      for (unsigned int j = 0; j < dx->Get_Initial_Size(); j++)
        if ((event_manager->Exist_Event_Dmc (num_var,j,ref)) && (! event_manager->Exist_Event_Dmc (num_var,j,current_time)) && (equal_value[i][j] != 0))
        {
          for (unsigned int k = 0; k < arity; k++)
            if ((i != k) && (scope_var[k]->Get_Domain()->Is_Present(equal_value[i][j][k])))
            {
              ds->Add_Element (scope_var[k]);
              scope_var[k]->Get_Domain()->Filter_Value(equal_value[i][j][k]);
              if (scope_var[k]->Get_Domain()->Get_Size() == 0)
              {
                pb->Set_Conflict (scope_var[k],this);
                return;
              }
              else
                if (scope_var[k]->Get_Domain()->Get_Size() == 1)
                  fix_var = k;
            }
        }
    }
  }

  if (fix_var != -1)
  {
    assert (scope_var[fix_var]->Get_Domain()->Get_Size() == 1);
    int v = scope_var[fix_var]->Get_Domain()->Get_Remaining_Value(0);     // the remaining value of the variable scope_var[fix_var]
    for (unsigned j = 0; j < arity; j++)
      if (j != (unsigned) fix_var) 
      {
        ds->Add_Element (scope_var[j]);
        scope_var[j]->Get_Domain()->Assign(equal_value[fix_var][v][j]);
        if (scope_var[j]->Get_Domain()->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[j],this);
          return;
        }
      }
  }
}


string All_Equal_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<allEqual> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</allEqual>";
  return expr;
}
