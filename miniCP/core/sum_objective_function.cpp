#include "sum_objective_function.h"
#include "less_weighted_sum_global_constraint.h"
#include "greater_weighted_sum_global_constraint.h"
#include "cop.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Sum_Objective_Function::Sum_Objective_Function (Objective obj, vector<Variable *> obj_scope, vector<int> w): Objective_Function(obj, obj_scope)
// constructs an objective function involving the objective obj and a weighted sum based on the variables of obj_scope and the weights of w
{
  weights = w;
  
  if (obj == MAXIMIZE)
  {
    for (unsigned int i = 0; i < scope.size(); i++)
      weights[i] = -weights[i];
  }
  
  criterion_information = "sum "+to_string(scope.size());
}


//-----------------
// basic functions
//-----------------


void Sum_Objective_Function::Update_Problem (COP * pb)
// updates the problem by adding some variables or constraints to the problem to handle the objective function
{
  if (! problem_updated)
  {
    problem_updated = true;
    vector<Variable *> scope_sum = scope;
    vector<int> coefs = weights;
    coefs.push_back (-1);  
    
    // we create the constraint related to the lower bound
    scope_sum.push_back (pb->Get_Lower_Bound_Variable());
    pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope_sum,coefs,false,0),false);
    
    // we create the constraint related to the upper bound
    scope_sum[scope_sum.size()-1] = pb->Get_Upper_Bound_Variable();
    pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope_sum,coefs,false,0),false);
  }
}


long Sum_Objective_Function::Get_Cost (Assignment & A)
// returns the value of the objective function for the assignment A, or raises an exception if all the involved variables are not assigned
{
  long sum = 0;
  
  for (unsigned int i = 0; i < scope.size(); i++)
  {
    unsigned int num = scope[i]->Get_Num();
    if (A.Is_Assigned(num))
      sum += scope[i]->Get_Domain()->Get_Real_Value(A.Get_Value(num)) * weights[i];
    else
      if (scope[i]->Get_Domain()->Get_Size() == 1)
        sum += scope[i]->Get_Domain()->Get_Remaining_Real_Value(0) * weights[i];
      else
        throw ("Error: a variable is not assigned when computing the cost of the objective function");
  }
  
  return sum;  
}


pair<long,long> Sum_Objective_Function::Get_Cost_Interval ()
// returns the interval of possible value for the objective function
{
  long min = 0;
  long max = 0;
  
  for (unsigned int i = 0; i < scope.size(); i++)
  {
    Domain * d = scope[i] ->Get_Domain();
    
    if (weights[i] > 0)
    {
      min += weights[i] * d->Get_Real_Min();
      max += weights[i] * d->Get_Real_Max();
    }
    else
      {
        min += weights[i] * d->Get_Real_Max();
        max += weights[i] * d->Get_Real_Min();
      }
  }
  
  return pair<long,long>(min,max);
}
