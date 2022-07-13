/// \file 
/// \brief Definitions related to the Channel_Global_Constraint class

#ifndef CHANNEL_GLOBAL_CONSTRAINT_H
#define CHANNEL_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"


class Channel_Global_Constraint: public Global_Constraint   /// This class implements the channel global constraint  \ingroup core
{
	protected:
    int start_index;      ///< the value from which the indices start
		
	public:
		// constructors 
		Channel_Global_Constraint (vector<Variable *> & var, int start);		///< constructs a new channel constraint which involves the variable in var and whose start index is start
		Channel_Global_Constraint (Channel_Global_Constraint & c);		      ///< constructs a new constraint by copying the constraint c
		
		// basic functions
		Constraint * Duplicate () override; 					///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	   		 	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	   ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Channel_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Channel_Global_Constraint (*this);
}

#endif
