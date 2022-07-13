#include "predicate_to_equal_distance_rewriting_rule.h"
#include "predicate_constraint.h"
#include "equal_distance_binary_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_equal_distance (Expression & e)
// returns true if e correspond to check whether the distance between two numbers are equal to a constant
{
	if ((e.Get_Type() == OP) && (e.Get_Operator() == EQ))
	{
		return ((e.Get_Subexpression(0).Get_Type() == OP) && 
						(((e.Get_Subexpression(0).Get_Operator() == ABS) &&
							(e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == SUB) && 
							(e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) &&
							(e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR)) ||
						 ((e.Get_Subexpression(0).Get_Operator() == DIST) && 
							(e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))) &&
					  (e.Get_Subexpression(1).Get_Type() == VAL)) ||
					 ((e.Get_Subexpression(1).Get_Type() == OP) && 
						(((e.Get_Subexpression(1).Get_Operator() == ABS) &&
							(e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == SUB) && 
							(e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) &&
							(e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR)) ||
						 ((e.Get_Subexpression(1).Get_Operator() == DIST) &&
							(e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR))) &&
					  (e.Get_Subexpression(0).Get_Type() == VAL));
	}
	return false;	
}


bool Predicate_To_Equal_Distance_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_equal_distance (e))
    {
      int cst;
      if (e.Get_Subexpression(0).Get_Type () == VAL)
        cst = e.Get_Subexpression(0).Get_Value();
      else cst = e.Get_Subexpression(1).Get_Value();
      
      pb->Replace_Constraint (num,new Equal_Distance_Binary_Constraint (c->Get_Scope_Var(),cst));
      return true;
    }
    else return false;
  }
  else return false;
}
