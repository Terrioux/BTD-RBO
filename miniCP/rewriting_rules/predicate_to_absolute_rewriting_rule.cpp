#include "predicate_to_absolute_rewriting_rule.h"
#include "predicate_constraint.h"
#include "absolute_equal_binary_constraint.h"
#include "absolute_not_equal_binary_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_binary_absolute_equal (Expression & e)
// returns true if e correspond to check whether two numbers are equal in absolute value
{
	if ((e.Get_Type() == OP) && (e.Get_Operator() == EQ))
	{
		return ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && 
						(e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR));
	}
	return false;	
}


bool is_binary_absolute_not_equal (Expression & e)
// returns true if e correspond to check whether two numbers are not equal in absolute value
{
	if ((e.Get_Type() == OP) && (e.Get_Operator() == NE))
	{
		return ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && 
						(e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR));
	}
	return false;
}


bool Predicate_To_Absolute_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_binary_absolute_equal(e))
    {
      pb->Replace_Constraint (num,new Absolute_Equal_Binary_Constraint (c->Get_Scope_Var()));
      return true;
    }
    else
      if (is_binary_absolute_not_equal (e))
      {
        pb->Replace_Constraint (num,new Absolute_Not_Equal_Binary_Constraint (c->Get_Scope_Var()));
        return true;
      }
      else return false;
  }
  else return false;
}
