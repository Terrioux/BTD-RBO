/// \file 
/// \brief Definitions related to the Variable_Heuristic class


#ifndef VARIABLE_HEURISTIC_H
#define VARIABLE_HEURISTIC_H

#include "csp.h"
#include "assignment.h"
#include "erwa_weights.h"

class Variable_Heuristic      /// This class allows to represent variable heuristics \ingroup heuristics
{
	protected:
    CSP * pb;                         ///< the CSP instance to which the heuristic refer
    Erwa_Weights * weights;                ///< the weights used by the heuristic (if any)
    bool has_original_weights;        ///< true if the weights are given with the orignal pointer, false otherwise (useful for the destruction)

	public:	
		// constructor
		Variable_Heuristic (CSP * ref_pb);	///< constructs a variable heuristic for the CSP instance ref_pb
		virtual ~Variable_Heuristic ();	    ///< destructor


		// basic functions
    virtual Erwa_Weights * Get_Weights ();                       ///< returns the weights used by the heuristic (if any)
    virtual void Add_Constraint (Constraint * c)=0;              ///< takes into account the new constraint c
    virtual void Update_Before_Filtering (Assignment & A, int var = -1);   ///< updates the variable heuristic before the application of a filtering related to the assignment A and the current variable var 
    virtual void Update_After_Filtering (Assignment & A, int var = -1);    ///< updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
    virtual void Update_When_Restarting ();                      ///< updates the data structure when restarting
    virtual void Update_When_New_Connected_Component ();         ///< updates the data structure when taking into account a new connected component
		virtual Variable * Next_Variable (Assignment & A, vector<Variable*> & candidates)=0;	       ///< returns the variable which has been selected among the variables of candidates which are unassigned in A
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Erwa_Weights * Variable_Heuristic::Get_Weights ()
// returns the weights used by the heuristic (if any)
{
  return weights;
}


inline void Variable_Heuristic::Update_Before_Filtering (Assignment & A, int var)
// updates the variable heuristic before the application of a filtering related to the assignment A and the current variable var 
{
}


inline void Variable_Heuristic::Update_After_Filtering (Assignment & A, int var)
// updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
{
}

inline void Variable_Heuristic::Update_When_Restarting ()
// updates the data structure when restarting
{
}

inline void Variable_Heuristic::Update_When_New_Connected_Component ()
// updates the data structure when taking into account a new connected component
{
}
#endif
