/// \file 
/// \brief Definitions related to the R_Sparse_Bitset class

#ifndef R_SPARSE_BITSET_H
#define R_SPARSE_BITSET_H

#include<vector>
#include "constraint.h"

/// the size of each word
#define WORD_SIZE (8 * sizeof(unsigned long))

class R_Sparse_Bitset    /// This class allows to represent reversible sparse bitset \ingroup core
{		
  protected:
    Constraint * c;                   ///< the constraint to which the bitset is related
    unsigned int card;                ///< the size of the bitset
    vector<unsigned long> words;      ///< the current values of the bitset
    vector<int> index;                ///< the position of each non-zero words
    int limit;                        ///< the largest index corresponding to non-zero words
    vector<unsigned long> mask;       ///< the mask used for bitset operation
    
    unsigned long last_word_modif;                ///< the time of the last modification of a word
    vector<unsigned long> last_offset_modif;      ///< the time of the last modification of a word
    vector<pair<int,unsigned long>> word_backup;  ///< the backup of words before their modification
    vector<unsigned int> last_size_word_backup;   ///< the backup of the size of each word before its modification
  
	public:			
		// constructor
		R_Sparse_Bitset (Constraint * ref_c);				///< constructor

    // operator
    R_Sparse_Bitset & operator= (R_Sparse_Bitset & set);    ///< assigns the current bitset with the bitset set
    
		// basic functions
    void Add_Elements (unsigned int k);                     ///< adds k elements to the bitset
		bool Is_Empty ();   				   				    		          ///< returns true if the set is empty, false otherwise
		void Clear_Mask ();	                 					          ///< resets each bit of the mask to the value 0
		void Reverse_Mask ();	                 			          	///< applies the bitwise not to each bit of the mask
		void Add_To_Mask (vector<unsigned long> & m);       	  ///< applies the bitwise or to each bit of the mask and m
		void Intersect_With_Mask (Deletion_Stack * ds);	        ///< applies the bitwise and to each bit of the mask and words
		int Intersect_Index (vector<unsigned long> & m);	      ///< returns the index of a word where bitset intersects with m, -1 otherwise
    unsigned long Get_Word (int index);                     ///< returns the word at position index
    void Restore ();                                        ///< restores the bitset in its previous state
    int Get_Limit ();                                       ///< returns the current limit
    unsigned int One_Number ();                             ///< returns the number of 1 in the bitset
    unsigned int One_Number (vector<unsigned long> & m);    ///< returns the number of 1 in the intersection between the bitset and m
};



//--------------------------------
// definition of inline functions
//--------------------------------


inline bool R_Sparse_Bitset::Is_Empty ()
// returns true if the set is empty, false otherwise
{
  return limit == -1;
}


inline unsigned long R_Sparse_Bitset::Get_Word (int index)
// returns the word at position index
{
  return words[index];
}


inline int R_Sparse_Bitset::Get_Limit ()
// returns the current limit
{
  return limit;
}

#endif
