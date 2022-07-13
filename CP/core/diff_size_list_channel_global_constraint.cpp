#include "diff_size_list_channel_global_constraint.h"


//--------------
// constructors 
//--------------


Diff_Size_List_Channel_Global_Constraint::Diff_Size_List_Channel_Global_Constraint (vector<Variable *> & var, vector<Variable *> & list_1, int start1, vector<Variable *> & list_2, int start2): List_Channel_Global_Constraint (var,list_1,start1,list_2,start2)
// constructs a new channel constraint which involves the variable in var and whose start index is start
{
  unsigned int size[2];
  size[0] = list_1.size();
  size[1] = list_2.size();
  
  
  for (vector<Variable *>::iterator iter = list[0].begin(); iter != list[0].end(); iter++)
  {
    Domain * d = (*iter)->Get_Domain();
    for (unsigned j = 0; j < d->Get_Initial_Size(); j++)
      if (d->Is_Present (j))
      {
        int val = d->Get_Real_Value (j) - start_index[1];
        
        if ((val < 0) || (val >= (signed) size[1]))
          d->Filter_Value(j);
      }
  }
}


Diff_Size_List_Channel_Global_Constraint::Diff_Size_List_Channel_Global_Constraint (Diff_Size_List_Channel_Global_Constraint & c): List_Channel_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


//-----------------
// basic functions
//-----------------


bool Diff_Size_List_Channel_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < arity; i++)
    if (belong_to_list[0][i] >= 0)
    {
      int j = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]) - start_index[1];
      if (list[1][j]->Get_Domain()->Get_Real_Value(t[Get_Position(list[1][j]->Get_Num())]) != start_index[0]+(signed) belong_to_list[0][i])
        return false;
    }

	return true;
}


void Diff_Size_List_Channel_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
 
  bool again;
  
  do
  {
    again = false;    
    
    // we first process the variables of the first list
   
    for (int i = list[0].size()-1; i >= 0; i--)
    {
      if (event_manager->Exist_Event_Fix(list[0][i]->Get_Num(),ref))
      {
        int j = list[0][i]->Get_Domain()->Get_Remaining_Real_Value(0) - start_index[1];

        Domain * d = list[1][j]->Get_Domain();
        int v = d->Get_Value (i + start_index[0]);
        
        ds->Add_Element (list[1][j]);

        if (v == -1)
          d->WipeOut();
        else d->Assign(v);
        
        if (d->Get_Size() == 0)
        {
          pb->Set_Conflict (list[1][j],this);
          return;
        }
        
        j = list[0][i]->Get_Domain()->Get_Remaining_Real_Value(0);
        for (int k = list[0].size()-1; k >= 0; k--)
          if (i != k)
          {
            Domain * d = list[0][k]->Get_Domain();
            int v = d->Get_Value (j);
            
            if ((v != -1) && (d->Is_Present (v)))
            {
              ds->Add_Element(list[0][k]);
              d->Filter_Value(v);
            
              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (list[0][k],this);
                return;
              }
              else
                if ((k > i) && (d->Get_Size() == 1))
                  again = true;
            }  
          }
      }
    }
    
    // we then process the variables of the second list
    for (int i = list[1].size()-1; i >= 0; i--)
    {
      if (event_manager->Exist_Event_Fix(list[1][i]->Get_Num(),ref))
      {
        int j = list[1][i]->Get_Domain()->Get_Remaining_Real_Value(0) - start_index[0];
        
        for (int k = list[0].size()-1; k >= 0; k--)
          if (k != j)
          {
            Domain * d = list[0][k]->Get_Domain();
            int v = d->Get_Value (i + start_index[1]);
            
            if ((v != -1) && (d->Is_Present (v)))
            {
              ds->Add_Element(list[0][k]);
              d->Filter_Value(v);
            
              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (list[0][j],this);
                return;
              }
              else
                if (d->Get_Size() == 1)
                  again = true;
            }
          }
      }
      
      if ((event_manager->Exist_Event_Fix(list[1][i]->Get_Num(),ref)) || (event_manager->Exist_Event_Dmc(list[1][i]->Get_Num(),ref)))
      {
        unsigned int delta_x;
        Domain * dx = list[1][i]->Get_Domain();
        int num_var = list[1][i]->Get_Num();
        unsigned int nb_removed_values = dx->Get_Initial_Size() - dx->Get_Size();

        delta_x = 0;

        while ((delta_x < nb_removed_values) && (event_manager->Exist_Event_Dmc (num_var, dx->Get_Deleted_Value(delta_x), ref)))
          delta_x++;
        
        for (unsigned int pos = 0; pos < delta_x; pos++)
        {
          int j = dx->Get_Deleted_Real_Value(pos) - start_index[0];
          
          if ((j >= 0) && (j < (signed) list[0].size()))
          {
            Domain * d = list[0][j]->Get_Domain();
            int v = d->Get_Value (i + start_index[1]);
            
            if ((v != -1) && (d->Is_Present (v)))
            {
              ds->Add_Element (list[0][j]);
              d->Filter_Value (v);

              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (list[0][j],this);
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
    
    ref = event_manager->Get_Current_Time() - 1;
  }
  while (again);
}
