/// \file 
/// \brief Definitions related to the Constraint_Selection class

#ifndef CONSTRAINT_SELECTION_H
#define CONSTRAINT_SELECTION_H

using namespace std;

#include "constraint.h"
#include <iostream>
class Constraint_Selection      /// This class allows to select some constraints \ingroup filtering
{
	public:	
		virtual ~Constraint_Selection()	{}
		// basic function
		virtual bool Is_Eligible (Constraint * c, int var)=0;	 ///< returns true if the constraint num is eligible w.r.t. the variable x of the CSP pb, false otherwise
};
#endif

