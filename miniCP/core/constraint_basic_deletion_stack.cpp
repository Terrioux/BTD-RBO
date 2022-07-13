/// \file 
/// \brief Sources related to the Constraint_Basic_Deletion_Stack class

#include "constraint_basic_deletion_stack.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Constraint_Basic_Deletion_Stack::Constraint_Basic_Deletion_Stack (CSP * pb, int level_number): Basic_Deletion_Stack (pb,level_number)
// constructs an empty deletion stack related to the CSP pb with at most level_number levels
/// \param[in] pb the CSP instance to which the stack is related
/// \param[in] level_number the maximal size of the stack
{
	timestamp = 0;
	constraint_last_modified = new unsigned long [pb->Get_M()];
  
  for (int i = pb->Get_M()-1; i >= 0; i--)
		constraint_last_modified[i] = 0;
    
  size_constraint_elements = new unsigned int [level_number];
}


Constraint_Basic_Deletion_Stack::~Constraint_Basic_Deletion_Stack()
// destructor
{
	delete [] constraint_last_modified;
	delete [] size_constraint_elements;
}

