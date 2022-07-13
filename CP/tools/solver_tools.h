/// \file 
/// \brief Definitions of auxiliary functions related to CSP solvers
/// \ingroup tools


#ifndef SOLVER_TOOLS_H
#define SOLVER_TOOLS_H

#include "csp.h"
#include "solver.h"
#include "filtering_tools.h"
#include "tree_decomposition_tools.h"

namespace solver_tools
{
  Solver * solver_initialization (CSP * pb, Tree_Decomposition * & TD, double time_limit, AC * ac); ///< initialization of the solver
}

#endif
