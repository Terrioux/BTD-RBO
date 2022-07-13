/// \file 
/// \brief Definitions related to the All_Constraint_Selection class

#ifndef ALL_CONSTRAINT_SELECTION_H
#define ALL_CONSTRAINT_SELECTION_H

using namespace std;

#include "constraint_selection.h"

class All_Constraint_Selection: public Constraint_Selection   /// This class allows to select all the constraints \ingroup filtering
{
	public:	
		// basic function
		bool Is_Eligible (Constraint * c, int var) override;	 ///< returns true if the constraint num is eligible w.r.t. the variable x of the CSP pb, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline bool All_Constraint_Selection::Is_Eligible (Constraint * c, int var)
// returns true if the constraint num is eligible w.r.t. the variable x of the CSP pb, false otherwise
{
	return c->Get_Unassigned_Variable_Number() != 0;
}
#endif

