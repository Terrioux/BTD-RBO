/// \file 
/// \brief Definitions related to the Weighted_R_Sparse_Set class

#ifndef WEIGHTED_R_SPARSE_SET_H
#define WEIGHTED_R_SPARSE_SET_H

#include<vector>
#include<stack>

template<class T>
class Weighted_R_Sparse_Set    /// This class allows to represent reversible sparse set \ingroup core
{		
  protected:
    vector<T> values;                     ///< the real values of the set
    vector<unsigned int> pos;             ///< the position of each value in the set
    vector<unsigned int> weight;          ///< the weight of each value in the set
    unsigned int card;                    ///< the size of the set
    
    unsigned long last_modif;             ///< the last time a value has been deleted from the set
    stack<vector<unsigned int>*> size_backup;    ///< the backup of the size of the set
  
	public:			
		// constructor
		Weighted_R_Sparse_Set ();	              			///< constructor

		// basic functions
    bool Is_Empty ();   				   				    	      ///< returns true if the set is empty, false otherwise
		void Add (T val);                                 ///< adds the value val to the set
    bool Remove (T val, Deletion_Stack * ds);	        ///< // removes the value val from the set and returns true if the deletion is performed
    T Get_Value (unsigned int i);                     ///< returns the ith value of the set
    void Restore ();                                  ///< restores the set in its previous state
    unsigned int Get_Size ();                         ///< returns the current size of the set
    void Clear (Deletion_Stack * ds);                 ///< removes all the values from the set
};


#include "weighted_r_sparse_set.tpp"

#endif

