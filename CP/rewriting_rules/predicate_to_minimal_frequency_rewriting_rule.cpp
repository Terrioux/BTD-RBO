#include "predicate_to_minimal_frequency_rewriting_rule.h"
#include "predicate_constraint.h"
#include "minimal_frequency_difference_binary_constraint.h"
#include "positive_minimal_frequency_difference_binary_constraint.h"
#include "negative_minimal_frequency_difference_binary_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_positive_minimal_frequency_difference (Expression & e)
// returns true if e corresponds to check whether the two numbers have a positive product or their difference (absolute value) exceeds a given constant
{
	if ((e.Get_Type() == OP) && (e.Get_Operator() == OR))
	{
		return ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == GT) &&
					 (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == MUL) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Value() == 0) &&
					 (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == GE) &&
					 (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL) &&
           (((e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == ABS) && 
					   (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == SUB) &&
					   (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) &&
					   (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR)) ||
            ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == DIST) &&  
             (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) &&
					   (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR)))) ||
           ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == LE) &&
            (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Operator() == MUL) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Value() == 1) &&            
            (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == LE) &&
            (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAL) &&
            (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == DIST) &&  
            (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) &&
            (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR)) ||
           ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == LE) &&
            (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAL) &&
            (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Operator() == DIST) &&  
            (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) &&
            (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) &&
            (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == LE) &&
            (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == MUL) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Value() == 1));
	}
	return false;
}


bool is_negative_minimal_frequency_difference (Expression & e)
// returns true if e corresponds to check whether the two numbers have a negative product or their difference (absolute value) exceeds a given constant
{
	if ((e.Get_Type() == OP) && (e.Get_Operator() == OR))
	{
		return ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == LT) &&
					 (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == MUL) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Value() == 0) &&
					 (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == GE) &&
					 (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL) &&
					 (((e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == ABS) && 
						 (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == SUB) &&
						 (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR)) ||
						((e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == DIST) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR)))) ||
           ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == LE) &&
            (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == MUL) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Value() == -1) &&
            (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == LE) &&
            (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAL) &&
            (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == DIST) &&
            (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) &&
            (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR)) ||
           ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == LE) &&
            (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAL) &&
            (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Operator() == DIST) &&
            (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) &&
            (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR) &&
            (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == LE) &&
            (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == MUL) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Value() == -1));
	}
	return false;
}


bool is_minimal_frequency_difference (Expression & e)
// returns true if e corresponds to check whether the two numbers have a positive or negative product or their difference (absolute value) exceeds a given constant
{
	if (e.Get_Type() == OP)
	{
		if (e.Get_Operator() == AND)
			return (is_minimal_frequency_difference (e.Get_Subexpression(0))) && (is_minimal_frequency_difference (e.Get_Subexpression(1)));
		else
			if (e.Get_Operator() == OR)
				return (is_positive_minimal_frequency_difference (e)) || (is_negative_minimal_frequency_difference (e));
	}
	return false;
}


void get_frequency_bounds (Expression & e, int & cst1, int & cst2)
// returns the bounds for an expression satisfying the condition of is_minimal_frequency_difference
{
	if (e.Get_Type() == OP)
	{
		if (e.Get_Operator() == AND)
		{
			get_frequency_bounds (e.Get_Subexpression(0),cst1,cst2);
			get_frequency_bounds (e.Get_Subexpression(1),cst1,cst2);
		}
		else
			if (is_positive_minimal_frequency_difference (e))
			{
        int a;
        if (e.Get_Subexpression(1).Get_Operator() == GE)
          a = e.Get_Subexpression(1).Get_Subexpression(1).Get_Value();
        else a = e.Get_Subexpression(1).Get_Subexpression(0).Get_Value();

				if (a > cst1)
					cst1 = a;	
			}
			else
				if (is_negative_minimal_frequency_difference (e))
				{
          int a;
          if (e.Get_Subexpression(1).Get_Operator() == GE)
            a = e.Get_Subexpression(1).Get_Subexpression(1).Get_Value();
          else a = e.Get_Subexpression(1).Get_Subexpression(0).Get_Value();

					if (a > cst2)
						cst2 = a;	
				}
	}
}


bool Predicate_To_Minimal_Frequency_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_positive_minimal_frequency_difference (e))
    {
      pb->Replace_Constraint (num,new Positive_Minimal_Frequency_Difference_Binary_Constraint (c->Get_Scope_Var(),e.Get_Subexpression(1).Get_Subexpression(1).Get_Value()));
      return true;
    }
    else 
      if (is_negative_minimal_frequency_difference (e))
      {
        pb->Replace_Constraint (num,new Negative_Minimal_Frequency_Difference_Binary_Constraint (c->Get_Scope_Var(),e.Get_Subexpression(1).Get_Subexpression(1).Get_Value()));
        return true;
      }
      else
        if (is_minimal_frequency_difference (e))
        {
          int cst1 = 0, cst2 = 0;
          get_frequency_bounds (e, cst1, cst2);
                                            
          pb->Replace_Constraint (num,new Minimal_Frequency_Difference_Binary_Constraint (c->Get_Scope_Var(),cst1,cst2));
          return true;
        }
        else return false;
  }
  else return false;
}
