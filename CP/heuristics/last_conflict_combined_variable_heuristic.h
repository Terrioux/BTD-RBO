/// \file 
/// \brief Definitions related to the Last_Conflict_Last_Conflict_Combined_Variable_Heuristic class


#ifndef LAST_CONFLICT_VARIABLE_HEURISTIC_H
#define LAST_CONFLICT_VARIABLE_HEURISTIC_H

#include "combined_variable_heuristic.h"

class Last_Conflict_Combined_Variable_Heuristic: public Combined_Variable_Heuristic      /// This class implements the last conflict variable heuristics \ingroup heuristics
{
	protected:
    Variable * last_conflict;         ///< the variable on which the last conflict occurs
    unsigned int last_size;           ///< the size of the assignment the last time the heuristic was used

	public:	
		// constructor
		Last_Conflict_Combined_Variable_Heuristic (CSP * ref_pb, Variable_Heuristic * var_heur);	///< constructs a variable heuristic for the CSP instance ref_pb

		// basic functions
    void Update_After_Filtering (Assignment & A, int var = -1) override;        ///< updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
		Variable * Next_Variable (Assignment & A, vector<Variable*> & candidates) override;	          ///< returns the variable which has been selected among the variables of candidates which are unassigned in A
    void Update_When_Restarting () override;                      ///< updates the data structure when restarting
    void Update_When_New_Connected_Component () override;         ///< updates the data structure when taking into account a new connected component
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Last_Conflict_Combined_Variable_Heuristic::Update_After_Filtering (Assignment & A, int var)
// updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
{
  Combined_Variable_Heuristic::Update_After_Filtering(A,var);
  
  if ((last_conflict == 0) && (A.Get_Size() == last_size + 1) && (pb->Get_Last_Conflict_Constraint() != 0))
    last_conflict = pb->Get_Variable (A.Last_Assigned());
}


inline void Last_Conflict_Combined_Variable_Heuristic::Update_When_Restarting ()
// updates the data structure when restarting
{
  last_conflict = 0;
  last_size = 0;
  vh->Update_When_Restarting();  
}

inline void Last_Conflict_Combined_Variable_Heuristic::Update_When_New_Connected_Component ()
// updates the data structure when taking into account a new connected component
{
  last_conflict = 0;
  last_size = 0;
  vh->Update_When_New_Connected_Component();
}

#endif
