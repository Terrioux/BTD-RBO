/// \file 
/// \brief Definitions related to the Labeled_Lexicographic_Tree class

#ifndef LABELED_LEXICOGRAPHIC_TREE_H
#define LABELED_LEXICOGRAPHIC_TREE_H

using namespace std;

#include "statistical_counter.h"

template <class T>
class Labeled_Lexicographic_Tree        /// This class implements labeled lexicographic trees \ingroup decomposition
{
	protected:
		int value;																						///< the value related to the node
		T * label;																						///< the label of the branch
		list<Labeled_Lexicographic_Tree<T> *> children;	///< the childrens of the node
		
	public:
		// constructor and destructor
		Labeled_Lexicographic_Tree ();					///< creates an empty tree
		Labeled_Lexicographic_Tree (int val);		///< creates a tree with a single node containing the value val
		~Labeled_Lexicographic_Tree ();					///< destructor
		
		// basic functions
		bool Is_Element (int * t, int arity);										///< returns true if the tuple t appears in the tree, false otherwise
		T * Get_Label (int * t, int arity);											///< returns the label associated to the tuple t (if t appears in the tree), 0 otherwise
		T * Add_Element (int * t,int arity, T & label);					///< adds the tuple t with its label to tree and returns a pointer on the added label
		void Clear ();																					///< removes all the elements from the tree
};


#include "labeled_lexicographic_tree.tpp"

#endif
