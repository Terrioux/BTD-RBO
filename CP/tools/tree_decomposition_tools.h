/// \file 
/// \brief Definitions of auxiliary functions for managing tree-decompositions
/// \ingroup tools


#ifndef TREE_DECOMPOSITION_TOOLS_H
#define TREE_DECOMPOSITION_TOOLS_H

#include "csp.h"
#include "root_heuristic.h"
#include "cluster_heuristic.h"
#include "variable_heuristic.h"
#include <tree_decomposition.h>

Root_Heuristic * Root_Heuristic_Initialization (int num, Variable_Heuristic *vh);  ///< returns the root cluster heuristic corresponding to the number num

Cluster_Heuristic * Next_Cluster_Heuristic_Initialization ();	///< returns the next cluster heuristic

Tree_Decomposition * Tree_Decomposition_Initialization (CSP * pb); ///< initialization of the tree-decomposition

#endif
