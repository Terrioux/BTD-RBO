/// \file 
/// \brief Definitions related to the MAB_Variable_Heuristic class


#ifndef MAB_VARIABLE_HEURISTIC_H
#define MAB_VARIABLE_HEURISTIC_H

#include "variable_heuristic.h"
#include "mab.h"

class MAB_Variable_Heuristic: public Variable_Heuristic      /// This class allows to represent variable heuristics based on Multi-Armed Bandits \ingroup heuristics
{
	protected:
    MAB * mab;                   ///< a multi-armed bandit which allows us to choose a variable heuristic among several ones
    vector<Variable_Heuristic *> possible_heuristics;     ///< the several possible heuristics
    unsigned int current_arm;    ///< the current arm of the multi-armed bandit

	public:	
		// constructor
		MAB_Variable_Heuristic (CSP * ref_pb, MAB * bandit, vector<Variable_Heuristic *> & possible_heur); 	///< constructs a variable heuristic for the CSP instance ref_pb based on a multi-armed bandit in order to select a variable heuristic among ones provided in possible_heur
		virtual ~MAB_Variable_Heuristic ();	    ///< destructor


		// basic functions
    Erwa_Weights * Get_Weights () override;             ///< returns the weights used by the heuristic (if any)
    void Add_Constraint (Constraint * c) override;       ///< takes into account the new constraint c
    void Update_Before_Filtering (Assignment & A, int var = -1) override;   ///< updates the variable heuristic before the application of a filtering related to the assignment A and the current variable var 
    void Update_After_Filtering (Assignment & A, int var = -1) override;    ///< updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
    void Update_When_Restarting () override;                                ///< updates the data structure when restarting
    void Update_When_New_Connected_Component() override;                    ///< updates the data structure when taking into account a new connected component        void Update_Before_Filtering (Assignment & A, int var = -1) override;   ///< updates the variable heuristic before the application of a filtering related to the assignment A and the current variable var 
    
		Variable * Next_Variable (Assignment & A, vector<Variable*> & candidates) override;	 ///< returns the variable which has been selected among the variables of candidates which are unassigned in A
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Erwa_Weights * MAB_Variable_Heuristic::Get_Weights ()
// returns the weights used by the heuristic (if any)
{
  return possible_heuristics[current_arm]->Get_Weights();
}

inline void MAB_Variable_Heuristic::Add_Constraint (Constraint * c)
// takes into account the new constraint c
{
  for (vector<Variable_Heuristic *>::iterator iter = possible_heuristics.begin(); iter != possible_heuristics.end(); iter++)
    (*iter)->Add_Constraint(c);
}


inline void MAB_Variable_Heuristic::Update_When_New_Connected_Component()
// updates the data structure when taking into account a new connected component
{
  for (vector<Variable_Heuristic *>::iterator iter = possible_heuristics.begin(); iter != possible_heuristics.end(); iter++)
    (*iter)->Update_When_New_Connected_Component();
    
  //~ current_arm = mab->Choose();
  current_arm = 0;
  mab->Reset();
}


inline Variable * MAB_Variable_Heuristic::Next_Variable (Assignment & A, vector<Variable*> & candidates)
// returns the variable which has been selected among the variables of candidates which are unassigned in A
{
  return possible_heuristics[current_arm]->Next_Variable(A,candidates);
}


inline void MAB_Variable_Heuristic::Update_Before_Filtering (Assignment & A, int var)
// updates the variable heuristic before the application of a filtering related to the assignment A and the current variable var 
{
  mab->Update_Before_Filtering(A,pb);
  possible_heuristics[current_arm]->Update_Before_Filtering(A,var);
}


inline void MAB_Variable_Heuristic::Update_After_Filtering (Assignment & A, int var)
//< updates the variable heuristic after the application of a filtering related to the assignment A and the current variable var 
{
  mab->Update_After_Filtering(A,pb);
  possible_heuristics[current_arm]->Update_After_Filtering(A,var);
}


inline void MAB_Variable_Heuristic::Update_When_Restarting ()
// updates the data structure when restarting
{
  possible_heuristics[current_arm]->Update_When_Restarting();
  current_arm = mab->Choose();
}
#endif
