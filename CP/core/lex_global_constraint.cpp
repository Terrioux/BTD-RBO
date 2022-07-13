#include "lex_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Lex_Global_Constraint::Lex_Global_Constraint (vector<Variable *> & var, bool equal): Global_Constraint (var)
// constructs a new lex global constraint
{
  or_equal = equal;
  tuple_size = var.size()/2;
}
		
		
Lex_Global_Constraint::Lex_Global_Constraint (Lex_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
  or_equal = c.or_equal;
  tuple_size = c.tuple_size;
}


//-----------------
// basic functions
//-----------------


bool Lex_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  unsigned int i = 0;
  unsigned int j = tuple_size;
  
  while ((i < tuple_size) && (scope_var[i]->Get_Domain()->Get_Real_Value(t[i]) == scope_var[j]->Get_Domain()->Get_Real_Value(t[j])))
  {
    i++;
    j++;
  }
  
  if (i == tuple_size)
    return or_equal;
  else
    return scope_var[i]->Get_Domain()->Get_Real_Value(t[i]) < scope_var[j]->Get_Domain()->Get_Real_Value(t[j]);
}


void Lex_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  // we first compute alpha and beta
  unsigned int alpha = 0;

  unsigned int i = 0;
  unsigned int j = tuple_size;
    
  while ((alpha < tuple_size) && (scope_var[i]->Get_Domain()->Get_Size() == 1) && (scope_var[j]->Get_Domain()->Get_Size() == 1) && (scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0) == scope_var[j]->Get_Domain()->Get_Remaining_Real_Value(0)))
  {
    i++;
    j++;
    alpha++;
  }

  if (alpha == tuple_size)
  {
    // the two vectors are equal
    if (! or_equal)
    {
      // the check fails
      if (scope_var[i]->Get_Domain()->Get_Size() > 1)
          pb->Set_Conflict (scope_var[i],this);
      else
        pb->Set_Conflict (scope_var[j],this);
    }
    return;
  }
  else
    {
      unsigned int index = alpha;
      int beta = -1;
      
      while ((index < tuple_size) && ((scope_var[i]->Get_Domain()->Get_Real_Min() < scope_var[j]->Get_Domain()->Get_Real_Max()) || ((or_equal) && (scope_var[i]->Get_Domain()->Get_Real_Min() == scope_var[j]->Get_Domain()->Get_Real_Max()))))
      {
        if (scope_var[i]->Get_Domain()->Get_Real_Min() == scope_var[j]->Get_Domain()->Get_Real_Max())
        {
          if (beta == -1)
            beta = i;
        }
        else beta = -1;
      
        index++;
        i++;
        j++;
      }
      
      if (index == tuple_size)
        beta = tuple_size + 2;
      else
        if (beta == -1)
          beta = index;
          
      if ((signed) alpha >= beta)
      {
        // the check fails
        if (scope_var[i]->Get_Domain()->Get_Size() > 1)
          pb->Set_Conflict (scope_var[i],this);
        else
          pb->Set_Conflict (scope_var[j],this);
        return;
      }

      // we look for inconsistent values
      
      i = alpha;
      j = alpha + tuple_size;

      bool stop;
      do
      {
        stop = true;
        
        // we enforce the condition x_i < x_j (or x_i <= x_j depending on whether index+1 is equal to beta)
        
        Domain * d_i = scope_var[i]->Get_Domain();
        Domain * d_j = scope_var[j]->Get_Domain();
        
        int ref_i = d_j->Get_Real_Max();
        int ref_j = d_i->Get_Real_Min();

        if ((signed) alpha + 1 < beta)
        {
          ref_i++;
          ref_j--;
        }
        
        // we filter the current variable of the first vector
        unsigned old_size = d_i->Get_Size();
        
        ds->Add_Element (scope_var[i]);
        d_i->Filter_Value_From (ref_i);

        if (d_i->Get_Size() != old_size)
        {
          if (d_i->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[i],this);
            return;
          }
        }
        
        // we filter the current variable of the second vector
        old_size = d_j->Get_Size();
        
        ds->Add_Element (scope_var[j]);
        d_j->Filter_Value_Before (ref_j);

        if (d_j->Get_Size() != old_size)
        {
          if (d_j->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[j],this);
            return;
          }
        }

        if ((signed) alpha + 1 < beta)
        {
          // we try to update alpha
          index = alpha;
          while ((alpha < tuple_size) && ((signed) index < beta) && (scope_var[i]->Get_Domain()->Get_Size() == 1) && (scope_var[j]->Get_Domain()->Get_Size() == 1) && (scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0) == scope_var[j]->Get_Domain()->Get_Remaining_Real_Value(0)))
          {
            alpha++;
            i++;
            j++;
          }
                        
          if (alpha == tuple_size)
            return;
          else
              if ((signed) alpha == beta)
              {
                // the check fails
                if (scope_var[i]->Get_Domain()->Get_Size() > 1)
                  pb->Set_Conflict (scope_var[i],this);
                else
                  pb->Set_Conflict (scope_var[j],this);
                return;
              }
              else 
                {
                  if (alpha != index)
                    stop = false;
                  else stop = true;
                }
        }
        else stop = true;
      }
      while (! stop);
    }  
}


string Lex_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<lex>\n";

  unsigned int i = 0;
  do
  {
    expr += "  <list> ";
    
    for (unsigned int k = 0; k < tuple_size; k++)
    {
      expr +=  scope_var[i]->Get_Name() + " ";
      i++;
    }
    expr += "</list>\n";
  }
  while (i < arity);
  
  expr += "  <operator> ";
  if (or_equal) 
    expr += "le";
  else expr += "lt";
  expr += " </operator>\n";
  expr += "</lex>";
  return expr;
}
