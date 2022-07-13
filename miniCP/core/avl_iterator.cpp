/// \file 
/// \brief Sources related to the AVL_Iterator class

#include "avl_iterator.h"


//--------------
// constructors
//--------------

AVL_Iterator::AVL_Iterator ()
// constructs an empty iterator
{
	object = 0;
	variable_pos = 0;
	current_pos = 0;
}


AVL_Iterator::AVL_Iterator (AVL * o, int var_pos,AVL_Node * initial_pos)
// constructs an iterator on the object o for which the current pos is set to initial_pos
/// \param[in] o the AVL to which the current iterator is related
/// \param[in] var_pos the position of the variable to which the current iterator is related
/// \param[in] initial_pos the node from which the current iterator is built
{
	object = o;
	variable_pos = var_pos;
	current_pos = initial_pos;
}


AVL_Iterator::AVL_Iterator (const AVL_Iterator & iter)
// constructs an iterator by copying the iterator iter
/// \param[in] iter the iterator we want to copy
{
	object = iter.object;
	variable_pos = iter.variable_pos;
	current_pos = iter.current_pos;
}


//-----------
// operators
//-----------


AVL_Iterator & AVL_Iterator::operator= (const AVL_Iterator & iter)
// operator =
/// \param[in] iter the iterator we want to assign
{
	if (this != &iter)
	{
		object = iter.object;
		variable_pos = iter.variable_pos;
		current_pos = iter.current_pos;
	}
	return *this;
}


AVL_Iterator & AVL_Iterator::operator++ (int)
// operator ++
{
	if (current_pos != 0)
	{
		current_pos = current_pos->Next_Tuple (variable_pos);
	}
	return *this;
}
