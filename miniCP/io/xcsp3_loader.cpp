#include "xcsp3_loader.h"
#include <XCSP3CoreParser.h>
#include "load_XCSP3Callbacks.h"


//----------------
// basic function
//----------------


void XCSP3_Loader::Load (CSP * pb, string filename)
// loads the CSP pb from the file filename whose format is XML XCSP 3 (core)
{
  ifstream file (filename.c_str(), ios::in);
  
  if (file.fail())  throw ("Error: file "+filename+" not found");
 
  Load_XCSP3Callbacks cb (pb); 	// creates an interface between the parser and the current library

	try
	{
		XCSP3Core::XCSP3CoreParser parser(&cb);
		parser.parse(file);
	}	

	catch (...)
	{
		delete pb;
		throw;
	}
	
	file.close();
}
