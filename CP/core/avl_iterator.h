/// \file 
/// \brief Definitions related to the AVL_Iterator class

#ifndef AVL_ITERATOR_H
#define AVL_ITERATOR_H

#include "avl_node.h"

class AVL;

using namespace std;


class AVL_Iterator            /// This class implements iterators on AVL trees \ingroup core
{
	protected:
		AVL * object;							///< the object to which the iterator is related
		AVL_Node * current_pos;		///< the current position in the object
		int variable_pos;					///< the position in the corresponding scope of the variable w.r.t. which we list the tuples
		
		
	public:
		// constructors
		AVL_Iterator ();																									///< constructs an empty iterator
		AVL_Iterator (AVL * o, int var_pos=0, AVL_Node * initial_pos=0);	///< constructs an iterator on the object o for which the current pos is set to initial_pos
		AVL_Iterator (const AVL_Iterator & iter);													///< constructs an iterator by copying the iterator iter
		
				
		// operators
		AVL_Iterator & operator= (const AVL_Iterator & iter);	///< operator =
		AVL_Iterator & operator++ (int);											///< operator ++
		int * operator* ();																		///< operator *
		bool operator== (const AVL_Iterator & iter);					///< operator ==
		bool operator!= (const AVL_Iterator & iter);					///< operator !=
};


//-----------------------------
// inline function definitions 
//-----------------------------


inline int * AVL_Iterator::operator* ()
// operator *
{
	return current_pos->Get_Tuple();	
}


inline bool AVL_Iterator::operator== (const AVL_Iterator & iter)
// operator ==
/// \param[in] iter the iterator to which the current iterator is compared
{
	return (object == iter.object) && (current_pos == iter.current_pos) && (variable_pos == iter.variable_pos);
}


inline bool AVL_Iterator::operator!= (const AVL_Iterator & iter)
// operator !=
/// \param[in] iter the iterator to which the current iterator is compared
{
	return ! ((object == iter.object) && (current_pos == iter.current_pos) && (variable_pos == iter.variable_pos));
}


#endif

