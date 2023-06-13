#include "minimum_objective_function.h"
#include "predicate_constraint.h"
#include "minimum_global_constraint.h"
#include "cop.h"
#include <climits>

//-----------------------------
// constructors and destructor
//-----------------------------


Minimum_Objective_Function::Minimum_Objective_Function (Objective obj, vector<Variable *> obj_scope): Objective_Function(obj, obj_scope)
// constructs an objective function involving the objective obj and a minimum based on the variables of obj_scope
{
  criterion_information = "minimum "+to_string(scope.size());
  x_obj = 0;
}


//-----------------
// basic functions
//-----------------


void Minimum_Objective_Function::Update_Problem (COP * pb)
// updates the problem by adding some variables or constraints to the problem to handle the objective function
{
  if (! problem_updated)
  {
    problem_updated = true;
    
    set<long> values_obj;
    for (unsigned int i = 0; i < scope.size(); i++)
    {
      Domain * d = scope[i]->Get_Domain();
      for (int j = d->Get_Size()-1; j >= 0; j--)        
        values_obj.insert (d->Get_Remaining_Real_Value(j));
    }
    
    pb->Add_Variable (values_obj,"obj",true);
    
    x_obj = pb->Get_Variable(pb->Get_N()-1);
    
    vector<Variable *> scope_ctr;
    scope_ctr = scope;
    scope_ctr.insert (scope_ctr.begin(),x_obj);
      
    // we create the constraint
    pb->Add_Constraint (new Minimum_Global_Constraint (scope_ctr),false);
   
    // we create the constraint related to the lower and upper bounds    
    Expression subexpr [3];
    subexpr[0] = Expression(VAR,x_obj->Get_Num());
    subexpr[1] = Expression(VAR,pb->Get_Lower_Bound_Variable()->Get_Num());
    subexpr[2] = Expression(VAR,pb->Get_Upper_Bound_Variable()->Get_Num());

    vector<Variable *> scope_expr;
    scope_expr.push_back(x_obj);
    scope_expr.push_back(pb->Get_Lower_Bound_Variable());
    scope_expr.push_back(pb->Get_Upper_Bound_Variable());
    
    Expression e = Expression (INI, subexpr,3);
    if (objective == MINIMIZE)
      bound_constraint = new Predicate_Constraint(scope_expr,e);
    else
      {
        Expression neg_e = Expression(NEG,&e);
        bound_constraint =  new Predicate_Constraint(scope_expr,neg_e);
      }
    pb->Add_Constraint(bound_constraint,false);
  }
}

long Minimum_Objective_Function::Get_Cost (Assignment & A)
// returns the value of the objective function for the assignment A, or raises an exception if all the involved variables are not assigned
{
  if (problem_updated)
  {
    if (x_obj->Get_Domain()->Get_Size() == 1)
      return x_obj->Get_Domain()->Get_Remaining_Real_Value(0);
    else
      throw ("Error: a variable is not assigned when computing the cost of the objective function");
  }
  else
      throw ("Error: the problem is not up to date");
}


pair<long,long> Minimum_Objective_Function::Get_Cost_Interval ()
// returns the interval of possible value for the objective function
{
  long min = LONG_MAX;
  long max = LONG_MAX;
  
  for (unsigned int i = 0; i < scope.size(); i++)
  {
    Domain * d = scope[i] ->Get_Domain();
    
    if (d->Get_Real_Min() < min)
       min = d->Get_Real_Min();

    if (d->Get_Real_Max() < max)
       max = d->Get_Real_Max();
  }
  
  return pair<long,long>(min,max);
}
