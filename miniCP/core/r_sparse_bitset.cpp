#include "r_sparse_bitset.h"
#include <cmath>


//-------------
// constructor
//-------------


R_Sparse_Bitset::R_Sparse_Bitset (Constraint * ref_c)
// constructor
{
  c = ref_c;
  card = 0;
  limit = -1;
  last_word_modif = 0;
}


//----------
// operator
//----------


R_Sparse_Bitset & R_Sparse_Bitset::operator= (R_Sparse_Bitset & set)
// assigns the current bitset with the bitset set
{
  if (this != &set)
  {
    c = set.c;
    card = set.card;
    words = set.words;
    index = set.index;
    limit = set.limit;
    mask.resize(set.mask.size());
    last_offset_modif.resize (last_offset_modif.size());
  }
  return *this;
}


//-----------------
// basic functions
//-----------------


void R_Sparse_Bitset::Add_Elements (unsigned int k)
// adds k elements to the bitset
{
  unsigned int new_size = ceil ((card + k) / (double) WORD_SIZE);

  if (new_size > index.size())
  {
    words.resize(new_size,0);
    index.resize(new_size);
    mask.resize(new_size);
    last_offset_modif.resize (new_size,0);
    limit = new_size - 1;
  }
  
  for (unsigned int i = 0; i < k; i++)
  {
    int offset = card / WORD_SIZE;
    if (card % WORD_SIZE == 0)
      index[offset] = offset;
    
    words[offset] = words[offset] | (1ul << (card % WORD_SIZE));
    card++;
  }
}


void R_Sparse_Bitset::Clear_Mask ()
// resets each bit of the mask to the value 0
{
  for (int i = 0; i <= limit; i++)
    mask[index[i]] = 0ul;
}


void R_Sparse_Bitset::Reverse_Mask ()
// applies the bitwise not to each bit of the mask
{
  for (int i = 0; i <= limit; i++)
    mask[index[i]] = ~mask[index[i]];
}


void R_Sparse_Bitset::Add_To_Mask (vector<unsigned long> & m)
// applies the bitwise or to each bit of the mask and m
{
  for (int i = 0; i <= limit; i++)
    mask[index[i]] = mask[index[i]] | m[index[i]];
}


void R_Sparse_Bitset::Intersect_With_Mask (Deletion_Stack * ds)
// applies the bitwise and to each bit of the mask and words
{
  for (int i = limit; i >= 0; i--)
  {
    int offset = index[i];
    unsigned long w = words[offset] & mask[offset];

    if (w != words[offset])
    {
      // backup
      unsigned long time = ds->Get_Timestamp();
      
      if (time > 0)
      {
        // we backup the word before its modification
        if (last_word_modif != time)
        {
          last_size_word_backup.push_back (word_backup.size());
          last_word_modif = time;
        }
        
        if (last_offset_modif[offset] != time)
        {
          word_backup.push_back (pair<int,unsigned long> (offset,words[offset]));
          last_offset_modif[offset] = time;
          
          ds->Add_Element (c);
        }
      }
      
      words[offset] = w;

      if (w == 0ul)
      {
        index[i] = index[limit];
        index[limit] = offset;
        limit--;
      }
    }
  }
}


int R_Sparse_Bitset::Intersect_Index (vector<unsigned long> & m)
// returns the index of a word where bitset intersects with m, -1 otherwise
{
  for (int i = 0; i <= limit; i++)
  {
    int offset = index[i];
    if ((words[offset] & m[offset]) != 0ul)
      return offset;
  }
  return -1;
}


void R_Sparse_Bitset::Restore ()
// restores the bitset in its previous state
{
  unsigned int last_size = last_size_word_backup.back();
  last_size_word_backup.pop_back();
  
  while (word_backup.size() > last_size)
  {
    pair<int,unsigned long> p = word_backup.back();
    word_backup.pop_back();
    if (words[p.first] == 0ul)
      limit++;
    words[p.first] = p.second;
  }
}


unsigned int R_Sparse_Bitset::One_Number ()
// returns the number of 1 in the bitset
{
  unsigned int nb = 0;
  for (int k = 0; k <= limit; k++)
  {
    for (unsigned int j = 0; j < WORD_SIZE; j++)
      if (((words[index[k]] >> j) & 1ul) == 1ul)
        nb++;
  }
  
  return nb;
}


unsigned int R_Sparse_Bitset::One_Number (vector<unsigned long> & m)
// returns the number of 1 in the intersection between the bitset and m
{
  unsigned int nb = 0;
  for (int k = 0; k <= limit; k++)
  {
    for (unsigned int j = 0; j < WORD_SIZE; j++)
      if ((((words[index[k]] & m[index[k]]) >> j) & 1ul) == 1)
        nb++;
  }
  
  return nb;
}
