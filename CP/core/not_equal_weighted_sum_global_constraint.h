/// \file 
/// \brief Definitions related to the Not_Equal_Weighted_Sum_Global_Constraint class

#ifndef NOT_EQUAL_WEIGHTED_SUM_GLOBAL_CONSTRAINT_H
#define NOT_EQUAL_WEIGHTED_SUM_GLOBAL_CONSTRAINT_H

#include "weighted_sum_global_constraint.h"


class Not_Equal_Weighted_Sum_Global_Constraint: public Weighted_Sum_Global_Constraint   /// This class implements the weighted sum global constraint with condition != \ingroup core
{
	public:
		// constructors and destructor
		Not_Equal_Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, long cst);	///< constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var weighted by weight from w to the constant cst with respect to !=
		Not_Equal_Weighted_Sum_Global_Constraint (Not_Equal_Weighted_Sum_Global_Constraint & c);	    	///< constructs a new constraint by copying the constraint c 
		
		// basic functions
		Constraint * Duplicate () override; 				         ///< returns a pointer on a copy of the constraint
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	         ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
 		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_Relation () override;                     ///< returns the relation used to compare the sum and the constant
    
  private:
    bool Compare (long sum);                     ///< returns true if the sum satisfies the constraint, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Not_Equal_Weighted_Sum_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint w.r.t. the CSP pb
{
	return new Not_Equal_Weighted_Sum_Global_Constraint (*this);
}


inline string Not_Equal_Weighted_Sum_Global_Constraint::Get_Relation ()
// returns the relation used to compare the sum and the constant
{
  return "ne";
}


inline bool Not_Equal_Weighted_Sum_Global_Constraint::Impacted_By_Last_Events (CSP * pb,timestamp ref)
// true if the constraint may be impacted by the last events, false otherwise
{
  int nb = 0;   // the number of variables whose domain contains at least two values
  
  for (unsigned int i = 0; (i < arity) && (nb <= 1); i++)
    if (scope_var[i]->Get_Domain()->Get_Size() > 1)
      nb++;
  
  return nb <= 1;
}


inline bool Not_Equal_Weighted_Sum_Global_Constraint::Compare (long sum)
// returns true if the sum satisfies the constraint, false otherwise
{
  return sum != constant;
}

#endif
