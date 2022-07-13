#include "predicate_to_sum_rewriting_rule.h"
#include "predicate_constraint.h"
#include "equal_sum_global_constraint.h"
#include "not_equal_sum_global_constraint.h"
#include "less_sum_global_constraint.h"
#include "greater_sum_global_constraint.h"
#include "equal_weighted_sum_global_constraint.h"
#include "not_equal_weighted_sum_global_constraint.h"
#include "less_weighted_sum_global_constraint.h"
#include "greater_weighted_sum_global_constraint.h"

#include "expression_tools.h"

//-----------------
// basic functions
//-----------------


bool is_sum (Expression & e)
// returns true if e is a sum, false otherwise
{
	if ((e.Get_Type() == OP) && (e.Get_Operator() == ADD))
	{
		if (e.Get_Subexpression(0).Get_Type() == VAR)
		{
			if (e.Get_Subexpression(1).Get_Type() == VAR)
				return true;
			else return is_sum (e.Get_Subexpression(1));
		}
		else 
			if (e.Get_Subexpression(1).Get_Type() == VAR)
				return is_sum (e.Get_Subexpression(0));
			else return is_sum (e.Get_Subexpression(0)) && is_sum (e.Get_Subexpression(1));
	}
	else return false;
}
	

bool is_sum_comparison (Expression & e)
// returns true if e is a sum compared to a constant, false otherwise
{
	if ((e.Get_Type() == OP) && ((e.Get_Operator() == EQ) || (e.Get_Operator() == NE) || (e.Get_Operator() == LT) || (e.Get_Operator() == LE) || (e.Get_Operator() == GT) || (e.Get_Operator() == GE)))
	{
		if (e.Get_Subexpression(0).Get_Type() == VAL)
			return is_sum (e.Get_Subexpression(1));
		else
			if (e.Get_Subexpression(1).Get_Type() == VAL)
				return is_sum (e.Get_Subexpression(0));
			else return false;
	}
	else return false;
}


bool is_weighted_sum (Expression & e)
// returns true if e is a sum, false otherwise
{
	if ((e.Get_Type() == OP) && ((e.Get_Operator() == ADD) || (e.Get_Operator() == SUM)))
	{
		int arity = e.Get_Arity();
		bool ok = true;
		int i = 0;
		while ((i < arity) && (ok))
		{
			if (e.Get_Subexpression(i).Get_Type() == VAR)
				ok = true;
			else
				if (e.Get_Subexpression(i).Get_Type() == OP)
				{
					if (e.Get_Subexpression(i).Get_Operator() == MUL)
					{
						Expression p = e.Get_Subexpression(i);
						ok = ((p.Get_Subexpression(0).Get_Type() == VAR) && (p.Get_Subexpression(1).Get_Type() == VAL)) || ((p.Get_Subexpression(0).Get_Type() == VAL) && (p.Get_Subexpression(1).Get_Type() == VAR));
					}
					else 
						if (e.Get_Subexpression(i).Get_Operator() == NEG)
							ok = e.Get_Subexpression(i).Get_Subexpression(0).Get_Type() == VAR;
						else ok = is_weighted_sum (e.Get_Subexpression(i));
				}
				else ok = false;
			i++;
		}
		return ok;
	}
	else return false;
}

bool is_weighted_sum_comparison (Expression & e)
// returns true if e is a sum compared to a constant, false otherwise
{
	if ((e.Get_Type() == OP) && ((e.Get_Operator() == EQ) || (e.Get_Operator() == NE) || (e.Get_Operator() == LT) || (e.Get_Operator() == LE) || (e.Get_Operator() == GT) || (e.Get_Operator() == GE)))
	{
		if (e.Get_Subexpression(0).Get_Type() == VAL)
			return is_weighted_sum (e.Get_Subexpression(1));
		else
			if (e.Get_Subexpression(1).Get_Type() == VAL)
				return is_weighted_sum (e.Get_Subexpression(0));
			else return 0;
	}
	else return false;
}


