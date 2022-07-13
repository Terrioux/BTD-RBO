/// \file 
/// \brief Definitions related to the XCSP3_Loader class

#ifndef XCSP3_LOADER_H
#define XCSP3_LOADER_H

#include "csp.h"

class XCSP3_Loader         /// This class allows to read from files CSP instances expressed in the XCSP 3 format \ingroup io
{
	public:
		// basic functions
		void Load (CSP * pb, string filename);		  ///< loads the CSP from the file filename whose format is XML XCSP 3 (core)
};
#endif
