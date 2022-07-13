//-------------
// constructor
//-------------


template<class T>
inline Weighted_R_Sparse_Set<T>::Weighted_R_Sparse_Set ()
// constructor
{
  card = 0;
  last_modif = 0;
}


//-----------------
// basic functions
//-----------------


template<class T>
inline bool Weighted_R_Sparse_Set<T>::Is_Empty ()
// returns true if the set is empty, false otherwise
{
  return card == 0;
}


template<class T>
inline void Weighted_R_Sparse_Set<T>::Add (T val)
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
      weight.resize(card+20);
    }
    values[card] = val;
    pos[card] = card;
    weight[card] = 1;
    card++;
  }
  else weight[pos[i]]++;
}


template<class T>
inline bool Weighted_R_Sparse_Set<T>::Remove (T val, Deletion_Stack * ds)
// removes the value val from the set and returns true if the deletion is performed
{
  unsigned long i = 0;
  while ((i < card) && (values[pos[i]] != val))
    i++;
  
  if (i < card)
  {
    unsigned long timestamp = ds->Get_Timestamp();
    
    if (timestamp > 0)
    {
      if (last_modif != timestamp)
      {
        last_modif = timestamp;
        size_backup.push (new vector<unsigned int>);
      }
      
      weight[pos[i]]--;
      
      if (weight[pos[i]] == 0)
      {
        unsigned int aux;
        card--;
        aux = pos[i];
        pos[i] = pos[card];
        pos[card] = aux;
        size_backup.top()->push_back (pos[card]);
      }
      else size_backup.top()->push_back (pos[i]);
    }
    else
      {
        weight[pos[i]]--;
        
        if (weight[pos[i]] == 0)
        {
          card--;
          pos[i] = pos[card];
        }
      }
    
    return true;
  }
  else return false; //throw ("Error: Invalid access to sparse set");
}


template<class T>
inline T Weighted_R_Sparse_Set<T>::Get_Value (unsigned int i)
// returns the ith value of the set
{
  if (i < card)
    return values[pos[i]];
  else throw ("Error: Invalid access to sparse set");
}


template<class T>
inline void Weighted_R_Sparse_Set<T>::Restore ()
// restores the bitset in its previous state
{
  if (! size_backup.empty())
  {
    for (vector<unsigned int>::reverse_iterator iter = size_backup.top()->rbegin(); iter != size_backup.top()->rend(); iter++)
    {
      if (weight[*iter] == 0)
        card++;
      weight[*iter]++;
    }
    delete size_backup.top();
    
    size_backup.pop();
  }
}


template<class T>
inline unsigned int Weighted_R_Sparse_Set<T>::Get_Size ()
// returns the current size of the set
{
  return card;
}


template<class T>
inline void Weighted_R_Sparse_Set<T>::Clear (Deletion_Stack * ds)
// removes all the values from the set
{
  if (card > 0)
  {
    unsigned long timestamp = ds->Get_Timestamp();
    if (timestamp > 0)
    {
      if (last_modif != timestamp)
      {
        last_modif = timestamp;
        size_backup.push (new vector<unsigned int>);
      }

      for (unsigned int i = 0; i < card; i++)
      {
        for (unsigned int j = 0; j < weight[pos[i]]; j++)
          size_backup.top()->push_back (pos[i]);
        weight[pos[i]] = 0;
      }
    }
    card = 0;
  }
}
