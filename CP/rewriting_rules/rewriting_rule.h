/// \file 
/// \brief Definitions related to the Rewriting_Rule class

#ifndef REWRITING_RULE_H
#define REWRITING_RULE_H

#include "csp.h"

class Rewriting_Rule      /// This class allows to represent rewriting rules \ingroup rewriting
{				
	public:	
		// constructor and destructor
		virtual ~Rewriting_Rule(){};    ///< destructor
		
		// basic functions
		virtual bool Rewrite (CSP * pb, int num) = 0; 				///< returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
};

#endif
