#include "io_tools.h"

// loaders
#include "xcsp3_loader.h"


#include <iostream>
#include "assignment.h"


void Load (CSP * pb, string filename)
// loads the CSP pb from the file filename which is encoded according to format
// If optimization is true, the unary constraints are not taken into account (format WCSP only)
{
	XCSP3_Loader L;
	L.Load(pb,filename);
}

void Print_Solution (CSP * pb, Assignment & solution)
// print the solution in the XCSP3 competition format
{
  #ifdef STAT
    solution.Print();
  #else
  for (unsigned int i = 0; i < pb->Get_N(); i++)
    cout << "x" << pb->Get_Variable(i)->Get_Name() << " = " << pb->Get_Variable(i)->Get_Domain()->Get_Real_Value (solution.Get_Value (i))	 << endl;
  #endif  
}


void Print_XCSP3_Solution (CSP * pb, Assignment & solution)
// print the solution in the XCSP3 competition format
{
  cout << "v <instantiation type=\"solution\">" << endl;
  cout << "v <list>";
  for (unsigned int i = 0; i < pb->Get_N(); i++)
    if (! pb->Get_Variable(i)->Is_Auxiliary())
      cout << " " << pb->Get_Variable(i)->Get_Name();
  cout << " </list>" << endl;
  cout << "v <values>";
  for (unsigned int i = 0; i < pb->Get_N(); i++)
    if (! pb->Get_Variable(i)->Is_Auxiliary())
      cout << " " << pb->Get_Variable(i)->Get_Domain()->Get_Real_Value (solution.Get_Value (i));
  cout << " </values>" << endl;
  cout << "v </instantiation>" << endl;  
}
