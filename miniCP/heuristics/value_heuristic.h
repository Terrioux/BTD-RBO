/// \file 
/// \brief Definitions related to the Value_Heuristic class

#ifndef VALUE_HEURISTIC_H
#define VALUE_HEURISTIC_H

#include "csp.h"


class Value_Heuristic     /// This class allows to represent value heuristics \ingroup heuristics
{
  protected:
    CSP * pb;               ///< the instance to which the value heuristic is related
    Assignment * last_solution;  ///< the last solution (if any)
    
	public:	
		// constructor and destructor
    Value_Heuristic (CSP * ref_pb);     ///< constructs a value heuristic for the instance ref_pb
		virtual ~Value_Heuristic ();        ///< destructor
		
		// basic functions
		virtual int Next_Value (Assignment & A, int var, int previous_val = -1)=0;	 ///< returns the value of the variable var of the CSP pb which is selected by the heuristic and which follows the value previous_val
    virtual void Set_Solution_Assignment (Assignment * sol) {};            ///< specifies that the solution assignment is given by sol
};

#endif
