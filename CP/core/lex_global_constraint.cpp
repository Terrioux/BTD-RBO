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
  // we compute the number of assigned variables
  unsigned int nb_assigned = 0;
  for (unsigned int i = 0; i < arity; i++)
    if (scope_var[i]->Get_Domain()->Get_Size() == 1)
      nb_assigned++;

  // we compute the first index from which the two vectors differ
  unsigned int alpha = 0;

  while ((alpha < tuple_size - 1) && (scope_var[alpha]->Get_Domain()->Get_Size() == 1) && (scope_var[alpha+tuple_size]->Get_Domain()->Get_Size() == 1) && (scope_var[alpha]->Get_Domain()->Get_Remaining_Real_Value(0) == scope_var[alpha+tuple_size]->Get_Domain()->Get_Remaining_Real_Value(0)))
    alpha++;
  
  unsigned int alpha0;
  do
  {
    alpha0 = alpha;
    
    Domain * d1 = scope_var[alpha]->Get_Domain();
    Domain * d2 = scope_var[alpha+tuple_size]->Get_Domain();
    
    bool modif = false;
    
    long int min = d1->Get_Real_Min();
      
    if (min > d2->Get_Real_Min())
    {
      modif = true;
      min--;
    }
    else
      if ((min == d2->Get_Real_Min()) && (! or_equal))
      {
        // we check whether the next variables 
        unsigned int beta = alpha+1;
        while ((beta < tuple_size) && (scope_var[beta]->Get_Domain()->Get_Real_Min() >= scope_var[beta+tuple_size]->Get_Domain()->Get_Real_Max()))
          beta++;
          
        if (beta == tuple_size) 
        {
          modif = true;
          
        }
      }
      
    if (modif)
    {
      unsigned int old_size = d2->Get_Size();

      ds->Add_Element (scope_var[alpha+tuple_size]);
      d2->Filter_Value_Before (min);

      if (d2->Get_Size() != old_size)
      {
        if (d2->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[alpha+tuple_size],this);
          return;
        }
      }
    }
    
    modif = false;
    
    long int max = d2->Get_Real_Max();
      
    if (d1->Get_Real_Max() > max)
    {
      modif = true;
      max++;
    }
    else
      if ((max == d2->Get_Real_Min()) && (! or_equal))
      {
        // we check whether the next variables 
        unsigned int beta = alpha+1;
        while ((beta < tuple_size) && (scope_var[beta]->Get_Domain()->Get_Real_Min() >= scope_var[beta+tuple_size]->Get_Domain()->Get_Real_Max()))
          beta++;
          
        if (beta == tuple_size) 
        {
          modif = true;
          
        }
      }
      
    if (modif)
    {
      unsigned int old_size = d1->Get_Size();

      ds->Add_Element (scope_var[alpha]);
      d1->Filter_Value_From (max);

      if (d1->Get_Size() != old_size)
      {
        if (d1->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[alpha],this);
          return;
        }
      }
    }
    
    while ((alpha < tuple_size) && (scope_var[alpha]->Get_Domain()->Get_Size() == 1) && (scope_var[alpha+tuple_size]->Get_Domain()->Get_Size() == 1) && (scope_var[alpha]->Get_Domain()->Get_Remaining_Real_Value(0) == scope_var[alpha+tuple_size]->Get_Domain()->Get_Remaining_Real_Value(0)))
      alpha++;
  }
  while ((alpha < tuple_size) && (alpha0 != alpha));
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
