/// \file 
/// \brief Definitions related to the Nogood_Base_Global_Constraint class

#ifndef NOGOOD_BASE_GLOBAL_CONSTRAINT_H
#define NOGOOD_BASE_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class Nogood_Base_Global_Constraint: public Global_Constraint      /// This class implements the nogood base constraint \ingroup core
{
	protected:
		typedef struct elt                   /// allows to represent nogoods
		{
			pair<unsigned int,int> * nogood;   ///< the pairs <variable,value> which form the nogoods
			int size;                          ///< the number of pairs (i.e. the size of the nogood)
			int watch1;                        ///< the first watched literal
			int watch2;                        ///< the second watched literal
		} element;                           ///< allows to represent nogoods with watched literals
    unsigned int n;                      ///< the maximum number of variables 
		list<element *> ** nary_nogood_base; ///< the base containing nogoods of size 2 or more
    	
	public:
		// constructors and destructor
		Nogood_Base_Global_Constraint (); 	///< constructs a new constraint which initially involves no variable and is universal
		Nogood_Base_Global_Constraint (Nogood_Base_Global_Constraint & c);		 	///< constructs a new constraint by copying the constraint c 
		~Nogood_Base_Global_Constraint();				///< destructor
    
    
		// basic functions
		Constraint * Duplicate () override; 							  	///< returns a pointer on a copy of the constraint
		bool Is_Satisfied (int * t) override;	 		 						///< returns true if the tuple t satisfies the constraint, false otherwise
 		void Add_Nogood (CSP * pb, pair <int,int> * ng,int size);										///< adds the nogood ng involving size variables into the nogood base and related to the instance pb
		void Add_Nogood (CSP * pb, pair <int,int> * ng,int size, Assignment & A);		///< adds the nogood ng involving size variables into the nogood base and related to the instance pb, and selects the watch literals depending on the assignment A 
    void Reset (CSP * pb);                                ///< resets the nogood base
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	      ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    bool Impacted_By_Last_Events (CSP * pb, timestamp ref) override;            ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Constraint * Nogood_Base_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Nogood_Base_Global_Constraint (*this);
}


inline bool Nogood_Base_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  for (set<unsigned int>::iterator iter = scope.begin(); iter != scope.end(); iter++)
    if (event_manager->Exist_Event_Fix (*iter,ref))
      return true;
  
  return false;
}

#endif
