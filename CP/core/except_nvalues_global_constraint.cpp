#include "except_nvalues_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Except_NValues_Global_Constraint::Except_NValues_Global_Constraint (vector<Variable *> & var, int except_val): NValues_Global_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that var[0]=NValues(var[1...]) but without taking into account the value except_val
{
  exception_value = except_val;
  exception_values = new int [arity];
  for (unsigned int i = 1; i < arity; i++)
    exception_values[i] = scope_var[i]->Get_Domain()->Get_Value(exception_value);
}

Except_NValues_Global_Constraint::Except_NValues_Global_Constraint (Except_NValues_Global_Constraint & c): NValues_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  exception_value = c.exception_value;
  exception_values = new int [arity];
  for (unsigned int i = 1; i < arity; i++)
    exception_values[i] = scope_var[i]->Get_Domain()->Get_Value(exception_value);
}


Except_NValues_Global_Constraint::~Except_NValues_Global_Constraint ()
// destructor
{
  delete [] exception_values;
}

//-----------------
// basic functions
//-----------------


bool Except_NValues_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  set<int> values;
	for (unsigned int i = 1; i < arity; i++)
    values.insert(scope_var[i]->Get_Domain()->Get_Real_Value (t[i]));
  
  values.erase (exception_value);
  
	return scope_var[0]->Get_Domain()->Get_Real_Value (t[0]) == (signed) values.size();
}


void Except_NValues_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  all_values.clear();
  fixed_values.clear();
  int nb_fixed_variables = 0;   // the number of fixed variables

  // we compute the lower bound and the upper bound for the size of the set of values
  unsigned int nvalues_min;    // the lower bound
  unsigned int nvalues_max;    // the upper bound
  for (unsigned int i = 1; i < arity; i++)
  {
    Domain * d = scope_var[i]->Get_Domain();
    if (d->Get_Size() == 1)
    {
      int val = d->Get_Remaining_Real_Value(0);
      
      if (val != exception_value)
      {
        fixed_values.insert(val);
        all_values.insert(val);
      }
      nb_fixed_variables++;
    }
  }

  unsigned int nb_no_intersection = 0;    // the number of variables whose domain intersects the set fixed_values
  for (unsigned int i = 1; i < arity; i++)
  {
    Domain * d = scope_var[i]->Get_Domain();
    if (d->Get_Size() > 1)
    {
      bool no_intersection = true;
      for (unsigned int j = 0; j < d->Get_Size(); j++)
      {
        int val = d->Get_Remaining_Real_Value(j);
        if (val != exception_value)
        {
          all_values.insert(val);
          no_intersection = (no_intersection) && (fixed_values.find(val) == fixed_values.end());
        }
        else no_intersection = false;
      }
      if (no_intersection)
        nb_no_intersection++;
    }
  }

  nvalues_min = fixed_values.size();
  if (nb_no_intersection > 0)
     nvalues_min++;

  nvalues_max = fixed_values.size() + arity - 1 - nb_fixed_variables;
  
  if (all_values.size() < nvalues_max)
    nvalues_max = all_values.size();

  ds->Add_Element (scope_var[0]);
  scope_var[0]->Get_Domain()->Filter_Value_Before(nvalues_min-1);
  scope_var[0]->Get_Domain()->Filter_Value_From(nvalues_max+1);

  if (scope_var[0]->Get_Domain()->Get_Size() == 0)
  {
    pb->Set_Conflict (scope_var[0],this);
    return;
  }
  
  
  if (scope_var[0]->Get_Domain()->Get_Size() == 1)
  {
    int goal = scope_var[0]->Get_Domain()->Get_Remaining_Real_Value(0);
    
    fixed_values.erase (exception_value);
    
    if (goal == (signed) fixed_values.size())
    {
      // we cannot introduce a new value
      // So we remove all the values which do not belong to fixed_values from the non-singleton domains
      for (unsigned int i = 1; i < arity; i++)
      {
        Domain * d = scope_var[i]->Get_Domain();
        if (d->Get_Size() > 1)
        {
          for (int j = d->Get_Size()-1; j >= 0; j--)
            if (fixed_values.find(d->Get_Remaining_Real_Value(j)) == fixed_values.end())
            {
              ds->Add_Element (scope_var[i]);
              d->Filter_Value (d->Get_Remaining_Value(j));
            }
          
          if (d->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[i],this);
            return;
          }
        }
      }      
    }
    else
      if (goal > (signed) fixed_values.size())
      {
        if (goal - fixed_values.size() == arity - 1 - nb_fixed_variables)
        {
          // each unfixed variable must take a new value
          // so we remove the fixed values from the non-singleton domains
          for (unsigned int i = 1 ; i < arity; i++)
          {
            Domain * d = scope_var[i]->Get_Domain();
            if (d->Get_Size() > 1)
            {
              for (int j = d->Get_Size()-1; j >= 0; j--)
                if ((d->Get_Remaining_Real_Value(j) == exception_values[i]) || (fixed_values.find(d->Get_Remaining_Real_Value(j)) != fixed_values.end()))
                {
                  ds->Add_Element (scope_var[i]);
                  d->Filter_Value (d->Get_Remaining_Value(j));
                }
              
              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (scope_var[i],this);
                return;
              }
              else
                if (d->Get_Size() == 1)
                  fixed_values.insert(d->Get_Remaining_Real_Value(0));
            }
          }
        }
      }
      else
        {
          // there are more distinct fixed values than allowed by goal
          throw ("PB");
          pb->Set_Conflict (scope_var[0],this);
        }
  }
}


string Except_NValues_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<nValues>\n";
  expr += "  <list> ";
  for (unsigned int i = 1; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <except> " + to_string(exception_value) + " </except>\n";
  expr += "  <condition> (eq,"+ scope_var[0]->Get_Name() +") </condition>\n";
  expr += "</nValues>";
  return expr;
}
