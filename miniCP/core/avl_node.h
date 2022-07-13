/// \file 
/// \brief Definitions related to the AVL_Node class

#ifndef AVL_NODE_H
#define AVL_NODE_H

#include "variable.h"
#include <vector>


class AVL_Node          /// This class allows to represent the nodes of a AVL tree \ingroup core
{
	private:
		int * value;				///< the value of the current node of the AVL
		int imbalance; 			///< the imbalance of the current node of the AVL (an integer between -1 and 1)
		AVL_Node * lchild;	///< the left child of the current node of the AVL
		AVL_Node * rchild;	///< the right child of the current node of the AVL
		AVL_Node ** next;		///< the next tuple for each value of the current node
		AVL_Node ** prev;		///< the previous tuple for each value of the current node
	
	public:
		// constructor and destructor
		AVL_Node (int * val, int arity, AVL_Node *** & tuple_list);			///< constructs a node with no child and a value val
		~AVL_Node ();																										///< destructor
		
		// basic functions
		bool Add_Element (AVL_Node * & A, int * val, int arity, AVL_Node *** & tuple_list);			///< adds the element val into the tree pointed by A (if it does not belong yet)
		bool Remove_Element (AVL_Node * & A, int * val, int arity, AVL_Node *** & tuple_list);	///< removes the element val into the tree pointed by A (if it belongs)
		bool Is_Present (int * val, int arity);											///< returns true if the element val belongs to the AVL, false otherwise
		AVL_Node * Get_Node (int * val, int arity);									///< returns a pointer on the node containing the element val if it belongs to the AVL, 0 otherwise
		void Print (int arity); 																		///< prints the content of the node and of the corresponding subtree
		int * Get_Tuple ();																					///< returns the current tuple
		AVL_Node * Next_Tuple (int pos);														///< returns the next tuple w.r.t. the variable at the position pos in the tuple
		int Height ();																							///< returns the height of the subtree rooted in the current node
	
	private:
		bool Balance_Left (AVL_Node * & A);		///< rebalances the AVL after a deletion in the left child
		bool Balance_Right (AVL_Node * & A);	///< rebalances the AVL after a deletion in the right child
};


inline int * AVL_Node::Get_Tuple ()
// returns the current tuple
{
	return value;
}


inline AVL_Node * AVL_Node::Next_Tuple (int pos)
// returns the next tuple w.r.t. the variable at the position pos in the tuple
/// \param[in] pos the position of the variable in the tuple
{
	return next[pos];
}

#endif
