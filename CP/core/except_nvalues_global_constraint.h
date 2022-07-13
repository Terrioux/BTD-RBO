/// \file 
/// \brief Definitions related to the Except_NValues_Global_Constraint class

#ifndef EXCEPT_NVALUES_GLOBAL_CONSTRAINT_H
#define EXCEPT_NVALUES_GLOBAL_CONSTRAINT_H

#include "nvalues_global_constraint.h"


class Except_NValues_Global_Constraint: public NValues_Global_Constraint   /// This class implements the global constraint NValues with an exception value \ingroup core
{
	protected:
    int exception_value;	  	///< the value which is not taken into account in the NValues constraint
    int * exception_values;		///< the value corresponding to exception_value for each variable
  
	public:
		// constructors and destructor
		Except_NValues_Global_Constraint (vector<Variable *> & var, int except_val);	 ///< constructs a new constraint which involves the variable in var and whose relation imposes that var[0]=NValues(var[1...]) but without taking into account the value except_val
		Except_NValues_Global_Constraint (Except_NValues_Global_Constraint & c);		   ///< constructs a new constraint by copying the constraint c
		~Except_NValues_Global_Constraint ();		    ///< destructor
		
		// basic functions
		Constraint * Duplicate () override;				  	///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
 		void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Except_NValues_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Except_NValues_Global_Constraint (*this);
}

#endif
