/// \file 
/// \brief Definitions related to the Binary_Constraint class

#ifndef BINARY_CONSTRAINT_H
#define BINARY_CONSTRAINT_H

#include "constraint.h"
#include "assignment.h"

class Binary_Constraint: public Constraint      /// This class allows to represent global constraints \ingroup core
{
	public:
		// constructors
    Binary_Constraint (vector<Variable *> & var);  ///< constructs a new global constraint with number n which involves the variable in var
		Binary_Constraint (Binary_Constraint & c);		 ///< constructs a new global constraint by copying the constraint c
};

#endif
