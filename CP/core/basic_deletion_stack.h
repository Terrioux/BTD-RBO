/// \file 
/// \brief Definitions related to the Basic_Deletion_Stack class

#ifndef BASIC_DELETION_STACK_H
#define BASIC_DELETION_STACK_H

using namespace std;

#include "csp.h"
#include "deletion_stack.h"

class Basic_Deletion_Stack: public Deletion_Stack       /// This class implements basic stacks of deletions \ingroup core
{
	protected:
		unsigned long timestamp;								///< the time stamp used in order to determine if the stack must be updated or not	
		unsigned long * last_modified;					///< specifies for each variable the last time it has been modified
		vector<pair<Domain *,unsigned int>> elements;	  ///< stack represented in a vector of the elements (the elements are pair (domain,size))
		int * number_elements;				  				///< the number of elements in the stack for each level
		unsigned int size;             				  ///< the current number of values in the stack
		int top; 											 				  ///< the top of the stack (i.e. the current level)

	public:
		// constructor and destructor
		Basic_Deletion_Stack (CSP * pb, int level_number);	 ///< construct an empty deletion stack related to the CSP pb with at most level_number levels
		~Basic_Deletion_Stack();														 ///< destructor
		
		
		// basic functions
		void New_Level () override;										///< adds a new level of stack (i.e. increases the top of the stack)
		void Add_Element (Variable * x) override;   	///< adds the domain of the variable x to the current level of the stack
		void Restore (CSP * pb) override;							///< restores the elements at the top level and decreases the top of the stack
    void Reset_Levels () override;					      ///< resets the level to the starting point
};


//-----------------------------
// inline function definitions
//-----------------------------

inline void Basic_Deletion_Stack::New_Level ()
// adds a new level of stack (i.e. increases the top of the stack)
{
	timestamp++;
	top++;
	number_elements[top] = 0;
}


inline void Basic_Deletion_Stack::Add_Element (Variable * x)
// adds the domain of the variable x to the current level of the stack
/// \param[in] x the variable whose domain must be backed up
{
	if (last_modified[x->Get_Num()] != timestamp)
	{
		number_elements[top]++;
    
    if (size == elements.size())
      elements.resize(elements.size()+1000);
		
    elements[size] = pair<Domain *,int>(x->Get_Domain(),x->Get_Domain()->Get_Size());
		size++;
    last_modified[x->Get_Num()] = timestamp;
	}
}


inline void Basic_Deletion_Stack::Restore (CSP * pb)
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
		
		top--;
	}
}


inline void Basic_Deletion_Stack::Reset_Levels ()
// resets the level to the starting point
{
 	top = -1;
	size = 0;
}
#endif
