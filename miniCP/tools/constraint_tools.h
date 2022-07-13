/// \file 
/// \brief Definitions of auxiliary functions for managing constraints
/// \ingroup tools


#ifndef CONSTRAINT_TOOLS_H
#define CONSTRAINT_TOOLS_H

#include "csp.h"
#include "predicate_constraint.h"

Constraint * Predicate_To_Extension (Predicate_Constraint * c);				///< returns an extension constraint corresponding to the predicate constraint c

void All_In_Extension (CSP * pb, int max_arity);				///< replaces the constraints in intention whose arity is not greater than max_arity by a corresponding constraint in extenson

double Get_Estimated_Tightness (Constraint * c, unsigned int sample_size);     ///< returns an estimation of the tightness of constraint c by sampling over sample_size assignments

#endif
