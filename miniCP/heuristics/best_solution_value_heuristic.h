/// \file 
/// \brief Definitions related to the Best_Solution_Value_Heuristic class

#ifndef BEST_SOLUTION_VALUE_HEURISTIC_H
#define BEST_SOLUTION_VALUE_HEURISTIC_H

#include "value_heuristic.h"
#include "assignment.h"

class Best_Solution_Value_Heuristic: public Value_Heuristic   /// This class implements the value heuristic based on the best found solution \ingroup heuristics
{
  protected:
    Value_Heuristic * auxiliary_heuristic;  ///< the auxiliary value heuristic used when there is no known solution
  
	public:	
		// constructor
		Best_Solution_Value_Heuristic (CSP * ref_pb, Value_Heuristic * vh);   ///< construct a value heuristic based on the best found solution for the instance ref_pb and exploiting vh when no solution is found yet
		~Best_Solution_Value_Heuristic ();   ///< destructor
    
		// basic functions
		int Next_Value (Assignment & A, int var, int previous_val = -1) override;	 ///< returns the value of the variable var of the CSP pb which follows the value previous_val in the lexicographical order
    void Set_Solution_Assignment (Assignment * sol) override;            ///< specifies that the solution assignment is given by sol
};


//-----------------------------
// inline function definitions
//-----------------------------


inline int Best_Solution_Value_Heuristic::Next_Value (Assignment & A, int var, int previous_val)
// returns the value of the variable var of the CSP pb which follows the value previous_val in the lexicographical order
{	
  assert (last_solution != 0);
  if (last_solution->Is_Assigned(var))
  {
    int val = last_solution->Get_Value(var);
    if (pb->Get_Variable(var)->Get_Domain()->Is_Present(val))
      return val;
  }
  
  return auxiliary_heuristic->Next_Value (A,var,previous_val);
}


inline void Best_Solution_Value_Heuristic::Set_Solution_Assignment (Assignment * sol)
{
  last_solution = sol;
}

#endif
