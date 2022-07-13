#include "predicate_to_binary_sum_rewriting_rule.h"
#include "predicate_constraint.h"
#include "diff_and_greater_equal_sum_binary_constraint.h"
#include "conditional_equal_sum_global_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_not_equal_greater_equal_sum (Expression & e)
// returns true if e correspond to check whether a sum between two different elements is greater or equal to a constant
{
	if ((e.Get_Type() == OP) && (e.Get_Operator() == AND))
	{
		return ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == NE) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR)) &&
		       ((e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == GE) && 
		        (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == ADD) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) &&
		        (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL));
	}
	return false;
}


bool is_conditional_sum (Expression & e)
// returns true if e correspond to a conditional sum, false otherwise
{
	if (e.Get_Type() == OP) 
	{
		if (e.Get_Operator() == ADD)
		{
			if (e.Get_Subexpression(0).Get_Type() == OP)
			{
				if (e.Get_Subexpression(0).Get_Operator() == ADD)
				{
					if (! is_conditional_sum (e.Get_Subexpression(0)))
						return false;
				}
				else
					if (e.Get_Subexpression(0).Get_Operator() == IF)
					{
						if ((e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() != OP) || (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() != GE) || (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() != VAR) || (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() != VAL) || (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() != VAL) || (e.Get_Subexpression(0).Get_Subexpression(2).Get_Type() != VAL))
							return false;
					}
					else return false;
			}
			if (e.Get_Subexpression(1).Get_Type() == OP)
			{
				if (e.Get_Subexpression(1).Get_Operator() == ADD)
				{
					if (! is_conditional_sum (e.Get_Subexpression(1)))
						return false;
				}
				else
					if (e.Get_Subexpression(1).Get_Operator() == IF)
					{
						if ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() != OP) || (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() != GE) || (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() != VAR) || (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() != VAL) || (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() != VAL) || (e.Get_Subexpression(1).Get_Subexpression(2).Get_Type() != VAL))
							return false;
					}
					else return false;
			}
					
			return true;
		}
		else return false;
	}
	else return false;
}


bool is_equal_conditional_sum (Expression & e)
// returns true if e correspond to an equal conditional sum, false otherwise
{
	return (e.Get_Type() == OP) && (e.Get_Operator() == EQ) && (is_conditional_sum(e.Get_Subexpression(0))) && (e.Get_Subexpression(1).Get_Type() == VAL);
}


void get_conditional_sum_constants (Expression & e, int constant_var [])
// returns the constant related to each variable
{
	if (e.Get_Type() == OP) 
	{
		if (e.Get_Operator() == ADD)
		{
			get_conditional_sum_constants (e.Get_Subexpression(0), constant_var);
			get_conditional_sum_constants (e.Get_Subexpression(1), constant_var);
		}
		else
			if (e.Get_Operator() == IF)
				constant_var[e.Get_Subexpression(0).Get_Subexpression(0).Get_Variable()] =  e.Get_Subexpression(0).Get_Subexpression(1).Get_Value();
	}
}


bool Predicate_To_Binary_Sum_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_not_equal_greater_equal_sum(e))
    {
      pb->Replace_Constraint (num,new Diff_And_Greater_Equal_Sum_Binary_Constraint (c->Get_Scope_Var(),e.Get_Subexpression(1).Get_Subexpression(1).Get_Value()));
      return true;
    }
    else
      if (is_equal_conditional_sum (e))
      {
        int constant_var [c->Get_Arity()];
        get_conditional_sum_constants (e.Get_Subexpression(0),constant_var);
        pb->Replace_Constraint (num,new Conditional_Equal_Sum_Global_Constraint (c->Get_Scope_Var(),e.Get_Subexpression(1).Get_Value(),constant_var));
        return true;
      }
      else return false;
  }
  else return false;
}
