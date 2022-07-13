/// \file 
/// \brief Definitions related to the Global_Constraint class

#ifndef GLOBAL_CONSTRAINT_H
#define GLOBAL_CONSTRAINT_H


#include "constraint.h"
#include "assignment.h"

class Global_Constraint: public Constraint      /// This class allows to represent global constraints \ingroup core
{
	public:
		// constructors
		Global_Constraint ();                     		 ///< constructs a new global constraint which initially involves no variable
    Global_Constraint (vector<Variable *> & var);  ///< constructs a new global constraint with number n which involves the variable in var
		Global_Constraint (Global_Constraint & c);		 ///< constructs a new global constraint by copying the constraint c
};

#endif
