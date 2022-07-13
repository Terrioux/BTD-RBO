#include "count_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Count_Global_Constraint::Count_Global_Constraint (vector<Variable *> & var, set<int> & value_set): Global_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that var[0]=|var[1...] in value_set|
{
  values = value_set;
}

Count_Global_Constraint::Count_Global_Constraint (Count_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  values = c.values;
}


//-----------------
// basic functions
//-----------------


bool Count_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  int nb = 0;
	for (unsigned int i = 1; i < arity; i++)
    if (values.find(scope_var[i]->Get_Domain()->Get_Real_Value (t[i])) != values.end())
      nb++;
    
	return scope_var[0]->Get_Domain()->Get_Real_Value (t[0]) == nb;
}


void Count_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  int count_min = 0;    // the minimum number of variables satisfying the condition of belonging
  int count_max = 0;    // the maximum number of variables satisfying the condition of belonging

  int nb_fixed_in = 0;  // the number of variables fixed with a value from values
  int nb_fixed_out = 0; // the number of variables fixed with a value out of values

	for (unsigned int i = 1; i < arity; i++)
	{
    Domain * d = scope_var[i]->Get_Domain();
    
		if (d->Get_Size() == 1)
    {
      if (values.find(d->Get_Remaining_Real_Value(0)) != values.end())
      {
        nb_fixed_in++;
        count_min++;
        count_max++;
      }
      else nb_fixed_out++;
    }
    else
      {
        int j = d->Get_Size()-1;
        while ((j >= 0) && (values.find(d->Get_Remaining_Real_Value(j)) != values.end()))
          j--;
        
        if (j < 0)      // all the values of d belong to values
        {
          count_min++;
          count_max++;
        }
        else
          if (j < (signed) d->Get_Size()-1)     // d and values share at least one value
            count_max++;
          else
            {
              j--;
              while ((j >= 0) && (values.find(d->Get_Remaining_Real_Value(j)) == values.end()))
                j--;
              
              if (j >= 0)     // d and values share at least one value
                count_max++;
            }
      }
  }
    
  ds->Add_Element (scope_var[0]);
  scope_var[0]->Get_Domain()->Filter_Value_Before(count_min-1);
  scope_var[0]->Get_Domain()->Filter_Value_From(count_max+1);

  if (scope_var[0]->Get_Domain()->Get_Size() == 0)
  {
    pb->Set_Conflict (scope_var[0],this);
    return;
  }
    
  if (scope_var[0]->Get_Domain()->Get_Size() == 1)
  {
    int goal = scope_var[0]->Get_Domain()->Get_Remaining_Real_Value(0);
    
    if (goal == nb_fixed_in)
    {
      // we cannot assign a value from values to a new variable
      // So we remove all the values which belong to values from the non-singleton domains
      for (unsigned int i = 1; i < arity; i++)
      {
        Domain * d = scope_var[i]->Get_Domain();
        if (d->Get_Size() > 1)
        {
          for (int j = d->Get_Size()-1; j >= 0; j--)
            if (values.find(d->Get_Remaining_Real_Value(j)) != values.end())
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
      if (goal > nb_fixed_in)
      {
        if (goal == (signed) arity - 1 - nb_fixed_out)
        {
          // each unfixed variable must take a value from values
          // so we remove the other values from the non-singleton domains
          for (unsigned int i = 1 ; i < arity; i++)
          {
            Domain * d = scope_var[i]->Get_Domain();
            if (d->Get_Size() > 1)
            {
              for (int j = d->Get_Size()-1; j >= 0; j--)
                if (values.find(d->Get_Remaining_Real_Value(j)) == values.end())
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
      }
      else
        {
          // there are more variables than allowed by goal
          throw ("PB");
          pb->Set_Conflict (scope_var[0],this);
        }
  }
}


string Count_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<count>\n";
  expr += "  <list> ";
  for (unsigned int i = 1; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <values> ";
  for (set<int>::iterator iter = values.begin(); iter != values.end(); iter++)
    expr += " " + to_string(*iter);
  expr += ") </condition>\n";
  expr += "  <condition> (eq,"+ scope_var[0]->Get_Name() +") </condition>\n";
  expr += "</count>";
  return expr;
}
