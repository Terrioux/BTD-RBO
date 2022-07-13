/// \file 
/// \brief Definitions related to the Deletion_Stack class

#ifndef DELETION_STACK_H
#define DELETION_STACK_H

#include <utility>
#include "variable.h"
class Constraint;

class Deletion_Stack      /// This class allows to represent the stack of deletions in which the deletions of values are saved \ingroup core
{				
	public:	
		// constructor and destructor
		virtual ~Deletion_Stack(){};    ///< destructor
		
		// basic functions
		virtual void New_Level ()=0;												  ///< adds a new level of stack (i.e. increases the top of the stack)
		virtual void Add_Element (Variable * x)=0; 						///< adds the domain of the variable x to the current level of the stack
		virtual void Add_Element (Constraint * c); 					  ///< adds the constraint c to the current level of the stack
    virtual void Add_Support (int num, int pos, int val, int arity, int * tuple);	///< adds a tuple to the current level of the stack
		virtual void Restore (CSP * pb)=0;									  ///< restores the elements at the top level and decreases the top of the stack
		
    virtual void Reset_Levels ()=0;									      ///< resets the level to the starting point
    virtual unsigned long Get_Timestamp ();					      ///< returns the current value of the timestamp
};


//-----------------------------
// inline function definitions
//-----------------------------


inline unsigned long Deletion_Stack::Get_Timestamp ()
// returns the current value of the timestamp
{
  return 0;
}


inline void Deletion_Stack::Add_Element (Constraint * c)
// adds the limit of the constraint c to the current level of the stack
{
}

inline void Deletion_Stack::Add_Support (int num, int pos, int val, int arity, int * tuple)
/// adds a tuple to the current level of the stack
{
}

#endif
