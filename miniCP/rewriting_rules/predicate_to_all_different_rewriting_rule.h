/// \file 
/// \brief Definitions related to the Predicate_To_All_Different_Rewriting_Rule class

#ifndef PREDICATE_TO_ALL_DIFFERENT_REWRITING_RULE_H
#define PREDICATE_TO_ALL_DIFFERENT_REWRITING_RULE_H

#include "rewriting_rule.h"

class Predicate_To_All_Different_Rewriting_Rule: public Rewriting_Rule      /// This class implements the rule which rewrites predicates as all-different global constraints \ingroup rewriting
{				
	public:	
		// basic functions
		bool Rewrite (CSP * pb, int num); 				///< returns true is the rewriting rule has been applied on the contraint whose number is num in the instance pb, false otherwise
};

#endif
