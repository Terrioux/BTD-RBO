#include <type_traits>
#include <typeinfo>
#include <gmpxx.h>

//----------------------------
// constructor and destructor
//----------------------------


template <class T>
Labeled_Lexicographic_Tree<T>::Labeled_Lexicographic_Tree()
// creates an empty tree
{
	value = -1;
	label = 0;
}


template <class T>
Labeled_Lexicographic_Tree<T>::Labeled_Lexicographic_Tree (int val)
// creates a tree with a single node containing the value val
{
	value = val;
	label = 0;
}


template <class T>
Labeled_Lexicographic_Tree<T>::~Labeled_Lexicographic_Tree ()
// destructor
{
	for (typename list<Labeled_Lexicographic_Tree<T> *>::iterator iter = children.begin(); iter != children.end(); iter++)
		delete (*iter);

	delete label;
}


//-----------------
// basic functions
//-----------------


template <class T>
bool Labeled_Lexicographic_Tree<T>::Is_Element (int * t, int arity)
// returns true if the tuple t appears in the tree, false otherwise
{
	int i = 0;
	Labeled_Lexicographic_Tree<T> * p = this;
	typename list<Labeled_Lexicographic_Tree<T> *>::iterator iter; 
	do
	{
		iter = p->children.begin();
		while ((iter != p->children.end()) && ((*iter)->value != t[i]))
			iter++;
			
		if (iter == p->children.end())
			return false;
		
		p = *iter;
		i++;
	}
	while (i < arity);
	
	return true;
}


template <class T>
T * Labeled_Lexicographic_Tree<T>::Get_Label (int * t, int arity)
// returns the label associated to the tuple t (it t appears in the hash table), 0 otherwise
{
	int i = 0;
	Labeled_Lexicographic_Tree<T> * p = this;
	typename list<Labeled_Lexicographic_Tree<T> *>::iterator iter; 
	do
	{
		iter = p->children.begin();
		while ((iter != p->children.end()) && ((*iter)->value != t[i]))
			iter++;
			
		if (iter == p->children.end())
			return 0;
		
		p = *iter;
		i++;	
	}
	while (i < arity);
	return p->label;
}


template <class T>
T * Labeled_Lexicographic_Tree<T>::Add_Element (int * t, int arity, T & label)
// adds the tuple t with its label to the hash table and returns a pointer on the added label
{
	int i = 0;
	Labeled_Lexicographic_Tree<T> * p = this;
	typename list<Labeled_Lexicographic_Tree<T> *>::iterator iter; 
	do
	{
		iter = p->children.begin();
		while ((iter != p->children.end()) && ((*iter)->value != t[i]))
			iter++;
			
		if (iter == p->children.end())
		{
			Labeled_Lexicographic_Tree<T> * q = new Labeled_Lexicographic_Tree<T> (t[i]);
			p->children.push_back (q);
			p = q;
		}
		else p = *iter;
		i++;	
	}
	while (i < arity);
	p->label = new T (label);
	return p->label;
}


template <class T>
void Labeled_Lexicographic_Tree<T>::Clear ()
// removes all the elements of the hash table
{
	for (typename list<Labeled_Lexicographic_Tree<T> *>::iterator iter = children.begin(); iter != children.end(); iter++)
		(*iter)->Clear();
	
	value = -1;
	delete label;
	label = 0;
}
