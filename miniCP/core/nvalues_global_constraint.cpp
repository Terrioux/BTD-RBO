#include "nvalues_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------

NValues_Global_Constraint::NValues_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that var[0]=NValues(var[1...])
{
}

NValues_Global_Constraint::NValues_Global_Constraint (NValues_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


NValues_Global_Constraint::~NValues_Global_Constraint ()
// destructor
{
}


//-----------------
// basic functions
//-----------------


bool NValues_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  set<int> values;
	for (unsigned int i = 1; i < arity; i++)
    values.insert(scope_var[i]->Get_Domain()->Get_Real_Value (t[i]));
    
	return scope_var[0]->Get_Domain()->Get_Real_Value (t[0]) == (signed) values.size();
}


void NValues_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
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
      fixed_values.insert(d->Get_Remaining_Real_Value(0));
      all_values.insert(d->Get_Remaining_Real_Value(0));
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
        all_values.insert(val);
        
        no_intersection = (no_intersection) && (fixed_values.find(val) == fixed_values.end());
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
  
  ds->Add_Element(scope_var[0]);
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
                if (fixed_values.find(d->Get_Remaining_Real_Value(j)) != fixed_values.end())
                {
                  ds->Add_Element (scope_var[i]);
                  d->Filter_Value (d->Get_Remaining_Value(j));
                }
              
              if (d->Get_Size() == 0)
              {
                //~ cout << "Vide " << scope_var[i]->Get_Name() << endl;
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


string NValues_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<nValues>\n";
  expr += "  <list> ";
  for (unsigned int i = 1; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <condition> (eq,"+ scope_var[0]->Get_Name() +") </condition>\n";
  expr += "</nValues>";
  return expr;
}