bool is_particular_weighted_sum_comparison (Expression & e)
// returns true if e is a sum s.t. X0+cst compared to a variable, false otherwise
{
	if ((e.Get_Type() == OP) && ((e.Get_Operator() == EQ) || (e.Get_Operator() == NE) || (e.Get_Operator() == LT) || (e.Get_Operator() == LE) || (e.Get_Operator() == GT) || (e.Get_Operator() == GE)))
	{
		if ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == ADD) && (e.Get_Subexpression(1).Get_Type() == VAR) &&
				(((e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAL)) ||
				 ((e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAR))))
			return true;

		if ((e.Get_Subexpression(1).Get_Type() == OP) && (e.Get_Subexpression(1).Get_Operator() == ADD) && (e.Get_Subexpression(0).Get_Type() == VAR) &&
				(((e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL)) ||
				 ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAL) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAR))))
			return true;
	}
	return false;
}


bool is_particular_conditional_sum_comparison (Expression & e)
// returns true if e is a sum of Boolean conditions compared to a value, false otherwise
{
	if ((e.Get_Type() == OP) && ((e.Get_Operator() == EQ) || (e.Get_Operator() == NE) || (e.Get_Operator() == LT) || (e.Get_Operator() == LE) || (e.Get_Operator() == GT) || (e.Get_Operator() == GE)))
	{
		if ((e.Get_Subexpression(0).Get_Type() == OP) && ((e.Get_Subexpression(0).Get_Operator() == ADD) || (e.Get_Subexpression(0).Get_Operator() == SUM)) && (e.Get_Subexpression(1).Get_Type() == VAL))
    {
      int arity = e.Get_Subexpression(0).Get_Arity();
      for (int i = 0; i < arity; i++)
      {
        operator_type op = e.Get_Subexpression(0).Get_Subexpression(i).Get_Operator();
        if ((op != EQ) && (op != NE) && (op != LT) && (op != LE) && (op != GT) && (op != GE))
          return false;
        else
          if ((e.Get_Subexpression(0).Get_Subexpression(i).Get_Subexpression(0).Get_Type() != VAL) || (e.Get_Subexpression(0).Get_Subexpression(i).Get_Subexpression(1).Get_Type() != VAR))
            return false;
      }
			return true;
    }
	}
	return false;
}


void get_sum_weights (Expression & e, vector<int> & weights)
// returns the weight of each variable via the vector weights. We assume that the expression e is a weighted sum as defined in function is_weighted_sum
{
	if ((e.Get_Type() == OP) && ((e.Get_Operator() == ADD) || (e.Get_Operator() == SUM)))
	{
		int arity = e.Get_Arity();
		for (int i = 0; i < arity; i++)
		{
			if (e.Get_Subexpression(i).Get_Type() == VAR)
				weights[e.Get_Subexpression(i).Get_Variable()]++;
			else
				if (e.Get_Subexpression(i).Get_Type() == OP)
				{
					if (e.Get_Subexpression(i).Get_Operator() == MUL)
					{
						Expression p = e.Get_Subexpression(i);
						if (p.Get_Subexpression(0).Get_Type() == VAR)
							weights[p.Get_Subexpression(0).Get_Variable()] += p.Get_Subexpression(1).Get_Value();
						else weights[p.Get_Subexpression(1).Get_Variable()] += p.Get_Subexpression(0).Get_Value();
					}
					else 
						if (e.Get_Subexpression(i).Get_Operator() == NEG)
							weights[e.Get_Subexpression(i).Get_Subexpression(0).Get_Variable()]--;
						else get_sum_weights (e.Get_Subexpression(i),weights);
				}
		}
	}
}


Constraint * create_weighted_sum_constraint (Expression & e, vector<Variable *> & scope_var)
// create a new Weighted_Sum_Constraint constraint
{
	int arity = scope_var.size();
	Constraint * c;
	vector<int> weights (arity,0);
	int constant;

	if (e.Get_Subexpression(0).Get_Type() == VAL)
	{
		constant = e.Get_Subexpression(0).Get_Value();
		get_sum_weights (e.Get_Subexpression(1),weights);
	}
	else 
		{
			constant = e.Get_Subexpression(1).Get_Value();
			get_sum_weights (e.Get_Subexpression(0),weights);
		}
	
	switch (e.Get_Operator())
	{
		case EQ: c = new Equal_Weighted_Sum_Global_Constraint (scope_var,weights,constant); break;
		case NE: c = new Not_Equal_Weighted_Sum_Global_Constraint (scope_var,weights,constant); break;
		case LT: c = new Less_Weighted_Sum_Global_Constraint (scope_var,weights,true,constant); break;
		case LE: c = new Less_Weighted_Sum_Global_Constraint (scope_var,weights,false,constant); break;
		case GT: c = new Greater_Weighted_Sum_Global_Constraint (scope_var,weights,false,constant); break;
		case GE: c = new Greater_Weighted_Sum_Global_Constraint (scope_var,weights,true,constant); break;
		default: c = 0;  // requires for removing some warnings at compilation step
	}
	
	return c;
}


