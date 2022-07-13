#include "cardinality_global_constraint.h"
#include <algorithm>

//--------------
// constructors 
//--------------


Cardinality_Global_Constraint::Cardinality_Global_Constraint (vector<Variable *> & var, vector<int> & val, bool is_closed): Global_Constraint (var)
// constructs a cardinality global constraint 
{
  closed = is_closed;
  values = val;
}
		
		
Cardinality_Global_Constraint::Cardinality_Global_Constraint (Cardinality_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
  closed = c.closed;
  values = c.values;
}


//-----------------
// basic functions
//-----------------


bool Cardinality_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  if (closed)
  {
    // we check whether each value belongs to values
    for (unsigned int i = 0; i < arity-values.size(); i++)
      if (find (values.begin(), values.end(), scope_var[i]->Get_Domain()->Get_Real_Value(t[i])) == values.end())
        return false;
  }
  
  for (unsigned int j = 0; j < values.size(); j++)
  {
    // we check whether the cardinality holds
    int nb = 0;
    for (unsigned int i = 0; i < arity-values.size(); i++)
      if (scope_var[i]->Get_Domain()->Get_Real_Value(t[i]) == values[j])
        nb++;

    int k = arity-values.size()+j;
    if (nb != scope_var[k]->Get_Domain()->Get_Real_Value(t[k]))
      return false;
  }

  return true;
}


void Cardinality_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  if ((ref == 0) && (closed))
  {
    // we remove all the values which do not appear in values
    for (unsigned int i = 0; i < arity-values.size(); i++)
    {
      Domain * d = scope_var[i]->Get_Domain();
      for (int j = d->Get_Size()-1; j >= 0; j--)
        if (find (values.begin(), values.end(), d->Get_Remaining_Real_Value (j)) == values.end())
        {
          ds->Add_Element (scope_var[i]);
          d->Filter_Value (d->Get_Remaining_Value(j));
        }

      if (d->Get_Size () == 0)
      {
        pb->Set_Conflict (scope_var[i],this);
        return;
      }
    }
  }
    
  bool again;
  do
  {
    again = false;
    
    // we adjust the domain of the cardinality variables
    int nb_fixed [values.size()];     // the number of domains which are fixed to a given value from values
    int nb_contain [values.size()];   // the number of domains which contain a given value from values and have a size at least two
    for (unsigned int i = 0; i < values.size(); i++)
    {
      nb_fixed[i] = 0;
      nb_contain[i] = 0;
      
      for (unsigned int j = 0; j < arity-values.size(); j++)
      {
        Domain * d = scope_var[j]->Get_Domain();
        if (d->Get_Size() == 1) 
        {
          if (d->Get_Remaining_Real_Value(0) == values[i])
            nb_fixed[i]++;
        }
        else
          {
            int v = d->Get_Value(values[i]);
            if ((v != -1) && (d->Is_Present(v)))
              nb_contain[i]++;
          }
      }
      
      int j = arity - values.size() + i;
      ds->Add_Element (scope_var[j]);
      
      unsigned int old_size = scope_var[j]->Get_Domain()->Get_Size ();

      scope_var[j]->Get_Domain()->Filter_Value_Before(nb_fixed[i]-1);
      scope_var[j]->Get_Domain()->Filter_Value_From(nb_fixed[i]+nb_contain[i]+1);

      if (old_size != scope_var[j]->Get_Domain()->Get_Size ())
        again = true;

      if (scope_var[j]->Get_Domain()->Get_Size () == 0)
      {
        pb->Set_Conflict (scope_var[j],this);
        return;
      }
    }
    
    unsigned int min_sum = 0;
    for (unsigned int i = 0; i < values.size(); i++)
      min_sum += scope_var[arity - values.size() + i]->Get_Domain()->Get_Real_Min();

    if (min_sum == arity - values.size())
    {
      for (unsigned int i = 0; i < values.size(); i++)
      {
        Domain * d = scope_var[arity - values.size() + i]->Get_Domain();
        if (d->Get_Size() > 1)
        {
          ds->Add_Element (scope_var[arity - values.size() + i]);
          d->Assign (d->Get_Min());
        }
      }
    }
    
    for (unsigned int i = 0; i < values.size(); i++)
    {
      int j = arity - values.size() + i;

      if (scope_var[j]->Get_Domain()->Get_Size() == 1)
      {
        int k = scope_var[j]->Get_Domain()->Get_Remaining_Real_Value(0);
        
        if (nb_fixed[i] == k)
        {
          // we remove the value values[i] from non-singleton domains
          for (unsigned int l = 0; l < arity-values.size(); l++)
          {
            Domain * d = scope_var[l]->Get_Domain();
            if (d->Get_Size() > 1)
            {
              int v = d->Get_Value(values[i]);
              if ((v != -1) && (d->Is_Present(v)))
              {
                ds->Add_Element (scope_var[l]);
                d->Filter_Value (v);
                again = true;
              }
            }
          }
        }
        else
          if (nb_fixed[i] < k)
          {
            if (k - nb_fixed[i] == nb_contain[i])
            {
              // we fix the variables which contain the value values[i] and which have non-singleton domain
              for (unsigned int l = 0; l < arity-values.size(); l++)
              {
                Domain * d = scope_var[l]->Get_Domain();
                if (d->Get_Size() > 1)
                {
                  int v = d->Get_Value(values[i]);
                  
                  if ((v != -1) && (d->Is_Present(v)))
                  {
                    ds->Add_Element (scope_var[l]);
                    d->Assign (v);
                    again = true;
                  }
                }
              }
            }
          }
      }
    }
  }
  while (again);
}


string Cardinality_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<cardinality>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity-values.size(); i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <values> ";
  for (unsigned int i = 0; i < values.size(); i++)
    expr +=  to_string (values[i]) + " ";
  expr += "</values>\n";
  expr += "  <occurs> ";
  for (unsigned int i = arity-values.size(); i < arity; i++)
    if (scope_var[i]->Get_Domain()->Get_Initial_Size() == 1)
      expr += to_string (scope_var[i]->Get_Domain()->Get_Real_Value(0)) + " ";
    else expr += to_string (scope_var[i]->Get_Domain()->Get_Real_Value(0)) + ".." + to_string (scope_var[i]->Get_Domain()->Get_Real_Value(scope_var[i]->Get_Domain()->Get_Initial_Size()-1)) + " ";
  expr += "</occurs>\n";
  expr += "</cardinality>";
  return expr;
}
