#include "ordered_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Ordered_Global_Constraint::Ordered_Global_Constraint (vector<Variable *> & var, vector<int> & len, bool equal): Global_Constraint (var)
// constructs a new ordered global constraint
{
  or_equal = equal;
  lengths = len;
}
		
		
Ordered_Global_Constraint::Ordered_Global_Constraint (Ordered_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
  or_equal = c.or_equal;
  lengths = c.lengths;
}


//-----------------
// basic functions
//-----------------


bool Ordered_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  int value1 = scope_var[0]->Get_Domain()->Get_Real_Value(t[0]);
  for (unsigned int i = 1; i < arity; i++)
  {
    int value2 = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]);
    if (! Compare(value1+lengths[i-1],value2))
      return false;
    value1 = value2;
  }
  return true;
}


string Ordered_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<ordered>\n";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</list>\n";
  
  bool all_null = true;
  for (vector<int>::iterator iter = lengths.begin(); (iter != lengths.end()) && (all_null); iter++)
    if (*iter != 0)
      all_null = false;
  
  if (! all_null)
  {
    expr += "<lengths>";
    for (vector<int>::iterator iter = lengths.begin(); iter != lengths.end(); iter++)
      expr +=  *iter + " ";
    expr += "</lengths>\n";
  }
  expr += "  <operator> " + Get_Relation() + " </operator>\n";
  expr += "</ordered>";
  return expr;
}
