/// \file 
/// \brief Definitions related to the Sum_Global_Constraint class

#ifndef SUM_GLOBAL_CONSTRAINT_H
#define SUM_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class Sum_Global_Constraint: public Global_Constraint   /// This class allows to represent sum global constraints \ingroup core
{
	protected:
		int constant;						///< the constant to which the sum is compared
	
	public:
		// constructors
		Sum_Global_Constraint (vector<Variable *> & var, int cst);		///< constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var to the constant cst with respect to comp
		Sum_Global_Constraint (Sum_Global_Constraint & c);						///< constructs a new constraint by copying the constraint c
		
		// basic functions
 		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
    virtual bool Compare (int sum) = 0;           ///< returns true if the sum satisfies the constraint, false otherwise
		int Get_Constant ();											  	///< returns the constant to which the sum is compared
    virtual string Get_Relation () = 0;           ///< returns the relation used to compare the sum and the constant
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline int Sum_Global_Constraint::Get_Constant ()
// returns the constant to which the sum is compared
{
	return constant;
}

#endif
