#include "nogood_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Nogood_Constraint::Nogood_Constraint (vector<Variable *> & var, int * ft, int n): Constraint (var)
// constructs a new constraint with number n which involves the variable in var whose all the tuples are allowed except the tuple ft
{
	forbidden_tuple = new int [arity];
}


Nogood_Constraint::Nogood_Constraint (Nogood_Constraint & c): Constraint (c)
// constructs a new constraint by copying the constraint c
{
	forbidden_tuple = new int [arity];
	for (unsigned int i = 0; i < arity; i++)
		forbidden_tuple[i] = c.forbidden_tuple[i];		
}


Nogood_Constraint::~Nogood_Constraint()
// destructor
{
	delete [] forbidden_tuple;
}



//------------------
// basics functions
//------------------


bool Nogood_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	unsigned int i = 0;
	while ((i < arity) && (t[i] == forbidden_tuple[i]))
		i++;
	
	return i != arity;
}


bool Nogood_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  if (unassigned_variable_number > 1)
		return false;
	else
	{
		bool possible_modif;		// true if a modification is possible, false otherwise
		
		possible_modif = true;
		Domain * d_x = 0;
		int pos_x = -1;
		
		for (unsigned int i = 0; (i < arity) && (possible_modif); i++)
		{
			if (scope_var[i]->Get_Num() != var)
			{
				if (scope_var[i]->Get_Domain()->Get_Remaining_Value(0) != forbidden_tuple[i])
					possible_modif = false;
			}
			else 
				{
					d_x = scope_var[i]->Get_Domain();
					if (d_x->Is_Present (forbidden_tuple[i]))
						pos_x = i;
					else possible_modif = false;
				}
		}
		
		if (possible_modif)
		{
			ds->Add_Element (scope_var[pos_x]);
			d_x->Filter_Value (forbidden_tuple[pos_x]);

      if (d_x->Get_Size() == 0)
        pb->Set_Conflict (scope_var[pos_x],this);
			
			return true;
		}
		else return false;
	}
}


void Nogood_Constraint::Get_Allowed_Tuple_List (list<int *> & list)
// constructs the list of tuples allowed by the constraint
{
  list.clear();
  
	int t [arity];
	int t0 [arity];
	int size;
	Domain * d;
	unsigned int i;
	bool no_tuple;
	
  // we compute the first tuple
  for (i = 0; i < arity; i++)
  {
    d = scope_var[i]->Get_Domain();
    size = d->Get_Initial_Size();
    
    t[i] = 0;
    while ((t[i] < size) && (! d->Is_Present (t[i])))
      t[i]++;
      
    if (t[i] == size)
      return;
      
    t0[i] = t[i];
  }
  
  // we compute all the tuples
  i = 0;
  d = scope_var[i]->Get_Domain();
  size = d->Get_Initial_Size();
  do
  {
    i = 0;

    if (Is_Satisfied (t))
      list.push_back (t);

    no_tuple = true;
    do
    {
      do
        t[i]++;
      while ((t[i] < size) && (! d->Is_Present (t[i])));

      if (t[i] == size)
      {
        t[i] = t0[i];
        i++;
        if (i < arity)
        {
          d = scope_var[i]->Get_Domain();
          size = d->Get_Initial_Size();
        }
      }
      else no_tuple = false;
    }
    while ((no_tuple) && (i < arity));
  }
  while (i < arity);
}
