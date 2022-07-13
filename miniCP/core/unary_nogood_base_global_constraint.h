/// \file 
/// \brief Definitions related to the Unary_Unary_Nogood_Base_Global_Constraint class

#ifndef UNARY_UNARY_NOGOOD_BASE_GLOBAL_CONSTRAINT_H
#define UNARY_UNARY_NOGOOD_BASE_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class Unary_Nogood_Base_Global_Constraint: public Global_Constraint      /// This class implements the unary nogood base constraint \ingroup core
{
	protected:
    unsigned int n;                         ///< the maximum number of variables 
    set<int> * unary_nogood_base;			      ///< the base containing unary nogoods
    	
	public:
		// constructors and destructor
		Unary_Nogood_Base_Global_Constraint (); 	///< constructs a new constraint which initially involves no variable and is universal
		Unary_Nogood_Base_Global_Constraint (Unary_Nogood_Base_Global_Constraint & c);		 	///< constructs a new constraint by copying the constraint c 
		~Unary_Nogood_Base_Global_Constraint();				///< destructor
		
    // operator
    Unary_Nogood_Base_Global_Constraint & operator= (Unary_Nogood_Base_Global_Constraint & c);   ///< operator =
    		
		// basic functions
		Constraint * Duplicate () override; 					  				///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 			  			///< returns true if the tuple t satisfies the constraint, false otherwise
 		void Add_Nogood (CSP * pb, unsigned int var, int val);	///< adds the unary nogood ng involving the variable var and the value val into the nogood base related to the instance pb
    void Reset (CSP * pb);                                  ///< resets the nogood base
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Unary_Nogood_Base_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Unary_Nogood_Base_Global_Constraint (*this);
}

#endif