Expression replace_particular_weighted_sum (Expression & e)
// replaces the particular sum by a weighted_sum
{
	Expression subexpression [2];
	Expression add[2];
	Expression mult[2];
	mult[0] = Expression (VAL,-1);
	
	if ((e.Get_Subexpression(0).Get_Type() == OP) && (e.Get_Subexpression(0).Get_Operator() == ADD) && (e.Get_Subexpression(1).Get_Type() == VAR))
	{
		mult[1] = Expression (VAR,e.Get_Subexpression(1).Get_Variable());
		if ((e.Get_Subexpression(0).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(0).Get_Subexpression(1).Get_Type() == VAL))
		{
			add[0] = Expression (VAR,e.Get_Subexpression(0).Get_Subexpression(0).Get_Variable());
			subexpression[1] = Expression (VAL,-e.Get_Subexpression(0).Get_Subexpression(1).Get_Value());
		}
		else
			{
				add[0] = Expression (VAR,e.Get_Subexpression(0).Get_Subexpression(1).Get_Variable());
				subexpression[1] = Expression (VAL,-e.Get_Subexpression(0).Get_Subexpression(0).Get_Value());
			}			
	}
	else
		{
			add[0] = Expression (VAR,e.Get_Subexpression(0).Get_Variable());
			
			if ((e.Get_Subexpression(1).Get_Subexpression(0).Get_Type() == VAR) && (e.Get_Subexpression(1).Get_Subexpression(1).Get_Type() == VAL))
			{
				mult[1] = Expression (VAR,e.Get_Subexpression(1).Get_Subexpression(0).Get_Variable());	
				subexpression[1] = Expression (VAL,e.Get_Subexpression(1).Get_Subexpression(1).Get_Value());
			}
			else
				{
					mult[1] = Expression (VAR,e.Get_Subexpression(1).Get_Subexpression(1).Get_Variable());	
					subexpression[1] = Expression (VAL,e.Get_Subexpression(1).Get_Subexpression(0).Get_Value());
				}				
		}

	add[1] = Expression (MUL,mult,2);
	subexpression[0] = Expression (ADD,add);
	
	Expression e2 = Expression (e.Get_Operator(),subexpression,2);
	
	return e2;
}


Expression replace_sub (Expression & e)
// replaces the SUB operator by an equivalent ADD operator
{
	if (e.Get_Type() == OP)
	{
		operator_type op = e.Get_Operator();
		
		Expression subexpression [e.Get_Arity()];
		
		if (op == SUB)
		{
			subexpression[0] = replace_sub(e.Get_Subexpression(0));
			Expression subsubexpression [2];
			subsubexpression[0] = Expression (VAL,-1);
			subsubexpression[1] = replace_sub(e.Get_Subexpression(1));
			subexpression[1] = Expression (MUL,subsubexpression,2);
			return Expression (ADD,subexpression,2);
		}
		else
			{
				for (int i = 0; i < e.Get_Arity(); i++)
					subexpression[i] = replace_sub(e.Get_Subexpression(i));
				return Expression (op,subexpression,e.Get_Arity());
			}		
	}
	else return Expression (e);
}


