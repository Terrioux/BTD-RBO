#include "predicate_to_all_different_rewriting_rule.h"
#include "predicate_constraint.h"
#include "all_different_global_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_binary_not_equal (Expression & e)
// returns true if e corresponds to check whether two variables are not equal, false otherwise
{
	return (e.Get_Type() == OP) && 
				(((e.Get_Operator() == NE) && (e.Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Type() == VAR)) ||
         ((e.Get_Operator() == NE) && (e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == SUB) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Type() == VAL) && (e.Get_Subexpression(1).Get_Value() == 0)) ||
         ((e.Get_Operator() == NE) && (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == SUB) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Value() == 0)) ||
         ((e.Get_Operator() == GT) && (e.Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Value() == 0) && 
					(e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == MUL) &&
					(e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == SUB) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR) &&
					(e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == SUB) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR) &&
					(e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Variable() == e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Variable()) &&
					(e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Variable() == e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Variable())));
}


bool Predicate_To_All_Different_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_binary_not_equal(e))
    {
      pb->Replace_Constraint (num,new All_Different_Global_Constraint (c->Get_Scope_Var()));
      return true;
    }
    else return false;
  }
  else return false;
}
