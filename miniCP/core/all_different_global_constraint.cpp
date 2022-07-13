#include "all_different_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


All_Different_Global_Constraint::All_Different_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a all-diff global constraint 
{
  val = new int [arity];
  real_val = new int [arity];
  candidates = new int [arity];
	for (unsigned int i = 0; i < arity; i++)
		candidates[i] = i;
}
		
		
All_Different_Global_Constraint::All_Different_Global_Constraint (All_Different_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
  val = new int [arity];
  real_val = new int [arity];
  candidates = new int [arity];
	for (unsigned int i = 0; i < arity; i++)
		candidates[i] = i;
}


All_Different_Global_Constraint::~All_Different_Global_Constraint ()
// destructor
{
  delete [] val;
  delete [] real_val;
  delete [] candidates;
}


//-----------------
// basic functions
//-----------------


bool All_Different_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < arity-1; i++)
  {
    int value = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]);
    for (unsigned int j = i+1; j < arity; j++)
      if (value == scope_var[j]->Get_Domain()->Get_Real_Value(t[j]))
        return false;
  }
  return true;
}


bool All_Different_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
  
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	int i = 0;
	int v;
	
  int * ts;
  bool has_no_support;
  bool record_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
	bool exist_more_tuples;
	int current = -1;
  
	do
	{
		v = dx->Get_Remaining_Value (i);
		
		ts = ls->Get_Support (num,x,v);
		
		if (ts == 0) 
		{
      for (unsigned int j = 0; j < arity; j++)
        val[j] = -1;
      val[x] = v;
      real_val[x] = dx->Get_Real_Value(v);
      
      if (x == 0)
        current = 1;
      else current = 0;
			
			has_no_support = true;
			record_support = true;
			exist_more_tuples = true;
		}
		else
			{
        bool valid = true;
				for (unsigned int j = 0; j < arity; j++)
        {
          if (! scope_var[j]->Get_Domain()->Is_Present(ts[j]))
          {
            valid = false;
            current = j;
          }

          if (valid)
          {
            val[j] = ts[j];
            real_val[j] = scope_var[j]->Get_Domain()->Get_Real_Value(ts[j]);
          }
          else val[j] = -1;
        }    

				if (valid)
				{
					has_no_support = false;
					record_support = false;
					exist_more_tuples = true;
				}
				else 
					{
						has_no_support = true;
						record_support = true;
						exist_more_tuples = true;
						
						if (ls->Continue_From_Scratch())
						{
              for (unsigned int j = 0; j < arity; j++)
                val[j] = -1;
      				val[x] = v;
              real_val[x] = dx->Get_Real_Value(v);
              
              if (x == 0)
                current = 1;
              else current = 0;
														
						}
					}
			}
      
    Domain * d;
		while ((has_no_support) && (exist_more_tuples))
		{
      // we select a possible value for the current position
      
      if (val[current] == -1)
        val[current] = 0;
      else val[current]++;
      
      d = scope_var[current]->Get_Domain();
      bool found = false;

      while (((unsigned) val[current] < d->Get_Initial_Size()) && (! found))
      {

        if (d->Is_Present(val[current]))
        {
          real_val[current] = d->Get_Real_Value (val[current]);
          
          if (real_val[current] != real_val[x])
          {
            int j = 0; 
            while ((j < current) && (real_val[j] != real_val[current]))
              j++;
              
            if (j == current)
              found = true;
            else val[current]++;
          }
          else val[current]++;
        }
        else val[current]++;
      }
      
      if (found)
      {
        current++;
        if (current == (signed) x)
          current++;
        
        if (current == (signed) arity)
          has_no_support = false;
      }
      else
        {
          val[current] = -1;
          current--;
          if (current == (signed) x)
            current--;
          
          if (current < 0)
            exist_more_tuples = false;
        }
		}

		if (has_no_support)
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			dx_size--;
			
			modif = true;
		}
		else 
			{
				if (record_support)
				{
					ds->Add_Support (num,x,v,arity,ts);
					ls->Change_Support (num,x,v,val,arity);
				}
				i++;
			}
  }
  while (i < dx_size);
  
  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);
  
  return modif;
}


void All_Different_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
	all_values.clear();
	
	for (unsigned int i = 0; i < arity; i++)
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
			all_values.insert(d->Get_Remaining_Real_Value(j));
	

		// we "remove" the selected candidate (we swap to avoid the reinitialization of candidates)
    int aux;
    aux = candidates[ind_min];
		candidates[ind_min] = candidates[i];
    candidates[i] = aux;
	
		if (all_values.size() == i+1)
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
	}
}


string All_Different_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<allDifferent> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</allDifferent>";
  return expr;
}
