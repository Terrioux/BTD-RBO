#include "all_different_except_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


All_Different_Except_Global_Constraint::All_Different_Except_Global_Constraint (vector<Variable *> & var, int exception): Global_Constraint (var)
// constructs a all-diff global constraint with an exception value
{
  exception_value = exception;
  val = new int [arity];
  real_val = new int [arity];
  candidates = new int [arity];
	for (unsigned int i = 0; i < arity; i++)
		candidates[i] = i;
}
		
		
All_Different_Except_Global_Constraint::All_Different_Except_Global_Constraint (All_Different_Except_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
  exception_value = c.exception_value;
  val = new int [arity];
  real_val = new int [arity];
  candidates = new int [arity];
	for (unsigned int i = 0; i < arity; i++)
		candidates[i] = i;
}


All_Different_Except_Global_Constraint::~All_Different_Except_Global_Constraint ()
// destructor
{
  delete [] val;
  delete [] real_val;
  delete [] candidates;
}


//-----------------
// basic functions
//-----------------


bool All_Different_Except_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < arity-1; i++)
  {
    int value = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]);
    if (value != exception_value)
    {
      for (unsigned int j = i+1; j < arity; j++)
        if ((value != exception_value) && (value == scope_var[j]->Get_Domain()->Get_Real_Value(t[j])))
          return false;
    }
  }
  return true;
}

#include <iostream>
void All_Different_Except_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
	all_values.clear();
	
  int nb_with_exception = 0;
  
	for (unsigned int i = 0; i < arity; i++)
  {
    Domain * d = scope_var[candidates[i]]->Get_Domain();
    int val = d->Get_Value(exception_value);
     
    if ((val != -1) && (d->Is_Present(val)))
    {
      // we "remove" the candidate (we swap to avoid the reinitialization of candidates)
      int aux;
      aux = candidates[nb_with_exception];
      candidates[nb_with_exception] = candidates[i];
      candidates[i] = aux;
      
      nb_with_exception++;
    }
  }
  
  unsigned int nb = 0;
	for (unsigned int i = nb_with_exception; i < arity; i++)
	{
		unsigned int min = scope_var[candidates[i]]->Get_Domain()->Get_Size();
		int ind_min = i;
    
		for (unsigned int j = i+1; (j < arity) && (min > 1); j++)
			if (scope_var[candidates[j]]->Get_Domain()->Get_Size() < min)
			{
				min = scope_var[candidates[j]]->Get_Domain()->Get_Size();
				ind_min = j;
			}
		
		// we update de the set all_values
		Domain * d = scope_var[candidates[ind_min]]->Get_Domain();		
		for (unsigned int j = 0; j < d->Get_Size(); j++)
      if (d->Get_Remaining_Real_Value(j) != exception_value)
        all_values.insert(d->Get_Remaining_Real_Value(j));
	

		// we "remove" the selected candidate (we swap to avoid the reinitialization of candidates)
    int aux;
    aux = candidates[ind_min];
		candidates[ind_min] = candidates[i];
    candidates[i] = aux;
	
		if (all_values.size() == nb+1)
		{
			// we remove all the values in all_values from the remaining domains
			for (unsigned int j = i+1; j < arity; j++)
			{
				d = scope_var[candidates[j]]->Get_Domain();		
				for (set<int>::iterator iter = all_values.begin(); (iter != all_values.end()) && (d->Get_Size() > 0); iter++)
				{
					int v = d->Get_Value(*iter);

					if ((v != -1) && (d->Is_Present(v)))
					{
						ds->Add_Element (scope_var[candidates[j]]);
						d->Filter_Value (v);
					}
				}
				
				if (d->Get_Size () == 0)
        {
          pb->Set_Conflict (scope_var[candidates[j]],this);
					return;
        }
			}
		}
    nb++;
	}
}


string All_Different_Except_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<allDifferent>\n";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <except> "+ to_string(exception_value)+ " </except>\n";
  expr += "</allDifferent>";
  return expr;
}
