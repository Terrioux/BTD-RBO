/// \file 
/// \brief Definitions related to the AC class

#ifndef AC_H
#define AC_H

using namespace std;

#include "support.h"
#include "constraint_selection.h"
#include "csp.h"
#include "assignment.h"
#include "filtering_data_structure.h"

class AC               /// This class allows to represent AC algorithms \ingroup filtering
{
	protected:
		Constraint_Selection * selection;			///< choice of a policy of constraint selection
		Support * supports;										///< the structure of the supports
 		Filtering_Data_Structure<int> * structure; 	///< the data structure representing the set of the constraints for which the consistency must be checked
		vector<bool> is_selected;	                  ///< true if the constraint c is in the structure, false otherwise
    vector<timestamp> last_check;               ///< specifies, for each constraint, the last time it has been checked

	public:
		AC (CSP * pb, Support * ls, Constraint_Selection * cs, Filtering_Data_Structure<int> * acds);	///< constructs an AC algorithm using support structure ls, the constraint selection policy cs and the AC data structure acds which will be achieved until the fix-point is reached otherwise
		~AC ();			///< destructor
		
		// basic functions
		Support * Get_Support ();                                   ///< returns a pointer on the structure representing the supports
		bool Achieve (CSP * pb, Assignment & A, Deletion_Stack * ds, int var);	///< achieves, from the variable var (if any), the current AC algorithm on the CSP pb under the assignment A and returns true if the problem is consistent w.r.t. this algorithm, false otherwise (the deletion of values are recorded in the stack ds)
    bool Achieve (CSP * pb);																		          		///< achieves the current filtering algorithm on the CSP pb and returns true if the problem is consistent w.r.t. this algorithm, false otherwise 
};


//-------------------------------
// definition of inline function
//-------------------------------


inline Support * AC::Get_Support ()
// returns a pointer on the structure representing the supports
{
	return supports;
}




#endif

