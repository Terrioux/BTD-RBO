#include "short_positive_compact_table_extension_constraint.h"
#include <cmath>
#include <climits>


// ---------------------------
// constructor and destructor
// ---------------------------


Short_Positive_Compact_Table_Extension_Constraint::Short_Positive_Compact_Table_Extension_Constraint (vector<Variable *> & var): Positive_Compact_Table_Extension_Constraint (var)
// constructs an extension constraint based on compact table whose scope is defined by the variables in var
{
  supports_star.resize(arity);
  tightness = 1;
  allowed_tuple_number = 0;
  for (unsigned int i = 0; i < arity; i++)
  {
    unsigned int size = scope_var[i]->Get_Domain()->Get_Initial_Size();
    supports_star[i].resize(size);
    tightness *= size;
  }
}


Short_Positive_Compact_Table_Extension_Constraint::Short_Positive_Compact_Table_Extension_Constraint (Short_Positive_Compact_Table_Extension_Constraint & c): Positive_Compact_Table_Extension_Constraint (c)
// constructs a new constraint by copying the constraint c 
{
  supports_star = c.supports_star;
}


//-----------------
// basic functions
//-----------------


Constraint * Short_Positive_Compact_Table_Extension_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
  return new Short_Positive_Compact_Table_Extension_Constraint(*this);
}


void Short_Positive_Compact_Table_Extension_Constraint::Allow_Tuple (int * t)
// allows the tuple t
{
  int new_size = ceil((tuple_number+1) / (double) WORD_SIZE);
  
  if (new_size > ceil(tuple_number / (double) WORD_SIZE))
  {
    for (unsigned int i = 0; i < arity; i++)
    {
      unsigned int size = scope_var[i]->Get_Domain()->Get_Initial_Size();
      for (unsigned int j = 0; j < size; j++)
      {
        data[i][j].resize(new_size,0);
        supports_star[i][j].resize(new_size,0);
      }
    }
  }

  unsigned long mask = 1ul << (tuple_number % WORD_SIZE);
  unsigned long nb = 1;
  for (unsigned int j = 0; j < arity; j++)
  {
    if (t[j] == INT_MAX)
    {
      nb *= scope_var[j]->Get_Domain()->Get_Initial_Size();
      for (int i = scope_var[j]->Get_Domain()->Get_Initial_Size()-1; i >= 0; i--)
      {
        data[j][i][tuple_number / WORD_SIZE] = data[j][i][tuple_number / WORD_SIZE] | mask;

        if (residues[j][i] == -1)
          residues[j][i] = tuple_number / WORD_SIZE;
      }
    }
    else
      {
        data[j][t[j]][tuple_number / WORD_SIZE] = data[j][t[j]][tuple_number / WORD_SIZE] | mask;
        supports_star[j][t[j]][tuple_number / WORD_SIZE] = supports_star[j][t[j]][tuple_number / WORD_SIZE] | mask;

        if (residues[j][t[j]] == -1)
          residues[j][t[j]] = tuple_number / WORD_SIZE;
      }   
  }
  
  allowed_tuple_number += nb;
  tightness -= nb;
  
  tuple_number++;
  current_table.Add_Elements (1);
}


void Short_Positive_Compact_Table_Extension_Constraint::Forbid_Tuple (int * t)
// forbids the tuple t
{
  unsigned int word_num = 0;
  
  for (vector<unsigned long>::iterator iter = data[0][t[0]].begin(); iter != data[0][t[0]].end(); iter++)
  {
    if (*iter != 0)
    {
      for (unsigned int i = 0; i < WORD_SIZE; i++)
      {
        if ((((*iter) >> i) & 1ul) == 1)
        {
          unsigned int j = 1;
          while ((j < arity) && (((data[j][t[j]][word_num] >> i) & 1ul) == 1))
            j++;
          
          if (j == arity)
          {
            unsigned long mask = ~(1 << i);
            for (int j = arity-1; j >= 0; j--)
            {
              data[j][t[j]][word_num] = data[j][t[j]][word_num] & mask;
              supports_star[j][t[j]][word_num] = supports_star[j][t[j]][word_num] & mask;
            }
            
            tuple_number--;
            allowed_tuple_number--;
            tightness++;
            
            return;
          }
        }
      }
    }
    word_num++;
  }  
}


void Short_Positive_Compact_Table_Extension_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  if (ref == 0)
  {
    event_manager = pb->Get_Event_Manager();

    // we remove the values which initially have no support
    for (unsigned int i = 0; i < arity; i++)
    {
      Domain * dx = scope_var[i]->Get_Domain();
      for (int j = dx->Get_Size()-1; j >= 0; j--)
        if (residues[i][dx->Get_Remaining_Value(j)] == -1)
        {
          ds->Add_Element (scope_var[i]);
          dx->Filter_Value (dx->Get_Remaining_Value(j));
        }
    }
  }
  
  S_val.clear();
  S_sup.clear();
  
  for (unsigned int i = 0; i < arity; i++)
  {
    if ((event_manager->Exist_Event_Dmc(scope_var[i]->Get_Num(),ref))|| (event_manager->Exist_Event_Fix(scope_var[i]->Get_Num(),ref)))
      S_val.insert(i);

    if (scope_var[i]->Get_Domain()->Get_Size() > 1)
      S_sup.insert(i);
  }
    
  if (Update_Table(pb,ds,ref))
    return;
  
  Filter_Domain(ds);
}


bool Short_Positive_Compact_Table_Extension_Constraint::Update_Table (CSP * pb, Deletion_Stack * ds, timestamp ref)
// updates the table and returns true iff the table is empty
{
  for (set<unsigned int>::iterator iter_pos = S_val.begin(); iter_pos != S_val.end(); iter_pos++)
  {
    Domain * dx = scope_var[*iter_pos]->Get_Domain();
    
    current_table.Clear_Mask();
   
    unsigned int delta_x;
    int num_var = scope_var[*iter_pos]->Get_Num();
    unsigned int nb_removed_values = dx->Get_Initial_Size() - dx->Get_Size();
    
    for (delta_x = 0; (delta_x < nb_removed_values) && (event_manager->Exist_Event_Dmc (num_var, dx->Get_Deleted_Value(delta_x), ref)); delta_x++);

    if (delta_x < dx->Get_Size())
    {
      for (unsigned int j = 0; j < delta_x; j++)
        current_table.Add_To_Mask (supports_star[*iter_pos][dx->Get_Deleted_Value(j)]);

      current_table.Reverse_Mask();
    }
    else
      {
        for (int i = dx->Get_Size()-1 ; i >= 0; i--)
          current_table.Add_To_Mask (data[*iter_pos][dx->Get_Remaining_Value(i)]);
      }
      
    current_table.Intersect_With_Mask(ds);
    if (current_table.Is_Empty())
    {
      pb->Set_Conflict (scope_var[*iter_pos],this);
      return true;
    }
  }
  
  return false;
}

