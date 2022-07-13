#include "constraint.h"
#include "csp.h"
#include "assignment.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Constraint::Constraint ()
// constructs a constraint with an empty scope
{
	num = -1;
	arity = 0;
	
	unassigned_variable_number = 0;
}


Constraint::Constraint (vector<Variable *> & var)
// constructs a constraint whose scope is defined by the variables in var
{
	num = -1;
	arity = var.size();
	
	unassigned_variable_number = arity;

	scope_var.resize (arity);
	
	for (unsigned int i = 0; i < arity; i++)
	{
		scope.insert (var[i]->Get_Num());
		scope_var[i] = var[i];
		
		position.insert(pair<int,int>(var[i]->Get_Num(),i));
	}
}


Constraint::Constraint (Constraint & c)
// constructs a constraint by copying the constraint c
{
	num = c.num;
	arity = c.arity;
	
	unassigned_variable_number = c.unassigned_variable_number;

	position = c.position;
	scope = c.scope;
	scope_var = c.scope_var;
}


Constraint::~Constraint()
// destructor
{
}


//-----------------
// basic functions
//-----------------


bool Constraint::Is_Satisfied (Assignment & A)
{
  int t [arity];
  
  for (unsigned int i = 0; i < arity; i++)
    if (A.Is_Assigned(scope_var[i]->Get_Num()))
      t[i] = A.Get_Value(scope_var[i]->Get_Num());
    else return false;
  return Is_Satisfied(t);
}


bool Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  if (arity == 0)   // useful for nogood base global constraint 
    return false;
  
  int x = Get_Position (var);

  Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	int i = 0;
	int v;
	
  int * ts;
  bool has_no_support;
  bool record_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
	bool exist_more_tuples;
	
  Tuple t (arity);
  
	do
	{
		v = dx->Get_Remaining_Value (i);
		
		ts = ls->Get_Support (num,x,v);
		
		if (ts == 0) 
		{
  		t[x] = v;
			t.Complete (scope_var,x);
			has_no_support = true;
			record_support = true;
			exist_more_tuples = true;
		}
		else
			{
				for (unsigned int j = 0; j < arity; j++)
					t[j] = ts[j];

				if (t.Is_Valid (scope_var))
				{
					has_no_support = false;
					record_support = false;
					exist_more_tuples = true;
				}
				else 
					{
						has_no_support = true;
						record_support = true;
						
						if (ls->Continue_From_Scratch())
						{
							t[x] = v;
							t.Complete (scope_var,x);
							exist_more_tuples = true;							
						}
						else exist_more_tuples = t.Next_Compatible_Valid (scope_var,x);
					}
			}
				
		while ((has_no_support) && (exist_more_tuples))
		{
      if (Is_Satisfied(t) != 0)
				has_no_support = false;
			else exist_more_tuples = t.Next_Valid (scope_var,x);
		}
		
		if (has_no_support)
		{
			ds->Add_Element (scope_var [x]);
			dx->Filter_Value (v);
			dx_size--;
			
			modif = true;
		}
		else 
			{
				if (record_support)
				{
					ds->Add_Support (num,x,v,arity,ts);
					ls->Change_Support (num,x,v,t.Get_Values(),arity);
				}
				i++;
			}
  }
  while (i < dx_size);

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
  
  return modif;
}


void Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  bool again;
  do
  {
    again = false;
    unsigned int old_size;
    for (vector<Variable *>::iterator iter = Begin_Var(); iter != End_Var(); iter++)
    {
      old_size = (*iter)->Get_Domain()->Get_Size();
      if ((! A.Is_Assigned ((*iter)->Get_Num())) && (Revise (pb,(*iter)->Get_Num(),ls,ds)) && ((*iter)->Get_Domain()->Get_Size() == 0))
        return;
      if (old_size != (*iter)->Get_Domain()->Get_Size())
        again = true;
    }
  }
  while (again);
}


void Constraint::Get_Allowed_Tuple_List (list<int *> & list)
// constructs the list of tuples allowed by the constraint
{
  list.clear();
  
	Tuple t (arity);
	t.Complete (scope_var);
	int * t2;
	bool again;
	
	do
	{
		if (Is_Satisfied (t.Get_Values()))
		{
			t2 = new int [arity];
			for (unsigned int i = 0; i < arity; i++)
				t2[i] = t[i];
			
			list.push_back (t2);
		}
		again = t.Next_Valid (scope_var);	
	}
	while (again);
}
