/// \file 
/// \brief Definitions related to the K_Dim_No_Overlap_Global_Constraint class

#ifndef K_DIM_NO_OVERLAP_GLOBAL_CONSTRAINT_H
#define K_DIM_NO_OVERLAP_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class K_Dim_No_Overlap_Global_Constraint: public Global_Constraint   /// This class implements the No-Overlap global constraint for K dimension \ingroup core
{
	protected:
    unsigned int object_number;      ///< the number of object
    unsigned int dimension_number;   ///< the number of dimensions
		vector<int> lengths;    ///< the length of each object in each dimension
		
	public:
		// constructors and destructor
		K_Dim_No_Overlap_Global_Constraint (vector<Variable *> & var, vector<int> & vec_lengths, unsigned int nb_dim);		///< constructs a new constraint which involves the variable in var
		K_Dim_No_Overlap_Global_Constraint (K_Dim_No_Overlap_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * K_Dim_No_Overlap_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new K_Dim_No_Overlap_Global_Constraint (*this);
}


inline bool K_Dim_No_Overlap_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  if (ref == 0)
    return true;
  
  for (unsigned int i = 0; i < arity; i++)
    if ((event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(),ref)) || (event_manager->Exist_Event_Lbc (scope_var[i]->Get_Num(),ref)) || (event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(),ref)))
      return true;
  
  return false;
}


#endif
