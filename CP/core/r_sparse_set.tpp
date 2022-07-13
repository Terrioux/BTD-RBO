//-------------
// constructor
//-------------


template<class T>
inline R_Sparse_Set<T>::R_Sparse_Set ()
// constructor
{
  card = 0;
  last_modif = 0;
}


//-----------------
// basic functions
//-----------------


template<class T>
inline bool R_Sparse_Set<T>::Is_Empty ()
// returns true if the set is empty, false otherwise
{
  return card == 0;
}


template<class T>
inline void R_Sparse_Set<T>::Add (T val)
// adds the value val to the set
{
  unsigned long i = 0;
  while ((i < card) && (values[pos[i]] != val))
    i++;
  
  if (i == card)
  {
    if (card >= pos.size())
    {
      values.resize(card+20);
      pos.resize(card+20);
    }
    values[card] = val;
    pos[card] = card;
    card++;
  }
}


template<class T>
inline bool R_Sparse_Set<T>::Remove (T val, Deletion_Stack * ds)
// removes the value val from the set and returns true if the deletion is performed
{
  unsigned long i = 0;
  while ((i < card) && (values[pos[i]] != val))
    i++;
  
  if (i < card)
  {
    unsigned long timestamp = ds->Get_Timestamp();
    if (last_modif != timestamp)
    {
      last_modif = timestamp;
      size_backup.push_back (card);
    }
    
    unsigned int aux;
    card--;
    aux = pos[i];
    pos[i] = pos[card];
    pos[card] = aux;
    
    return true;
  }
  else return false; //throw ("Error: Invalid access to sparse set");
}


template<class T>
inline T R_Sparse_Set<T>::Get_Value (unsigned int i)
// returns the ith value of the set
{
  if (i < card)
    return values[pos[i]];
  else throw ("Error: Invalid access to sparse set");
}


template<class T>
inline void R_Sparse_Set<T>::Restore ()
// restores the bitset in its previous state
{
  card = size_backup.back();
  size_backup.pop_back();
}


template<class T>
inline unsigned int R_Sparse_Set<T>::Get_Size ()
// returns the current size of the set
{
  return card;
}


template<class T>
inline void R_Sparse_Set<T>::Clear (Deletion_Stack * ds)
// removes all the values from the set
{
  unsigned long timestamp = ds->Get_Timestamp();
  if (last_modif != timestamp)
  {
    last_modif = timestamp;
    size_backup.push_back (card);
  }

  card = 0;
}
