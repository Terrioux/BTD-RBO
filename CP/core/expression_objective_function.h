/// \file 
/// \brief Definitions related to the Objective_Function class

#ifndef EXPRESSION_OBJECTIVE_FUNCTION_H
#define EXPRESSION_OBJECTIVE_FUNCTION_H

#include "objective_function.h"
#include "predicate_constraint.h"

class Expression_Objective_Function: public Objective_Function       /// This class allows to represent objective functions of COP instances which are based on an expression \ingroup core
{
	protected:
    Expression expression;      ///< the function to optimize
    Predicate_Constraint * bound_constraint;  ///< the constraint related to the lower and upper bounds
    
	public:
		// constructors and destructor
		Expression_Objective_Function (Objective obj, vector<Variable *> obj_scope, Expression expr);  ///< constructs an objective function involving the objective obj and the expression expr based on the variables of obj_scope and the weights of w
		
		// basic functions
    void Update_Problem (COP * pb) override;         ///< updates the problem by adding some variables or constraints to the problem to handle the objective function
    void Update_From_Bounds () override;             ///< updates the problem when the lower or upper bound takes a new value
    long Get_Cost (Assignment & A) override;         ///< returns the value of the objective function for the assignment A, or raises an exception if all the involved variables are not assigned
    pair<long,long> Get_Cost_Interval () override;   ///< returns the interval of possible value for the objective function
};


inline void Expression_Objective_Function::Update_From_Bounds ()
//< updates the problem when the lower or upper bound takes a new value
{
  bound_constraint->Reset_Support();
}
#endif
