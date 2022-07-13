#include "predicate_to_interval_rewriting_rule.h"
#include "predicate_constraint.h"
#include "diff_in_interval_binary_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_diff_in_interval (Expression & e)
// returns true if e corresponds to check whether a difference belongs to an interval, false otherwise// returns true if e corresponds to check whether a difference does not belong to an interval, false otherwise
{
	if ((e.Get_Type() == OP) && ((e.Get_Operator() == LE) || (e.Get_Operator() == LT)))
	{
		if (e.Get_Subexpression(0).Get_Type() == OP) 
		{
			return (((e.Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == SUB) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
							|| ((e.Get_Subexpression(0).Get_Operator() == DIST) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR)))
							&& (e.Get_Subexpression(1).Get_Type() == VAL);
		}
		else return false;
	}
	else
		if ((e.Get_Type() == OP) && ((e.Get_Operator() == GE) || (e.Get_Operator() == GT))) 
		{
			if (e.Get_Subexpression(1).Get_Type() == OP)
			{
				return (((e.Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == SUB) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
								|| ((e.Get_Subexpression(1).Get_Operator() == DIST) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR)))
								&& (e.Get_Subexpression(0).Get_Type() == VAL);
			}
			else return false;
		}
		else return false;
}



bool Predicate_To_Interval_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_diff_in_interval (e))
    {
      int cst;
      if (e.Get_Operator() == LE)
        cst = e.Get_Subexpression(1).Get_Value();
      else
        if (e.Get_Operator() == LT)
          cst = e.Get_Subexpression(1).Get_Value()-1;
      else 
        if (e.Get_Operator() == GE)
          cst = e.Get_Subexpression(0).Get_Value();
        else cst = e.Get_Subexpression(0).Get_Value()-1;

      pb->Replace_Constraint (num,new Diff_In_Interval_Binary_Constraint (c->Get_Scope_Var(),-cst,cst));
      return true;
    }
    else return false;
  }
  else return false;
}
