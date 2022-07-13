/// \file 
/// \brief Sources related to the Basic_Deletion_Stack class

#include "basic_deletion_stack.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Basic_Deletion_Stack::Basic_Deletion_Stack (CSP * pb, int level_number) : Deletion_Stack(), elements (1000)
// constructs an empty deletion stack related to the CSP pb with at most level_number levels
/// \param[in] pb the CSP instance to which the stack is related
/// \param[in] level_number the maximal size of the stack
{
	timestamp = 0;
	last_modified = new unsigned long [pb->Get_N()];
  
  for (int i = pb->Get_N()-1; i >= 0; i--)
		last_modified[i] = 0;

	number_elements = new int [level_number];
	top = -1;
	size = 0;
}


Basic_Deletion_Stack::~Basic_Deletion_Stack()
// destructor
{
	delete [] number_elements;
	delete [] last_modified;
}

