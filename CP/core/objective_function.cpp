#include "objective_function.h"

Objective_Function::Objective_Function (Objective obj, vector<Variable *> obj_scope)
// constructs an objective function involving the objective obj and the variables of obj_scope
{
  objective  = obj;
  scope = obj_scope;
  criterion_information = "";
  problem_updated = false;
}
