#include "precedence_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Precedence_Global_Constraint::Precedence_Global_Constraint (vector<Variable *> & var, long value_s, long value_t): Global_Constraint (var)
// constructs a new precedence global constraint where value_s precedes value_t
{
  s = value_s;
  t = value_t;
}
		
		
Precedence_Global_Constraint::Precedence_Global_Constraint (Precedence_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
  s = c.s;
  t = c.t;
}


//-----------------
// basic functions
//-----------------


bool Precedence_Global_Constraint::Is_Satisfied (int * tt)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  int pos_s = -1;
  int pos_t = -1;
  
  for (unsigned int i = 0; (i < arity) && (pos_s <= pos_t); i++)
  {
    if ((pos_s == -1) && (scope_var[i]->Get_Domain()->Get_Real_Value(tt[i]) == s))
      pos_s = i;
    
    if ((pos_t == -1) && (scope_var[i]->Get_Domain()->Get_Real_Value(tt[i]) == t))
      pos_t = i;
  }

  if (pos_t == -1)
    return true;
  else
    return pos_s < pos_t;
}


void Precedence_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  // we first compute alpha
  unsigned long alpha = 0;

  while ((alpha < arity) && ((scope_var[alpha]->Get_Domain()->Get_Value(s) == -1) || (! scope_var[alpha]->Get_Domain()->Is_Present(scope_var[alpha]->Get_Domain()->Get_Value(s)))))
  {
    // we remove t from scope[alpha] (if it is present)
    ds->Add_Element (scope_var[alpha]);
    Domain * d = scope_var[alpha]->Get_Domain();
    int val = d->Get_Value(t);
    
    if ((val != -1) && (d->Is_Present(val)))
    {
      d->Filter_Value (val);
      
      if (d->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[alpha],this);
        return;
      }
    }

    alpha++;
  }
  
  if (alpha < arity)
  {
    // we remove t from scope[alpha] (if it is present)
    ds->Add_Element (scope_var[alpha]);
    Domain * d = scope_var[alpha]->Get_Domain();
    int val = d->Get_Value(t);
    
    if ((val != -1) && (d->Is_Present(val)))
    {
      d->Filter_Value (val);
      
      if (d->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[alpha],this);
        return;
      }
    }
    
    // we compute gamma
    unsigned long gamma = alpha + 1;
    
    while ((gamma < arity) && ((scope_var[gamma]->Get_Domain()->Get_Size() != 1) || (scope_var[gamma]->Get_Domain()->Get_Remaining_Real_Value(0) != t)))
      gamma++;

    // we compute beta
    unsigned long beta = alpha + 1;
    while ((beta < arity) && (scope_var[beta]->Get_Domain()->Get_Value(s) == -1))
      beta = beta + 1;
    
    if (gamma < beta)
    {
      // we assign scope[alpha] to s
      ds->Add_Element (scope_var[alpha]);
      Domain * d = scope_var[alpha]->Get_Domain();
      d->Assign (d->Get_Value(s));
      
      if (d->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[alpha],this);
        return;
      }
    }
  }
}


string Precedence_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<precedence>\n";

  expr += "  <list> ";
  
  for (unsigned int k = 0; k < arity; k++)
    expr +=  scope_var[k]->Get_Name() + " ";

  expr += "</list>\n";

  expr += "  <value> " + to_string(s) + " " + to_string(t) + " </value> ";
  
  expr += "</precedence>";
  return expr;
}
