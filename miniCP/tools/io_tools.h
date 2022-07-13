/// \file 
/// \brief Definitions of input/output auxiliary functions
/// \ingroup tools


#ifndef IO_TOOLS_H
#define IO_TOOLS_H

#include "csp.h"


// i/o functions

void Load (CSP * pb, string filename);				///< loads the CSP pb from the file filename 

void Print_Solution (CSP * pb, Assignment & solution);            ///< print the solution in the XCSP3 competition format
void Print_XCSP3_Solution (CSP * pb, Assignment & solution);      ///< print the solution in the XCSP3 competition format

#endif
