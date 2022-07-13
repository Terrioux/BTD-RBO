/// \file 
/// \brief Definitions related to the Weighted_Sum_Global_Constraint class

#ifndef WEIGHTED_SUM_GLOBAL_CONSTRAINT_H
#define WEIGHTED_SUM_GLOBAL_CONSTRAINT_H

#include "sum_global_constraint.h"
#include <cassert>

class Weighted_Sum_Global_Constraint: public Sum_Global_Constraint    /// This class allows to represent weighted sum global constraints \ingroup core
{
	protected:
		int * weights;			    ///< the weights associated to each variable
		
	public:
		// constructors and destructor
		Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, int cst);	///< constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var weighted by weight from w to the constant cst with respect to comp
		Weighted_Sum_Global_Constraint (Weighted_Sum_Global_Constraint & c);									///< constructs a new constraint by copying the constraint c
		~Weighted_Sum_Global_Constraint ();								 			   														///< destructor
		
		// basic functions
 		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
  	int Get_Weight (unsigned int i);						  ///< returns the weight of the ith variable in the scope
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline int Weighted_Sum_Global_Constraint::Get_Weight (unsigned int i)
// returns the weight of the ith variable in the scope
{
	assert (i < arity);
	return (weights[i]);
}

#endif
