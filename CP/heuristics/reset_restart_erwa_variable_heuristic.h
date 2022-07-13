/// \file 
/// \brief Definitions related to the Reset_Restart_Erwa_Variable_Heuristic class

#ifndef RESET_RESTART_ERWA_VARIABLE_HEURISTIC_H
#define RESET_RESTART_ERWA_VARIABLE_HEURISTIC_H

#include "variable_heuristic.h"


class Reset_Restart_Erwa_Variable_Heuristic: public Variable_Heuristic        /// This class implements the ERWA variable heuristic (Exponential Recency Weighted Average) with restart management  \ingroup heuristics
{
  protected:
    double delta;     ///< the weight we give to each constraint
    
	public:	
		// constructor
		Reset_Restart_Erwa_Variable_Heuristic (CSP * ref_pb, double alpha_val, double beta_val, double epsilon_val, double delta_val);	///< constructs a variable heuristic for the CSP instance ref_pb
		Reset_Restart_Erwa_Variable_Heuristic (CSP * ref_pb, Erwa_Weights * w, double delta_val);   ///< constructs a variable heuristic for the CSP instance ref_pb with weights w
    
		// basic functions
		Variable * Next_Variable (Assignment & A, vector<Variable*> & candidates) override;    ///< returns the variable which has been selected among the variables of candidates which are unassigned in A
    void Add_Constraint (Constraint * c) override;               ///< takes into account the new constraint c
    void Update_After_Filtering (Assignment & A, int var = -1) override;       ///< updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
    void Update_When_New_Connected_Component () override;        ///< updates the data structure when taking into account a new connected component
    void Update_When_Restarting () override;                     ///< updates the variable heuristic when restarting
};


//-----------------------------
// inline function definitions
//-----------------------------

inline void Reset_Restart_Erwa_Variable_Heuristic::Add_Constraint (Constraint * c)
// takes into account the new constraint c
{
  weights->Add_Constraint (c);
}


inline void Reset_Restart_Erwa_Variable_Heuristic::Update_After_Filtering (Assignment & A, int var)
// updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
{
  weights->Update_After_Filtering (A,var);
}


inline void Reset_Restart_Erwa_Variable_Heuristic::Update_When_New_Connected_Component ()
// updates the data structure when taking into account a new connected component
{
  weights->Update_When_New_Connected_Component();
}


inline void Reset_Restart_Erwa_Variable_Heuristic::Update_When_Restarting ()
// updates the variable heuristic when restarting
{
  weights->Update_When_Restarting ();
  dynamic_cast<Erwa_Weights*>(weights)->Reset_Alpha ();
}


#endif


