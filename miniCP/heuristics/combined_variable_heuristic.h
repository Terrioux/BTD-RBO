/// \file 
/// \brief Definitions related to the Combined_Variable_Heuristic class


#ifndef COMBINED_VARIABLE_HEURISTIC_H
#define COMBINED_VARIABLE_HEURISTIC_H

#include "variable_heuristic.h"

class Combined_Variable_Heuristic: public Variable_Heuristic      /// This class implements the last conflict variable heuristics \ingroup heuristics
{
	protected:
    Variable_Heuristic * vh;          ///< the variable heuristic used when there is no conflict

	public:	
		// constructor and destructor
		Combined_Variable_Heuristic (CSP * ref_pb, Variable_Heuristic * var_heur);	///< constructs a variable heuristic for the CSP instance ref_pb
		virtual ~Combined_Variable_Heuristic ();	    ///< destructor

		// basic functions
    Erwa_Weights * Get_Weights () override;                            ///< returns the weights used by the heuristic (if any)
    void Add_Constraint (Constraint * c) override;                ///< takes into account the new constraint c
    void Update_Before_Filtering (Assignment & A, int var = -1) override;       ///< updates the variable heuristic before the application of a filtering related to the assignment A and the current variable var 
    void Update_After_Filtering (Assignment & A, int var = -1) override;        ///< updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
    void Update_When_Restarting () override;                      ///< updates the data structure when restarting
    void Update_When_New_Connected_Component () override;         ///< updates the data structure when taking into account a new connected component
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Erwa_Weights * Combined_Variable_Heuristic::Get_Weights ()
// returns the weights used by the heuristic (if any)
{
  return vh->Get_Weights();
}


inline void Combined_Variable_Heuristic::Add_Constraint (Constraint * c)
// takes into account the new constraint c
{
  vh->Add_Constraint(c);
}


inline void Combined_Variable_Heuristic::Update_Before_Filtering (Assignment & A, int var)
// updates the variable heuristic before the application of a filtering related to the assignment A and the current variable var 
{
  vh->Update_Before_Filtering(A,var);
}


inline void Combined_Variable_Heuristic::Update_After_Filtering (Assignment & A, int var)
// updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
{
  vh->Update_After_Filtering(A,var);
}


inline void Combined_Variable_Heuristic::Update_When_Restarting ()
// updates the data structure when restarting
{
  vh->Update_When_Restarting();
}


inline void Combined_Variable_Heuristic::Update_When_New_Connected_Component ()
// updates the data structure when taking into account a new connected component
{
  vh->Update_When_New_Connected_Component();
}

#endif
