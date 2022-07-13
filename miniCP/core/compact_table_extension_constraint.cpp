#include "compact_table_extension_constraint.h"
#include <cmath>


// ---------------------------
// constructor and destructor
// ---------------------------


Compact_Table_Extension_Constraint::Compact_Table_Extension_Constraint (vector<Variable *> & var): Extension_Constraint (var), current_table(this)
// constructs an extension constraint based on compact table whose scope is defined by the variables in var
{
  data.resize(arity);
  possible_tuple_number = 1;
  tuple_number = 0;
  for (unsigned int i = 0; i < arity; i++)
  {
    unsigned int size = scope_var[i]->Get_Domain()->Get_Initial_Size();
    data[i].resize(size);
    possible_tuple_number *= size;
  }  
}


Compact_Table_Extension_Constraint::Compact_Table_Extension_Constraint (Compact_Table_Extension_Constraint & c): Extension_Constraint (c.scope_var), current_table(this)
// constructs a new constraint by copying the constraint c 
{
  possible_tuple_number = c.possible_tuple_number;
  tuple_number = c.tuple_number;
  current_table = c.current_table;
  data = c.data;
}


Compact_Table_Extension_Constraint::~Compact_Table_Extension_Constraint()
// destructor
{
}


//-----------------
// basic functions
//-----------------


void Compact_Table_Extension_Constraint::Add_Tuple (int * t)
// adds the tuple t to data
{
  int new_size = ceil((tuple_number+1) / (double) WORD_SIZE);
  
  if (new_size > ceil(tuple_number / (double) WORD_SIZE))
  {
    for (unsigned int i = 0; i < arity; i++)
    {
      unsigned int size = scope_var[i]->Get_Domain()->Get_Initial_Size();
      for (unsigned int j = 0; j < size; j++)
        data[i][j].resize(new_size,0);
    }
  }  
  
  unsigned long mask = 1ul << (tuple_number % WORD_SIZE);
  
  for (unsigned int j = 0; j < arity; j++)
    data[j][t[j]][tuple_number / WORD_SIZE] = data[j][t[j]][tuple_number / WORD_SIZE] | mask;
  
  tuple_number++;
  current_table.Add_Elements (1);
}


void Compact_Table_Extension_Constraint::Remove_Tuple (int * t)
// removes the tuple t from data
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
            }
            
            tuple_number--;

            return;
          }
        }
      }
    }
    word_num++;
  }  
}
