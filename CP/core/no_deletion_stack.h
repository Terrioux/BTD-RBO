/// \file 
/// \brief Definitions related to the No_Deletion_Stack class

#ifndef NO_DELETION_STACK_H
#define NO_DELETION_STACK_H

#include "csp.h"
#include "deletion_stack.h"

class No_Deletion_Stack: public Deletion_Stack    /// This class implements the absence of stack of deletions \ingroup core
{
	public:
		
		// basic functions
		void New_Level () override {};												///< adds a new level of stack (i.e. increases the top of the stack)
		void Add_Element (Variable * x) override {};					///< adds the domain of the variable x to the current level of the stack
		void Restore (CSP * pb) override {};									///< restores the elements at the top level and decreases the top of the stack
    void Reset_Levels () override {};							      ///< resets the level to the starting point
};

#endif