bool Predicate_To_Sum_Rewriting_Rule::Rewrite (CSP * pb, int num)
// returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
{
  Constraint * c = pb->Get_Constraint(num);
  if (dynamic_cast<Predicate_Constraint *>(c) != 0)
  {
    Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();
  
    if (is_sum(e))
    {
      Expression e = dynamic_cast<Predicate_Constraint *>(c)->Get_Relation();
      
      int constant;
      if (e.Get_Subexpression(0).Get_Type() == VAL)
        constant = e.Get_Subexpression(0).Get_Value();
      else constant = e.Get_Subexpression(1).Get_Value();

      vector<Variable *> scope_var = c->Get_Scope_Var();
      Constraint * new_c;
      
      switch (e.Get_Operator())
      {
        case EQ: new_c = new Equal_Sum_Global_Constraint (scope_var,constant); break;
        case NE: new_c = new Not_Equal_Sum_Global_Constraint (scope_var,constant); break;
        case LT: new_c = new Less_Sum_Global_Constraint (scope_var,true,constant); break;
        case LE: new_c = new Less_Sum_Global_Constraint (scope_var,false,constant); break;
        case GT: new_c = new Greater_Sum_Global_Constraint (scope_var,true,constant); break;
        case GE: new_c = new Greater_Sum_Global_Constraint (scope_var,false,constant); break;
        default: new_c = 0;  // requires for removing some warnings at compilation step
      }
      
      pb->Replace_Constraint (num,new_c);

      return true;
    }
    else
      if (is_weighted_sum_comparison(e))
      {
        pb->Replace_Constraint (num,create_weighted_sum_constraint (e,c->Get_Scope_Var()));
        return true;
      }
      else
        if (is_particular_weighted_sum_comparison (e))
        {
          Expression e2 = replace_particular_weighted_sum (e);
          pb->Replace_Constraint (num,create_weighted_sum_constraint (e2,c->Get_Scope_Var()));
          return true;
        }
        else
          {
            Expression e2 = replace_sub (e);
            if (is_weighted_sum_comparison(e2))
            {
              pb->Replace_Constraint (num,create_weighted_sum_constraint (e2,c->Get_Scope_Var()));
              return true;
            }
            else 
              if (is_particular_conditional_sum_comparison (e))
              {
                int arity = e.Get_Subexpression(0).Get_Arity();
                vector<Variable *> scope_var_sum;
                for (int i = 0; i < arity; i++)
                {
                  string name = "c"+to_string(c->Get_Num())+"_"+to_string(i);
                  pb->Add_Variable (0,1,name, true); 
                  
                  scope_var_sum.push_back(pb->Get_Variable(pb->Get_N()-1));

                  string expr = "eq(" + name + ",";
                  
                  switch (e.Get_Subexpression(0).Get_Subexpression(i).Get_Operator())
                  {
                    case EQ: expr += "eq"; break;
                    case NE: expr += "ne"; break;
                    case LT: expr += "lt"; break;
                    case LE: expr += "le"; break;
                    case GT: expr += "gt"; break;
                    case GE: expr += "ge"; ; break;
                    default: ;  // requires for removing some warnings at compilation step
                  }
                  
                  expr += "(" +to_string(e.Get_Subexpression(0).Get_Subexpression(i).Get_Subexpression(0).Get_Value()) + "," + c->Get_Scope_Variable(e.Get_Subexpression(0).Get_Subexpression(i).Get_Subexpression(1).Get_Variable())->Get_Name() + "))"; 
                  
                  vector<Variable *> scope_var;
                  
                  Expression expression = string_to_expression (expr,pb,scope_var);
                  
                  pb->Add_Constraint (new Predicate_Constraint (scope_var,expression),false);
                }

                int constant = e.Get_Subexpression(1).Get_Value();
                
                Constraint * new_c;
                
                switch (e.Get_Operator())
                {
                  case EQ: new_c = new Equal_Sum_Global_Constraint (scope_var_sum,constant); break;
                  case NE: new_c = new Not_Equal_Sum_Global_Constraint (scope_var_sum,constant); break;
                  case LT: new_c = new Less_Sum_Global_Constraint (scope_var_sum,true,constant); break;
                  case LE: new_c = new Less_Sum_Global_Constraint (scope_var_sum,false,constant); break;
                  case GT: new_c = new Greater_Sum_Global_Constraint (scope_var_sum,true,constant); break;
                  case GE: new_c = new Greater_Sum_Global_Constraint (scope_var_sum,false,constant); break;
                  default: new_c = 0;  // requires for removing some warnings at compilation step
                }
                
                pb->Replace_Constraint (num,new_c);

                return true;
              }
            else return false;
          }
  }
  else return false;
}
