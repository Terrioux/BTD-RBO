/// \file 
/// \brief Definitions related to the Constraint_Basic_Deletion_Stack class

#ifndef CONSTRAINT_BASIC_DELETION_STACK_H
#define CONSTRAINT_BASIC_DELETION_STACK_H

#include "basic_deletion_stack.h"


class Constraint_Basic_Deletion_Stack: public Basic_Deletion_Stack       /// This class implements basic stacks of deletions with constraint management \ingroup core
{
	protected:
		vector<Constraint *> constraint_elements;	    ///< stack represented in a vector of the modified constraints
		unsigned int * size_constraint_elements;      ///< the current number of values in the stack
    unsigned long * constraint_last_modified;			///< specifies for each constraint the last time it has been modified

	public:
		// constructor and destructor
		Constraint_Basic_Deletion_Stack (CSP * pb, int level_number);	 ///< construct an empty deletion stack related to the CSP pb with at most level_number levels
		~Constraint_Basic_Deletion_Stack();														 ///< destructor
		
		
		// basic functions
		void New_Level () override;											///< adds a new level of stack (i.e. increases the top of the stack)
		void Add_Element (Constraint * c) override; 		///< adds the constraint c to the current level of the stack
		void Restore (CSP * pb) override;								///< restores the elements at the top level and decreases the top of the stack
    unsigned long Get_Timestamp () override;	      ///< returns the current value of the timestamp
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Constraint_Basic_Deletion_Stack::New_Level ()
// adds a new level of stack (i.e. increases the top of the stack)
{
  Basic_Deletion_Stack::New_Level();
  size_constraint_elements[top] = constraint_elements.size();
}

		
inline void Constraint_Basic_Deletion_Stack::Add_Element (Constraint * c)
// adds the constraint c to the current level of the stack
{
 	if (constraint_last_modified[c->Get_Num()] != timestamp)
	{
		constraint_elements.push_back(c);
    constraint_last_modified[c->Get_Num()] = timestamp;
	}
}

    
inline void Constraint_Basic_Deletion_Stack::Restore (CSP * pb)
// restores the elements at the top level and decreases the top of the stack
/// \param[in] pb the CSP instance in which the values must be restored
{
	if (top != -1)
	{
		while (number_elements[top] != 0)
		{
			size--;
			elements[size].first->Restore (elements[size].second);
      
			number_elements[top]--;
		}

    while (constraint_elements.size() > size_constraint_elements[top])
    {
      constraint_elements.back()->Restore();
      constraint_elements.pop_back();
    }
    
		top--;
	}
}


inline unsigned long Constraint_Basic_Deletion_Stack::Get_Timestamp ()
// returns the current value of the timestamp
{
  return timestamp;
}

#endif
