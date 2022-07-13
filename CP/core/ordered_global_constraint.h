/// \file 
/// \brief Definitions related to the Ordered_Global_Constraint class

#ifndef ORDERED_GLOBAL_CONSTRAINT_H
#define ORDERED_GLOBAL_CONSTRAINT_H

#include "global_constraint.h"

class Ordered_Global_Constraint: public Global_Constraint      /// This class implements the ordered global constraint \ingroup core
{
  protected:
    bool or_equal;        ///< true if the relation is not a strict inequality, false otherwise
    vector<int> lengths;  ///< the minimum distances between two consecutive variables
  
	public:
		// constructors and destructor
		Ordered_Global_Constraint (vector<Variable *> & var, vector<int> & len, bool equal);  ///< constructs a new ordered global constraint
		Ordered_Global_Constraint (Ordered_Global_Constraint & c);	       ///< constructs a new constraint by copying the constraint c
				
		// basic functions
		bool Is_Satisfied (int * t) override;	 		   	///< returns true if the tuple t satisfies the constraint, false otherwise
    virtual bool Compare (int val_1, int val_2) = 0;   ///< returns true if the comparison between val_1 and val_2 holds, false otherwise
    virtual string Get_Relation () = 0;           ///< returns the relation used to compare two values
    bool Impacted_By_Last_Events (CSP * pb,timestamp ref) override;     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
    string Get_XCSP3_Expression() override;       ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline bool Ordered_Global_Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  if (ref == 0)
    return true;
  
  Event_Manager * event_manager = pb->Get_Event_Manager();
  for (set<unsigned int>::iterator iter = scope.begin(); iter != scope.end(); iter++)
    if ((event_manager->Exist_Event_Lbc (*iter,ref)) || (event_manager->Exist_Event_Ubc (*iter,ref)))
      return true;
  
  return false;
}
#endif

