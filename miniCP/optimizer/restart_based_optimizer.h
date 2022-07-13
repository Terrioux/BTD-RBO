/// \file 
/// \brief Definitions related to the Restart_Based_Optimizer class

#ifndef RESTART_BASED_OPTIMIZER_H
#define RESTART_BASED_OPTIMIZER_H


using namespace std;


#include "information.h"
#include "cop.h"
#include "assignment.h"
#include "value_heuristic.h"
#include "variable_heuristic.h"
#include "ac.h"
#include "update_policy.h"
#include <cfloat>
#include "restart_policy.h"

enum Optimizer_State ///< type for representing the state of optimizer
{
  NOT_RUN_YET,                  ///< the optimizer has not been run yet
  HAS_FOUND_SOLUTION,           ///< the optimizer has been run and has found a solution
  HAS_FOUND_OPTIMUM,            ///< the optimizer has been run and has found an optimal solution
  HAS_FOUND_INCONSISTENT,       ///< the optimizer has been run and has proved the inconsistency of the instance
  HAS_BEEN_STOPPED              ///< the optimizer has been run and has been stopped before finding a solution 
}; 

class Restart_Based_Optimizer     /// this class allows to represent optimizers for the COP problem  \ingroup optimizer
{
	protected:
    COP * pb;																      ///< the instance to solve
    Optimizer_State state;									      ///< the current state of the optimizer
    Assignment solution;                          ///< the best solution the optimizer has found
    long solution_cost;                           ///< the cost of the best solution the optimizer has found
		Variable_Heuristic * variable_heuristic;			///< the variable heuristic used in the optimizer for ordering the variables and so for choosing the next variable to be assigned
		Value_Heuristic * value_heuristic;						///< the value heuristic used in the optimizer for ordering the values of each variable
		AC * ac;			                              	///< the arc-consistency algorithm used for checking the consistency of the current assignment
		Deletion_Stack * ds;													///< the stack of value deletion
		double time_limit;														///< the time limit 
		Assignment A;																	///< the current assignment of the optimizer
    Update_Policy * update;                       ///< the policy according to which the problem is updated when a solution is found
    Restart_Policy * restart_policy;	            ///< the restart policy

	public:
		// constructor and destructor
		Restart_Based_Optimizer (COP * inst, Variable_Heuristic * var_heur, Value_Heuristic * val_heur, AC * cc, Deletion_Stack * stack, Restart_Policy *rp, Update_Policy * update_pol, double limit);   ///< constructs a optimizer in order to solve the instance inst by using the variable heuristic var_heur, the value heuristic val_heur, the arc-consistency cc and the deletion stack stack. The runtime is limited to limit
		~Restart_Based_Optimizer ();			             ///< destructor

		// basic functions
		Optimizer_State Solve ();	         ///< solves the instance and returns the optimizer state when the solving finishes by proving (in)consistency or by reaching the timeout
    Assignment & Get_Solution (); 	   ///< returns the solution if the COP is consistent or an empty assignment if the COP is inconsistent or has not been solved yet
    long Get_Cost ();                  ///< returns the cost of the best solution the optimizer has found

};


//-----------------------------
// inline function definitions
//-----------------------------

inline Assignment & Restart_Based_Optimizer::Get_Solution ()
// returns the solution if the COP is consistent or an empty assignment if the COP is inconsistent or has not been solved yet
{
  return solution;
}


inline long Restart_Based_Optimizer::Get_Cost ()
// returns the cost of the best solution the optimizer has found
{
  return solution_cost;
}


#endif

