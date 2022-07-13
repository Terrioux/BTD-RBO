#include "equal_size_list_channel_global_constraint.h"


//--------------
// constructors 
//--------------


Equal_Size_List_Channel_Global_Constraint::Equal_Size_List_Channel_Global_Constraint (vector<Variable *> & var, vector<Variable *> & list_1, int start1, vector<Variable *> & list_2, int start2): List_Channel_Global_Constraint (var,list_1,start1,list_2,start2)
// constructs a new channel constraint which involves the variable in var and whose start index is start
{
  unsigned int size[2];
  size[0] = list_1.size();
  size[1] = list_2.size();
  
  for (unsigned int i = 0; i < arity; i++)
  {
    for (int k = 0; k < 2; k++)
      if (belong_to_list[k][i] >= 0)
      { 
        Domain * d = scope_var[i]->Get_Domain();
        for (unsigned j = 0; j < d->Get_Initial_Size(); j++)
          if (d->Is_Present (j))
          {
            int val = d->Get_Real_Value (j) - start_index[1-k];
            
            if ((val < 0) || (val >= (signed) size[1-k]))
              d->Filter_Value(j);
          }
      }
  }
}


Equal_Size_List_Channel_Global_Constraint::Equal_Size_List_Channel_Global_Constraint (Equal_Size_List_Channel_Global_Constraint & c): List_Channel_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


//-----------------
// basic functions
//-----------------


bool Equal_Size_List_Channel_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < arity; i++)
  {
    for (int k = 0; k < 2; k++)
      if (belong_to_list[k][i] >= 0)
      {
        int j = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]) - start_index[1-k];
        if (list[1-k][j]->Get_Domain()->Get_Real_Value(t[Get_Position(list[1-k][j]->Get_Num())]) != start_index[k]+(signed) belong_to_list[k][i])
          return false;
      }
  }
  
	return true;
}


void Equal_Size_List_Channel_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
 
  bool again;
  
  do
  {
    again = false;    

    for (unsigned int i = 0; i < arity; i++)
    {
      if (event_manager->Exist_Event_Fix(scope_var[i]->Get_Num(),ref))
      {
        for (int k = 0; k < 2; k++)
          if (belong_to_list[k][i] >= 0)
          {
            int j = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0) - start_index[1-k];
            
            Domain * d = list[1-k][j]->Get_Domain();
            int v = d->Get_Value (belong_to_list[k][i] + start_index[k]);
            
            ds->Add_Element (list[1-k][j]);

            int size = d->Get_Size();

            if (v == -1)
              d->WipeOut();
            else d->Assign(v);
            
            if (d->Get_Size() == 0)
            {
              pb->Set_Conflict (list[1-k][j],this);
              return;
            }
            else
              if ((k == 1) && (size > 1))
                again = true;
            
            // we remove the value j to each variable of the same list
            j = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0);
            
            for (int l = list[k].size()-1; l >= 0; l--)
              if (l != belong_to_list[k][i])
              {
                Domain * d = list[k][l]->Get_Domain();
                int v = d->Get_Value (j);
                
                if ((v != -1) && (d->Is_Present (v)))
                {
                  ds->Add_Element(list[k][l]);
                  d->Filter_Value(v);
                
                  if (d->Get_Size() == 0)
                  {
                    pb->Set_Conflict (list[k][l],this);
                    return;
                  }
                  else
                    if (d->Get_Size() == 1)
                      again = true;
                }  
              }
          }
      }
      else
        if (event_manager->Exist_Event_Dmc(scope_var[i]->Get_Num(),ref))
        {
          unsigned int delta_x;
          Domain * dx = scope_var[i]->Get_Domain();
          int num_var = scope_var[i]->Get_Num();
          unsigned int nb_removed_values = dx->Get_Initial_Size() - dx->Get_Size();

          for (delta_x = 0; (delta_x < nb_removed_values) && (event_manager->Exist_Event_Dmc (num_var, dx->Get_Deleted_Value(delta_x), ref)); delta_x++);
          
          for (int k = 0; k < 2; k++)
            if (belong_to_list[k][i] >= 0)
            {
              for (unsigned int pos = 0; pos < delta_x; pos++)
              {
                int j = dx->Get_Deleted_Real_Value(pos) - start_index[1-k];
                
                if ((j >= 0) && (j < (signed) list[1-k].size()))
                {
                  Domain * d = list[1-k][j]->Get_Domain();
                  int v = d->Get_Value (belong_to_list[k][i] + start_index[k]);
                  
                  if ((v != -1) && (d->Is_Present (v)))
                  {
                    ds->Add_Element (list[1-k][j]);
                    d->Filter_Value (v);

                    if (d->Get_Size() == 0)
                    {
                      pb->Set_Conflict (list[1-k][j],this);
                      return;
                    }
                    else
                      if (d->Get_Size() == 1)
                        again = true;
                  }
                }
              }
            }
        }
    }

    ref = event_manager->Get_Current_Time() - 1;
  }
  while (again);
}
