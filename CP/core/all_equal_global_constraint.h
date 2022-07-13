/// \file 
/// \brief Definitions related to the All_Equal_Global_Constraint class

#ifndef ALL_EQUAL_GLOBAL_CONSTRAINT_H
#define ALL_EQUAL_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class All_Equal_Global_Constraint: public Global_Constraint   /// This class implements the global constraint AllEqual \ingroup core
{
	protected:
		int *** equal_value;    ///< maps to each variable i and value j the value k for each of the other variables s.t. j and k correspond to the same value
	
	public:
		// constructors and destructor
		All_Equal_Global_Constraint (vector<Variable *> & var);											///< constructs a new constraint which involves the variable in var and whose relation imposes that all the variables are equal
		All_Equal_Global_Constraint (All_Equal_Global_Constraint & c);		///< constructs a new constraint by copying the constraint c
		~All_Equal_Global_Constraint ();													 			   						///< destructor
		
		// basic functions
		Constraint * Duplicate () override;				  	///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
 		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * All_Equal_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new All_Equal_Global_Constraint (*this);
}


inline bool All_Equal_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  return unassigned_variable_number >= arity-1;
}

#endif
