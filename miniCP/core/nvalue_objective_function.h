/// \file 
/// \brief Definitions related to the Nvalue_Objective_Function class

#ifndef NVALUE_OBJECTIVE_FUNCTION_H
#define NVALUE_OBJECTIVE_FUNCTION_H

#include "objective_function.h"
#include "predicate_constraint.h"


class Nvalue_Objective_Function: public Objective_Function       /// This class allows to represent objective functions of COP instances which are based on nvalue \ingroup core
{
	protected:
    Variable * x_obj;         ///< the variable representing the value to optimize
    Predicate_Constraint * bound_constraint;  ///< the constraint related to the lower and upper bounds
    
	public:
		// constructors and destructor
		Nvalue_Objective_Function (Objective obj, vector<Variable *> obj_scope);  ///< constructs an objective function involving the objective obj and a maximum based on the variables of obj_scope
		
		// basic functions
    void Update_Problem (COP * pb) override;         ///< updates the problem by adding some variables or constraints to the problem to handle the objective function
    void Update_From_Bounds () override;             ///< updates the problem when the lower or upper bound takes a new value
    long Get_Cost (Assignment & A) override;         ///< returns the value of the objective function for the assignment A, or raises an exception if all the involved variables are not assigned
    pair<long,long> Get_Cost_Interval () override;   ///< returns the interval of possible value for the objective function
};


inline void Nvalue_Objective_Function::Update_From_Bounds ()
//< updates the problem when the lower or upper bound takes a new value
{
  bound_constraint->Reset_Support();
}

#endif
