/// \file 
/// \brief Definitions related to the Objective_Function class

#ifndef OBJECTIVE_FUNCTION_H
#define OBJECTIVE_FUNCTION_H

#include <vector>
#include "assignment.h"


enum Objective ///< type of objective we want to reach
{
  MINIMIZE,                      ///< the objective is a minimization
  MAXIMIZE                       ///< the objective is a maximization
};

class COP;

class Objective_Function        /// This class allows to represent objective functions of COP instances \ingroup core
{
	protected:
    Objective objective;                  ///< the kind of objective
    vector<Variable *> scope;             ///< the variables involved in the objective function
    string criterion_information;         ///< some information about the objective function
    bool problem_updated;                 ///< specifies if the related COP instance has been updated to handle the objective function
    
	public:
		// constructors and destructor
		Objective_Function (Objective obj, vector<Variable *> obj_scope);  ///< constructs an objective function involving the objective obj and the variables of obj_scope
		virtual ~Objective_Function (){};	 				      ///< destructor
		
		// basic functions
    virtual void Update_Problem (COP * pb) = 0;         ///< updates the problem by adding some variables or constraints to the problem to handle the objective function
    virtual void Update_From_Bounds (){};               ///< updates the problem when the lower or upper bound takes a new value
    Objective Get_Objective ();                         ///< returns the objective we want to reach
    string Get_Criterion_Information ();                ///< returns the information about the objective function
    virtual long Get_Cost (Assignment & A) = 0;         ///< returns the value of the objective function for the assignment A, or raises an exception if all the involved variables are not assigned
    virtual pair<long,long> Get_Cost_Interval () = 0;   ///< returns the interval of possible value for the objective function
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Objective Objective_Function::Get_Objective ()
// returns the objective we want to reach
{
  return objective;
}


inline string Objective_Function::Get_Criterion_Information ()
// returns the information about the objective function
{
  return criterion_information;
}

#endif
