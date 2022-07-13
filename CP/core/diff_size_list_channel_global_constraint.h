/// \file 
/// \brief Definitions related to the Diff_Size_List_Channel_Global_Constraint class

#ifndef DIFF_SIZE_LIST_CHANNEL_GLOBAL_CONSTRAINT_H
#define DIFF_SIZE_LIST_CHANNEL_GLOBAL_CONSTRAINT_H

#include "list_channel_global_constraint.h"


class Diff_Size_List_Channel_Global_Constraint: public List_Channel_Global_Constraint   /// This class implements the channel global constraint  \ingroup core
{
	public:
		// constructors 
		Diff_Size_List_Channel_Global_Constraint (vector<Variable *> & var, vector<Variable *> & list_1, int start1, vector<Variable *> & list_2, int start2);		///< constructs a new channel constraint which involves the variable in var and whose start indices are respectively start1 and start2
		Diff_Size_List_Channel_Global_Constraint (Diff_Size_List_Channel_Global_Constraint & c);		      ///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		  	///< returns true if the tuple t satisfies the constraint, false otherwise
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Diff_Size_List_Channel_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Diff_Size_List_Channel_Global_Constraint (*this);
}


inline bool Diff_Size_List_Channel_Global_Constraint::Impacted_By_Last_Events (CSP * pb,timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  for (int i = list[0].size()-1; i >= 0; i--)
    if (event_manager->Exist_Event_Fix(list[0][i]->Get_Num(),ref))
      return true;
  
  for (int i = list[1].size()-1; i >= 0; i--)
    if ((event_manager->Exist_Event_Fix(list[1][i]->Get_Num(),ref)) || (event_manager->Exist_Event_Dmc(list[1][i]->Get_Num(),ref)))
      return true;
  
  return false;
}

#endif
