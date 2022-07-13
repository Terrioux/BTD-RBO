/// \file 
/// \brief Definitions related to the Less_Ordered_Global_Constraint class

#ifndef LESS_ORDERED_GLOBAL_CONSTRAINT_H
#define LESS_ORDERED_GLOBAL_CONSTRAINT_H

#include "ordered_global_constraint.h"

class Less_Ordered_Global_Constraint: public Ordered_Global_Constraint      /// This class implements the ordered global constraint with operator < or <= \ingroup core
{
	public:
		// constructors and destructor
		Less_Ordered_Global_Constraint (vector<Variable *> & var, vector<int> & len, bool equal);  ///< constructs a new ordered global constraint
		Less_Ordered_Global_Constraint (Less_Ordered_Global_Constraint & c);	  ///< constructs a new constraint by copying the constraint c
				
		// basic functions
    Constraint * Duplicate () override;   	///< returns a pointer on a copy of the constraint
    bool Compare (int val_1, int val_2);    ///< returns true if the comparison between val_1 and val_2 holds, false otherwise
    string Get_Relation () override;        ///< returns the relation used to compare two values
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	  ///< applies the event-based propagator of the constraint by considering the events occurred since ref
};


//-----------------------------
// inline function definitions
//-----------------------------

inline Constraint * Less_Ordered_Global_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Less_Ordered_Global_Constraint (*this);
}


inline bool Less_Ordered_Global_Constraint::Compare (int val_1, int val_2)
// returns true if the comparison between val_1 and val_2 holds, false otherwise
{
  return or_equal ? val_1 <= val_2 : val_1 < val_2;  
}


inline string Less_Ordered_Global_Constraint::Get_Relation ()
{
  return or_equal ? "le": "lt";
}
#endif

