/// \file 
/// \brief Definitions related to the Sum_Objective_Function class

#ifndef SUM_OBJECTIVE_FUNCTION_H
#define SUM_OBJECTIVE_FUNCTION_H

#include "objective_function.h"


class Sum_Objective_Function: public Objective_Function       /// This class allows to represent objective functions of COP instances which are based on a weighted sum \ingroup core
{
	protected:
    vector<int> weights;             ///< the weight relate to each variable involved in the objective function
    
	public:
		// constructors and destructor
		Sum_Objective_Function (Objective obj, vector<Variable *> obj_scope, vector<int> w);  ///< constructs an objective function involving the objective obj and a weighted sum based on the variables of obj_scope and the weights of w
		
		// basic functions
    void Update_Problem (COP * pb) override;         ///< updates the problem by adding some variables or constraints to the problem to handle the objective function
    long Get_Cost (Assignment & A) override;         ///< returns the value of the objective function for the assignment A, or raises an exception if all the involved variables are not assigned
    pair<long,long> Get_Cost_Interval () override;   ///< returns the interval of possible value for the objective function
};

#endif
