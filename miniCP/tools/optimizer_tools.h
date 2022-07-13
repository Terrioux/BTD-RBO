/// \file 
/// \brief Definitions of auxiliary functions related to CSP solvers
/// \ingroup tools


#ifndef OPTIMIZER_TOOLS_H
#define OPTIMIZER_TOOLS_H

#include "cop.h"
#include "restart_based_optimizer.h"
#include "filtering_tools.h"
#include "tree_decomposition_tools.h"

namespace optimizer_tools
{
  Restart_Based_Optimizer * optimizer_initialization (COP * pb, double time_limit, AC * ac); ///< initialization of the solver
}

#endif
