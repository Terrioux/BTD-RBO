/// \file 
/// \brief Definitions related to the List_Channel_Global_Constraint class

#ifndef LIST_CHANNEL_GLOBAL_CONSTRAINT_H
#define LIST_CHANNEL_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class List_Channel_Global_Constraint: public Global_Constraint   /// This class implements the channel global constraint based on two lists of variables \ingroup core
{
	protected:
    vector<Variable *> list[2];      ///< the lists of variables
    vector<int> belong_to_list[2];   ///< the l
    int start_index[2];              ///< the value from which the indices start for the each list
		
	public:
		// constructors
		List_Channel_Global_Constraint (vector<Variable *> & var, vector<Variable *> & list_1, int start1, vector<Variable *> & list_2, int start2);		///< constructs a new channel constraint which involves the variable in var and whose start indices are respectively start1 and start2
		List_Channel_Global_Constraint (List_Channel_Global_Constraint & c);		      ///< constructs a new constraint by copying the constraint c
		
		// basic functions
		bool Is_Satisfied (int * t) override = 0;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override = 0;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    string Get_XCSP3_Expression() override;           ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};

#endif
