/// \file 
/// \brief Definitions related to the Support_Deletion_Stack class

#ifndef SUPPORT_DELETION_STACK_H
#define SUPPORT_DELETION_STACK_H

using namespace std;

#include "csp.h"
#include "basic_deletion_stack.h"
#include "support.h"

class Support_Deletion_Stack: public Basic_Deletion_Stack     /// This class implements a stack of deletions with backup of the supports \ingroup core
{
	protected:
		CSP * ref_pb; 							///< the CSP to which the stack is related
		Support * ls;								///< a pointer on the support data structure
		pair<int, vector <pair<int,pair <int, int *> > > > ** supports;		///< the supports for a given constraint and a given variable (represented by its position in the scope of the constraint), for different depths		
		pair<int, vector <pair<int,int> > > * support_change;							///< the stack containing the modification of support
		int ** max_used_size;       ///< the number of allocated supports for each constraint and each variable
		int ** is_support_changed;  ///< true if the support for a given constraint and a given variable has changed, false otherwise
		
	public:
		// constructor and destructor
		Support_Deletion_Stack (CSP * pb, int level_number, Support * l_s);	 ///< construct an empty deletion stack related to the CSP pb with at most level_number levels
		~Support_Deletion_Stack();					 ///< destructor
		
		
		// basic functions
		void New_Level () override;													///< adds a new level of stack (i.e. increases the top of the stack)
		void Add_Support (int num, int pos, int val, int arity, int * tuple) override;		///< adds a tuple to the current level of the stack
		void Restore (CSP * pb) override;										///< restores the elements at the top level and decreases the top of the stack
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Support_Deletion_Stack::New_Level ()
// adds a new level of stack (i.e. increases the top of the stack)
{
	timestamp++;
	top++;
	number_elements[top] = 0;
	support_change[top].first = 0;
}


inline void Support_Deletion_Stack::Add_Support (int num, int pos, int val, int arity, int * tuple)
// adds an element to the current level of the stack
/// \param[in] num the number of the constraint
/// \param[in] pos the position of the variable
/// \param[in] val the value of the variable
/// \param[in] arity the arity of the support tuple
/// \param[in] tuple the support tuple
{
	// we look if a support has not been saved yet for the constraint num, the position pos, the value val and the current depth
	bool not_found = true;
	
	if (tuple != 0)
	{
		int i = supports[num][pos].first - 1;
		while ((i >= 0) && (supports[num][pos].second[i].first == top) && (not_found))
		{
			if (supports[num][pos].second[i].second.first == -1)
				not_found = supports[num][pos].second[i].second.second[pos] != val;
			i--;
		}
	}	
	
	if (not_found)
	{
		// we add the support
		if (supports[num][pos].first == (signed) supports[num][pos].second.size())
		{
			supports[num][pos].second.resize (supports[num][pos].second.size()+100,pair<int,pair<int,int*> >(0,pair<int, int*>(-1,0)));
			supports[num][pos].second[supports[num][pos].first].second.second = 0;
		}

		supports[num][pos].second[supports[num][pos].first].first = top;
		if (supports[num][pos].first == max_used_size[num][pos])
		{
			supports[num][pos].second[supports[num][pos].first].second.second = new int [arity];
			max_used_size[num][pos]++;
		}
		
		if (tuple == 0)
			supports[num][pos].second[supports[num][pos].first].second.first = val;
		else
			{
				supports[num][pos].second[supports[num][pos].first].second.first = -1;
				for (int i = 0; i < arity; i++)
					supports[num][pos].second[supports[num][pos].first].second.second[i] = tuple[i];
			}
		supports[num][pos].first++;
		
		// we update the data structure support_change if needed
		if (is_support_changed[num][pos] != top)
		{
			support_change[top].second[support_change[top].first].first = num;
			support_change[top].second[support_change[top].first].second = pos;
			support_change[top].first++;
			is_support_changed[num][pos] = top;
		}
	}
}


inline void Support_Deletion_Stack::Restore (CSP * pb)
// restores the elements at the top level and decreases the top of the stack
/// \param[in] pb the CSP instance in which the values must be restored
{
	if (top != -1)
	{
		while (number_elements[top] != 0)
		{
			size--;
			elements[size].first->Unfilter_Value (elements[size].second);
			number_elements[top]--;
		}

		while (support_change[top].first > 0)
		{
			support_change[top].first--;
			int num = support_change[top].second[support_change[top].first].first;
			int pos = support_change[top].second[support_change[top].first].second;
			int arity = pb->Get_Constraint(num)->Get_Arity();
			
			int i = supports[num][pos].first - 1;
			while ((i >= 0) && (supports[num][pos].second[i].first == top))
			{
				if (supports[num][pos].second[i].second.first == -1)
					ls->Change_Support (num, pos,supports[num][pos].second[i].second.second[pos], supports[num][pos].second[i].second.second, arity);
				else ls->Reset_Support (num, pos,supports[num][pos].second[i].second.first);

				i--;
				supports[num][pos].first--;
			}
			is_support_changed[num][pos] = -1;
		}
		
		top--;
	}
}

#endif
