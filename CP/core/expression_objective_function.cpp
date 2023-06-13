#include "expression_objective_function.h"
#include "cop.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Expression_Objective_Function::Expression_Objective_Function (Objective obj, vector<Variable *> obj_scope, Expression expr): Objective_Function(obj, obj_scope)
// constructs an objective function involving the objective obj and the expression expr based on the variables of obj_scope
{
  if (obj == MAXIMIZE)
    expression = Expression (NEG,&expr);
  else
    expression = expr;
  criterion_information = "expression "+to_string(scope.size());
}


//-----------------
// basic functions
//-----------------


void Expression_Objective_Function::Update_Problem (COP * pb)
// updates the problem by adding some variables or constraints to the problem to handle the objective function
{
  if (! problem_updated)
  {
    problem_updated = true;
    
    // we create the constraint related to the lower and upper bounds    
    Expression subexpr [3];
    subexpr[0] = expression;
    subexpr[1] = Expression(VAR,scope.size());
    subexpr[2] = Expression(VAR,scope.size()+1);

    vector<Variable *> scope_expr;
    scope_expr = scope;
    scope_expr.push_back(pb->Get_Lower_Bound_Variable());
    scope_expr.push_back(pb->Get_Upper_Bound_Variable());
    
    Expression e = Expression (INI, subexpr,3);
    bound_constraint = new Predicate_Constraint(scope_expr,e);
    pb->Add_Constraint(bound_constraint,false);
    
    
  }
}


long Expression_Objective_Function::Get_Cost (Assignment & A)
// returns the value of the objective function for the assignment A, or raises an exception if all the involved variables are not assigned
{
  int t [scope.size()];
  
  for (unsigned int i = 0; i < scope.size(); i++)
  {
    unsigned int num = scope[i]->Get_Num();
    if (A.Is_Assigned(num))
      t[i] = scope[i]->Get_Domain()->Get_Real_Value(A.Get_Value(num));
    else
      if (scope[i]->Get_Domain()->Get_Size() == 1)
        t[i] = scope[i]->Get_Domain()->Get_Remaining_Real_Value(0);
      else
        throw ("Error: a variable is not assigned when computing the cost of the objective function");
  }
  
  return expression.Evaluate(t);  
}


pair<long,long> Expression_Objective_Function::Get_Cost_Interval ()
// returns the interval of possible value for the objective function
{
  int min [scope.size()];
  int max [scope.size()];
  for (unsigned int i = 0; i < scope.size(); i++)
  {
    min[i] = scope[i]->Get_Domain()->Get_Real_Min();
    max[i] = scope[i]->Get_Domain()->Get_Real_Max();
  }
  return pair<long,long>(expression.Get_Minimal_Value(min,max), expression.Get_Maximal_Value(min,max));
}
