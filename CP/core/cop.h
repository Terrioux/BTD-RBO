/// \file 
/// \brief Definitions related to the COP class

#ifndef COP_H
#define COP_H

#include "csp.h"

enum Objective ///< type of objective we want to reach
{
  MINIMIZE,                      ///< the objective is a minimization
  MAXIMIZE                       ///< the objective is a maximization
};     

class COP: public CSP      /// This class allows to represent COP instances \ingroup core
{
	protected:
    Objective objective;                            ///< the kind of objective
    Variable * objective_variable;                  ///< the variable representing the objective we want to minimize
    string criterion_information;                   ///< some information about the objective function
    
				
	public:
		// constructors and destructor
		COP (Event_Manager * em=0, string pb_name="");  ///< constructs an empty CSP whose name is pb_name and for which the events are managed by em
		COP (COP & pb);						                      ///< constructs a CSP by copying the CSP pb
		~COP ();	        				                      ///< destructor
		
		// basic functions
    void Set_Objective (Objective obj, unsigned int num);  ///< sets the objective criterion to obj and the objective variable to the variable whose number is num
    Objective Get_Objective ();                     ///< returns the objective we want to reach
    void Set_Criterion_Information (string info);   ///< sets to info the information about the objective function 
    string Get_Criterion_Information ();            ///< returns the information about the objective function
    Variable * Get_Objective_Variable ();           ///< returns the objective variable
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Objective COP::Get_Objective ()
// returns the objective we want to reach
{
  return objective;
}


inline void COP::Set_Criterion_Information (string info)
// sets to info the information about the objective function 
/// param[in] info the new information about the objective function
{
  criterion_information = info;
}


inline string COP::Get_Criterion_Information ()
// returns the information about the objective function
{
  return criterion_information;
}


inline Variable * COP::Get_Objective_Variable ()
// returns the objective variable
{
  return objective_variable;
}

#endif
