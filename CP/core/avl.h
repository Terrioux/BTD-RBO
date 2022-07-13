/// \file 
/// \brief Definitions related to the AVL class

#ifndef AVL_H
#define AVL_H

#include "avl_node.h"
#include "variable.h"
#include <vector>
#include "avl_iterator.h"

class AVL                     /// This class allows to represent AVL trees \ingroup core
{	
	protected:	
		AVL_Node * root;					///< the root of the AVL
		AVL_Node *** tuple_list;	///< the list of tuples for each pair (variable,value) 
		int arity; 								///< the arity of the tuple in the tree
		unsigned long size;				///< the size of the AVL
	
	public:
		// constructors and destructor
		AVL (vector<Variable *> & var);		  	///< constructs an empty AVL for the tuples over the Cartesian products of the domains of the variables in var
		~AVL ();													  	///< destructor

		// iterators
		typedef AVL_Iterator iterator;                  ///< defines the type representing iterators on AVL
		AVL::iterator begin (int var_pos, int val);			///< returns an iterator referring to the first element of the AVL for the variable at position var_pos in the associated scope and for the value val
		AVL::iterator begin (int var_pos, int * tuple);	///< returns an iterator referring to the element of the AVL containing tuple for the variable at position var_pos in the associated scope
		AVL::iterator end (int var_pos);					 			///< returns an iterator referring to the position after the last element of the AVL for the variable at position var_pos in the associated scope
		
		
		// basic functions
		bool Is_Empty ();								///< returns true if the AVL is empty, false otherwise
		unsigned long Get_Size ();			///< returns the size of the AVL (i.e. the number of elements it contains)
		void Add_Element (int * val);		///< adds the element val into the AVL (if it does not belong to the AVL yet)
		void Remove_Element (int * val);///< removes the element val into the AVL (if it belongs to the AVL)
		bool Is_Present (int * val);		///< returns true if the element val belongs to the AVL, false otherwise
		AVL_Node * Get_First_Tuple (int pos, int val);	///< returns the first node for the variable at position pos and for the val
		void Generate_Complement (vector<Variable *> & var, AVL & C); ///< computes the AVL C corresponding to the complement of the set represented by the current AVL over the Cartesian product of the domains of the variables in var
		int Height ();									///< returns the height of the AVL
		void Print (); 									///< prints the content of the AVL
};



//--------------------------------------
// inline iterator function definitions 
//--------------------------------------


inline AVL::iterator AVL::begin (int var_pos, int val)
// returns an iterator referring to the first element of the AVL for the variable at position var_pos in the associated scope and for the value val
/// \param[in] var_pos the position of the variable to which the wanted iterator refers 
/// \param[in] val the value to which the wanted iterator refers 
{
	return AVL_Iterator (this,var_pos,tuple_list[var_pos][val]);
}


inline AVL::iterator AVL::begin (int var_pos, int * tuple)
// returns an iterator referring to the element of the AVL containing tuple for the variable at position var_pos in the associated scope
/// \param[in] var_pos the position of the variable to which the wanted iterator refers 
/// \param[in] tuple the tuple to which the wanted iterator refers 
{
	return AVL_Iterator (this,var_pos,root->Get_Node (tuple,arity));
}


inline AVL::iterator AVL::end (int var_pos)
// returns an iterator referring to the position after the last element of the AVL for the variable at position var_pos in the associated scope
/// \param[in] var_pos the position of the variable to which the wanted iterator refers 
{
	return AVL_Iterator (this,var_pos);
}


//-----------------------------------
// inline basic function definitions 
//-----------------------------------


inline bool AVL::Is_Empty ()
// returns true if the AVL is empty, false otherwise
{
	return root == 0;
}


inline unsigned long AVL::Get_Size ()
// returns the size of the AVL (i.e. the number of elements it contains)
{
	return size;
}	


inline void AVL::Add_Element (int * val)
// adds the element val into the AVL (if it does not belong to the AVL yet)
/// \param[in] val the tuple we want to add
{
	size++;
	if (root == 0)  root = new AVL_Node (val,arity,tuple_list);
	else root->Add_Element (root,val,arity,tuple_list);
}


inline void AVL::Remove_Element (int * val)
// removes the element val into the AVL (if it belongs to the AVL)
/// \param[in] val the tuple we want to remove
{
	size--;
	root->Remove_Element (root,val,arity,tuple_list);
}


inline bool AVL::Is_Present (int * val)
// returns true if the element val belongs to the AVL, false otherwise
/// \param[in] val the tuple whose belonging we want to test
{
	if (root == 0) return 0;
	else return root->Is_Present (val,arity);
}


inline AVL_Node * AVL::Get_First_Tuple (int pos, int val)
// returns the first node for the variable at position pos and for the val
/// \param[in] pos the position of the variable to which the wanted tuple refers 
/// \param[in] val the value to which the wanted tuple refers 
{
	return tuple_list[pos][val];
}


inline void AVL::Print ()
// prints the content of the AVL
{
	if (root != 0) root->Print (arity);
}


inline int AVL::Height ()
// returns the height of the AVL
{
	if (root == 0) return 0;
	return root->Height();
}
#endif
