/// \file 
/// \brief Definitions related to the All_Different_List_Global_Constraint class

#ifndef ALL_DIFFERENT_LIST_GLOBAL_CONSTRAINT_H
#define ALL_DIFFERENT_LIST_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class All_Different_List_Global_Constraint: public Global_Constraint      /// This class implements the all-diff global constraint for lists \ingroup core
{
  protected:
    unsigned int list_size;                 ///< the size of each list
    unsigned int list_number;               ///< the number of lists
    set<unsigned int> fixed_lists;          ///< the set of lists for which all variables have a singleton domain
    set<unsigned int> almost_fixed_lists;   ///< the set of lists for which all variables except one have a singleton domain
  
	public:
		// constructors and destructor
		All_Different_List_Global_Constraint (vector<Variable *> & var, unsigned int size);              ///< constructs a new all-diff global constraint
		All_Different_List_Global_Constraint (All_Different_List_Global_Constraint & c);	 ///< constructs a new constraint by copying the constraint c
		~All_Different_List_Global_Constraint ();														         ///< destructor
				
		// basic functions
    Constraint * Duplicate () override;  				  ///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * All_Different_List_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new All_Different_List_Global_Constraint (*this);
}


inline bool All_Different_List_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  if (ref == 0)
    return true;
  return  true;
  bool has_fixed_list = false;
  bool has_almost_fixed_list = false;
  for (unsigned int i = 0; i < list_number; i++)
  {
    unsigned int j = 0;
    unsigned int nb_non_singleton = 0;  // the number of non singleton domain
    while ((j < list_size) && (nb_non_singleton <= 1))
    {
      if (scope_var[list_size*i+j]->Get_Domain()->Get_Size() > 1)
        nb_non_singleton++;
      j++;
    }
    
    if (nb_non_singleton == 0)
      has_fixed_list = true;
    else
      if (nb_non_singleton == 1)
        has_almost_fixed_list = true;
      
    if (has_fixed_list && has_almost_fixed_list)
      return true;
  }
  return false;
}

#endif

