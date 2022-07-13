/// \file 
/// \brief Definitions related to the auxiliary functions for graphs
/// \ingroup graph_tools

#ifndef GRAPH_AUXILIARY_FUNCTIONS_H
#define GRAPH_AUXILIARY_FUNCTIONS_H

#include "light_graph.h"

int Width (Light_Graph * g);                     	///< returns the width of the graph g (see [Freuder,1982])

int Nb_Connected_Components (Light_Graph * g);    ///< returns the number of connected components of the graph g

bool Is_Acyclic (Light_Graph * g);								///< returns true if the graph g is acyclic, false otherwise


#endif
