/// \file 
/// \brief Definitions related to the Nary_Extension_Constraint class

#ifndef NARY_EXTENSION_CONSTRAINT_H
#define NARY_EXTENSION_CONSTRAINT_H

#include "avl.h"
#include "extension_constraint.h"

class Nary_Extension_Constraint: public Extension_Constraint    /// This class allows to represent n-ary constraints in extension \ingroup core
{
	protected:
		AVL * relation;		///< the relation related to the constraint
		
	public:
		// destructor 
		Nary_Extension_Constraint (vector<Variable *> & var);				///< constructs a n-ary extension constraint whose scope is defined by the variables in var
		virtual ~Nary_Extension_Constraint(); 								  		///< destructor
		
		
		// basic functions
		AVL * Get_Relation ();											 ///< returns a pointer on the relation
};




inline AVL * Nary_Extension_Constraint::Get_Relation ()
// returns a pointer on the relation
{
	return relation;
}



#endif
