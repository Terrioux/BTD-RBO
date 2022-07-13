#include "predicate_to_no_overlap_rewriting_rule.h"
#include "predicate_constraint.h"
#include "no_overlap_global_constraint.h"


//-----------------
// basic functions
//-----------------


bool is_diff_out_interval (Expression & e)
// returns true if e corresponds to check whether a difference does not belong to an interval, false otherwise
{
	if (e.Get_Type() == OP)
	{
		if (e.Get_Operator() == OR)
		{
			if ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == LE) && (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == LE))
			{
				if ((e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == ADD) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
				{
					if (((e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() != VAR) || (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() != VAL)) &&
							((e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() != VAL) || (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() != VAR)))
						return false;
				}
				else
					if ((e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Operator() == ADD) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR))
					{
						if (((e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() != VAR) || (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Type() != VAL)) &&
								((e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() != VAL) || (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Type() != VAR)))
							return false;
					}
					
				if ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == ADD) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR))
				{
					if (((e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() != VAR) || (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() != VAL)) &&
							((e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() != VAL) || (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() != VAR)))
						return false;
				}
				else
					if ((e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == ADD) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR))
					{
						if (((e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() != VAR) || (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() != VAL)) &&
								((e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() != VAL) || (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() != VAR)))
							return false;
					}
				return true;
			}
			else return false;
		}
		else
			if ((e.Get_Operator() == GT) || (e.Get_Operator() == GE))
			{
				if (e.Get_Subexpression(0).Get_Type() == OP)
				{
					return  (((e.Get_Subexpression(0).Get_Operator() == ABS) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == SUB) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
									|| ((e.Get_Subexpression(0).Get_Operator() == DIST) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR)))
									&& (e.Get_Subexpression(1).Get_Type() == VAL);
				}
				else return false;
			}
			else
				if ((e.Get_Operator() == LT) || (e.Get_Operator() == LE))
				{
					if (e.Get_Subexpression(1).Get_Type() == OP)
					{
						return  (((e.Get_Subexpression(1).Get_Operator() == ABS) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == SUB) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
										|| ((e.Get_Subexpression(1).Get_Operator() == DIST) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR)))
										&& (e.Get_Subexpression(0).Get_Type() == VAL);
					}
					else return false;
				}
				else return false;
	}
	else return false;
}


void Get_Lengths (Expression & e, vector<int> & lengths)
// returns the length vector for an expression e representing a binary no-overlap global constraint
{
	if (e.Get_Type() == OP) 
	{
		if (e.Get_Operator() == OR)
		{
			if ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == LE) && (e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == LE))
			{
				if ((e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Operator() == ADD) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
				{
					if ((e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAL))
					{
						if (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Variable() == 0)
							lengths[0] = e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Value();
						else lengths[1] = e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Value();
					}
					else
						if ((e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
						{
							if (e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(1).Get_Variable() == 0)
								lengths[0] = e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Value();
							else lengths[1] = e.Get_Subexpression(0).Get_Subexpression(0).Get_Subexpression(0).Get_Value();
						}
				}
				else
					if ((e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Operator() == ADD) && (e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR))
					{
						if ((e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL))
						{
							if (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Variable() == 0)
								lengths[1] = - e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Value();
							else lengths[0] = - e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Value();
						}
						else
							if ((e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR))
							{
								if (e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(1).Get_Variable() == 0)
									lengths[1] = - e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Value();
								else lengths[0] = e.Get_Subexpression(0).Get_Subexpression(1).Get_Subexpression(0).Get_Value();
							}
					}
					
				if ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Operator() == ADD) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR))
				{
					if ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAL))
					{
						if (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Variable() == 0)
							lengths[0] = e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Value();
						else lengths[1] = e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Value();
					}
					else
						if ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))
						{
							if (e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(1).Get_Variable() == 0)
								lengths[0] = e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Value();
							else lengths[1] = e.Get_Subexpression(1).Get_Subexpression(0).Get_Subexpression(0).Get_Value();						
						}
				}
				else
					if ((e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Operator() == ADD) && (e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR))
					{
						if ((e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL))
						{
							if (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Variable() == 0)
								lengths[1] = - e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Value();
							else lengths[0] = - e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Value();
						}
						else
							if ((e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAL) || (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR))
							{
								if (e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(1).Get_Variable() == 0)
									lengths[1] = - e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Value();
								else lengths[0] = - e.Get_Subexpression(1).Get_Subexpression(1).Get_Subexpression(0).Get_Value();
							}
					}
			}
		}
		else
			if (e.Get_Operator() == GT)
			{
				lengths[0] = e.Get_Subexpression(1).Get_Value() + 1;
				lengths[1] = lengths[0];
			}
			else
				if (e.Get_Operator() == GE)
				{
					lengths[0] = e.Get_Subexpression(1).Get_Value();
					lengths[1] = lengths[0];
				}
				else
					if (e.Get_Operator() == LT)
					{
						lengths[0] = e.Get_Subexpression(0).Get_Value() + 1;
						lengths[1] = lengths[0];
					}
					else
						if (e.Get_Operator() == LE)
						{
							lengths[0] = e.Get_Subexpression(0).Get_Value();
							lengths[1] = lengths[0];
						}
	}
}


bool Predicate_To_No_Overlap_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  
  if ((dynamic_cast<Predicate_Constraint *>(c) != 0) && (c->Get_Arity() == 2))
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();

    if (is_diff_out_interval(e))
    {
      vector<int> lengths (2);
      Get_Lengths (e,lengths);
      pb->Replace_Constraint (num,new No_Overlap_Global_Constraint(c->Get_Scope_Var(), lengths));
      return true;
    }
    else return false;
  }
  else return false;
}
