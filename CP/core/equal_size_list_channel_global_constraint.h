/// \file 
/// \brief Definitions related to the Equal_Size_List_Channel_Global_Constraint class

#ifndef EQUAL_SIZE_LIST_CHANNEL_GLOBAL_CONSTRAINT_H
#define EQUAL_SIZE_LIST_CHANNEL_GLOBAL_CONSTRAINT_H

#include "list_channel_global_constraint.h"


class Equal_Size_List_Channel_Global_Constraint: public List_Channel_Global_Constraint   /// This class implements the channel global constraint  \ingroup core
{
	public:
		// constructors 
		Equal_Size_List_Channel_Global_Constraint (vector<Variable *> & var, vector<Variable *> & list_1, int start1, vector<Variable *> & list_2, int start2);		///< constructs a new channel constraint which involves the variable in var and whose start indices are respectively start1 and start2
		Equal_Size_List_Channel_Global_Constraint (Equal_Size_List_Channel_Global_Constraint & c);		      ///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 	  ///< returns true if the tuple t satisfies the constraint, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Equal_Size_List_Channel_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Equal_Size_List_Channel_Global_Constraint (*this);
}

#endif
