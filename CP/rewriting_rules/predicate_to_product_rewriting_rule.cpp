#include "predicate_to_product_rewriting_rule.h"
#include "predicate_constraint.h"
#include "positive_product_binary_constraint.h"
#include "negative_product_binary_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_positive_binary_product (Expression & e)
// returns true if e corresponds to check whether the two numbers have a positive or zero product
{
	if (e.Get_Type() == OP)
	{
		if (e.Get_Operator() == GE)
			return (e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == MUL) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Type() == VAL) && (e.Get_Subexpression(1).Get_Value() == 0);
		else
			if (e.Get_Operator() == LE)
				return (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == MUL) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Value() == 0);
	}
	return false;
}


bool is_negative_binary_product (Expression & e)
// returns true if e corresponds to check whether the two numbers have a negative or zero product
{
	if (e.Get_Type() == OP)
	{
		if (e.Get_Operator() == LE)
			return (e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == MUL) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Type() == VAL) && (e.Get_Subexpression(1).Get_Value() == 0);
		else
			if (e.Get_Operator() == GE)
				return (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == MUL) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Value() == 0);
	}
	return false;
}


bool Predicate_To_Product_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_positive_binary_product (e))
    {
      pb->Replace_Constraint (num,new Positive_Product_Binary_Constraint (c->Get_Scope_Var()));
      return true;
    }
    else
      if (is_negative_binary_product (e))
      {
        pb->Replace_Constraint (num,new Negative_Product_Binary_Constraint (c->Get_Scope_Var()));
        return true;
      }
      else return false;
  }
  else return false;
}
