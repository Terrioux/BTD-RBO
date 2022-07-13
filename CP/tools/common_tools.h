/// \file 
/// \brief Definitions of auxiliary functions related to both CSP and #CSP solvers
/// \ingroup tools


#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

#include "csp.h"
#include "restart_policy.h"
#include "variable_heuristic.h"
#include "value_heuristic.h"
#include "ac.h"

Restart_Policy * Restart_Policy_Initialization (CSP * pb);		///< initialization of the restart policy

Variable_Heuristic * Variable_Heuristic_Initialization (CSP * pb);     ///< initialization of the variable heuristic depending on the chosen options

Value_Heuristic * Value_Heuristic_Initialization (CSP * pb);         ///< initialization of the value heuristic depending on the chosen options

#endif
