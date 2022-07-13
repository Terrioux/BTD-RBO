/// \file 
/// \brief Definitions related to the Regular_Global_Constraint class

#ifndef REGULAR_GLOBAL_CONSTRAINT_H
#define REGULAR_GLOBAL_CONSTRAINT_H


#include "global_constraint.h"
#include "automaton.h"
#include "r_sparse_set.h"
#include "weighted_r_sparse_set.h"
#include <stack>

class Regular_Global_Constraint: public Global_Constraint      /// This class implements the regular global constraint with variable value (which is the last variable of the scope) \ingroup core
{
  protected:
    Automaton * automaton;                 ///< the automaton used by the regular constraint
    R_Sparse_Set<state> ** Q;              ///< the set of states which supports a pair (variable,value)
    Weighted_R_Sparse_Set<state> ** out_arcs;       ///< the outgoing arcs for each pair (variable,state)
    Weighted_R_Sparse_Set<state> ** in_arcs;        ///< the incoming arc for each pair (variable,state)
    stack<pair<unsigned long,tuple<set<pair<unsigned int,state>>,set<pair<unsigned int,state>>,set<pair<unsigned int,unsigned int>>>>> backup_data;   ///< the data structure used for backuping information to be restored when a backtrack occurs
    
	public:
		// constructors and destructor
		Regular_Global_Constraint (vector<Variable *> & var, Automaton * def_A);    ///< constructs a new regular constraint which involves the variable in var and which is based on the automaton def_A
		Regular_Global_Constraint (Regular_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		~Regular_Global_Constraint ();																///< destructor
				
		// basic functions
    Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 	  ///< returns true if the tuple t satisfies the constraint, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    void Restore () override;                     ///< restores the constraint in its previous state
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
    
  private:
    void Update_Out_Arcs (CSP * pb, unsigned int i, state q_k, Deletion_Stack * ds);    ///< updates the out_arcs data structure when an arc from q_k is deleted
    void Update_In_Arcs (CSP * pb, unsigned int i, state q_k, Deletion_Stack * ds);     ///< updates the in_arcs data structure when an arc to q_k is deleted
    void Update_Backup_Out_Arcs_Data (unsigned int i, state q_l, Deletion_Stack * ds);  ///< memorizes the out arcs to be restored
    void Update_Backup_In_Arcs_Data (unsigned int i, state q_l, Deletion_Stack * ds);   ///< memorizes the in arcs to be restored
    void Update_Backup_Q_Data (unsigned int i, unsigned int j, Deletion_Stack * ds);    ///< memorizes the states from Q to be restored
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Regular_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Regular_Global_Constraint (*this);
}


inline bool Regular_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  for (unsigned int i = 0; i < arity; i++)
    if ((event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(),ref)) || (event_manager->Exist_Event_Dmc (scope_var[i]->Get_Num(),ref)))
      return true;
  return false;
}

#endif
