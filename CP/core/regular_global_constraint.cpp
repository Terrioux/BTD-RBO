#include "regular_global_constraint.h"
#include "no_deletion_stack.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Regular_Global_Constraint::Regular_Global_Constraint (vector<Variable *> & var, Automaton * def_A): Global_Constraint (var)
// constructs a new regular constraint which involves the variable in var and which is based on the automaton def_A
{
  automaton = def_A;

  // initialization of data structure
  Q = new R_Sparse_Set<state> * [arity];
  out_arcs = new Weighted_R_Sparse_Set<state> * [arity];
  in_arcs = new Weighted_R_Sparse_Set<state> * [arity+1];

  set<state> * N;   // the states of each layer
  N = new set<state> [arity+1];
  unsigned int nb_states = automaton->Get_State_Number();
  
  for (unsigned int i = 0; i < arity; i++)
  {
    Q[i] = new R_Sparse_Set<state> [scope_var[i]->Get_Domain()->Get_Initial_Size()];
    out_arcs[i] = new Weighted_R_Sparse_Set<state> [nb_states+1];
    in_arcs[i+1] = new Weighted_R_Sparse_Set<state> [nb_states+1];
  }
  
  // forward phase
  N[0].insert (automaton->Get_Initial_State());
  for (unsigned int i = 0; i < arity; i++)
  {
    Domain * d = scope_var[i]->Get_Domain();
    for (int a = d->Get_Size()-1; a >= 0; a--)
    {
      unsigned int j = d->Get_Remaining_Value(a);
      unsigned int v_j = d->Get_Remaining_Real_Value(a);
      for (set<state>::iterator iter_state = N[i].begin(); iter_state != N[i].end(); iter_state++)
      {
        state q_k = automaton->Delta(*iter_state,v_j);

        if (q_k != 0)
        {
          Q[i][j].Add(*iter_state);
          N[i+1].insert(q_k);
        }
      }
    }
  }
  
  
  // backward phase
  
  // we keep only the final state
  for (set<state>::iterator iter_state = N[arity].begin(); iter_state != N[arity].end(); )
  {
    if (automaton->Is_Final_State(*iter_state))
      iter_state++;
    else
      {
        set<state>::iterator iter_state2 = iter_state;
        iter_state++;
        N[arity].erase (iter_state2);
      }
  }

  
  int mark [nb_states+1];
  
  for (unsigned int i = 0; i <= nb_states; i++)
    mark[i] = -1;
  
  No_Deletion_Stack nds;
  
  for (int i = arity-1; i >= 0; i--)
  {
    Domain * d = scope_var[i]->Get_Domain();
    for (int a = d->Get_Size()-1; a >= 0; a--)
    {
      unsigned int j = d->Get_Remaining_Value(a);
      unsigned int v_j = d->Get_Remaining_Real_Value(a);
      
      for (unsigned int b = 0; b < Q[i][j].Get_Size(); )
      {
        state q_k = Q[i][j].Get_Value(b);
        state dq_k = automaton->Delta(q_k,v_j);
        
        if (N[i+1].find(dq_k) != N[i+1].end())
        {
          out_arcs[i][q_k].Add(dq_k);
          in_arcs[i+1][dq_k].Add(q_k);
          mark[q_k] = i;
          b++;
        }
        else Q[i][j].Remove (q_k,&nds);
      }
    }

    for (set<state>::iterator iter_state = N[i].begin(); iter_state != N[i].end(); )
    {
      if (mark[*iter_state] != i)
      {
        set<state>::iterator iter_state2 = iter_state;
        iter_state++;
        N[i].erase(iter_state2);
      }
      else iter_state++;
    }
  }

  delete [] N;

  
  // we clean up the domains
  for (unsigned int i = 0; i < arity; i++)
  {
    Domain * d = scope_var[i]->Get_Domain();
    for (int a = d->Get_Size()-1; a >= 0; a--)
    {
      unsigned int j = d->Get_Remaining_Value(a);
      if (Q[i][j].Get_Size() == 0)
      {
        d->Filter_Value (j);
        if (d->Get_Size() == 0)
          return;
      }
    }
  }
}
		
		
Regular_Global_Constraint::Regular_Global_Constraint (Regular_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c
{
  automaton = new Automaton;
  
  *automaton = *(c.automaton);

  // initialization of data structure
  Q = new R_Sparse_Set<state> * [arity];
  out_arcs = new Weighted_R_Sparse_Set<state> * [arity];
  in_arcs = new Weighted_R_Sparse_Set<state> * [arity+1];
  unsigned int nb_states = automaton->Get_State_Number();
  
  for (unsigned int i = 0; i < arity; i++)
  {
    Q[i] = new R_Sparse_Set<state> [scope_var[i]->Get_Domain()->Get_Initial_Size()];
    out_arcs[i] = new Weighted_R_Sparse_Set<state> [nb_states+1];
    in_arcs[i+1] = new Weighted_R_Sparse_Set<state> [nb_states+1];
  }
}


Regular_Global_Constraint::~Regular_Global_Constraint ()
// destructor
{
  delete automaton;

  for (unsigned int i = 0; i < arity; i++)
  {
    delete [] Q[i];
    delete [] out_arcs[i];
    delete [] in_arcs[i+1];
  }

  delete [] Q;
  delete [] out_arcs;
  delete [] in_arcs;
}


//-----------------
// basic functions
//-----------------


bool Regular_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  state s = automaton->Get_Initial_State();

  for (unsigned int i = 0; (i < arity) && (s != 0); i++)
    s = automaton->Delta (s,scope_var[i]->Get_Domain()->Get_Real_Value(t[i]));

  return (s != 0) && (automaton->Is_Final_State(s));
}


void Regular_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
    
  for (unsigned int i = 0; i < arity; i++)
    if ((event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(),ref)) || (event_manager->Exist_Event_Dmc (scope_var[i]->Get_Num(),ref)))
    {
      Domain * d = scope_var[i]->Get_Domain();
      unsigned int delta_x;
      int num_var = scope_var[i]->Get_Num();
      
      unsigned int nb_removed_values = d->Get_Initial_Size() - d->Get_Size();
          
      for (delta_x = 0; (delta_x < nb_removed_values) && (event_manager->Exist_Event_Dmc (num_var, d->Get_Deleted_Value(delta_x), ref)); delta_x++);

      for (unsigned int a = 0; a < delta_x; a++)
      {
        // we update the data structure for each deleted value
        unsigned int j = d->Get_Deleted_Value(a);
        int v_j = d->Get_Deleted_Real_Value(a);

        for (unsigned int b = 0; b < Q[i][j].Get_Size(); b++)
        {
          state q_k = Q[i][j].Get_Value(b);
          state dq_k = automaton->Delta(q_k,v_j);

          if (out_arcs[i][q_k].Remove (dq_k,ds))
          {
            Update_Backup_Out_Arcs_Data (i,q_k,ds);
            Update_Out_Arcs (pb,i,q_k,ds);
          }
          
          if ((pb->Get_Last_Conflict_Constraint() == 0) && (in_arcs[i+1][dq_k].Remove (q_k,ds)))
          {
            Update_Backup_In_Arcs_Data (i+1,dq_k,ds);
            Update_In_Arcs (pb,i+1,dq_k,ds);
          }
          
          if (pb->Get_Last_Conflict_Constraint() != 0)
            return;
        }
        Q[i][j].Clear(ds);
        Update_Backup_Q_Data (i,j,ds);
      }
    }
}


void Regular_Global_Constraint::Update_Out_Arcs (CSP * pb, unsigned int i, state q_k, Deletion_Stack * ds)
// updates the out_arcs data structure when an arc from q_k is deleted
{
  if ((i > 0) && (out_arcs[i][q_k].Get_Size() == 0))
  {
    for (unsigned int a = 0; a < in_arcs[i][q_k].Get_Size(); a++)
    {
      state q_l = in_arcs[i][q_k].Get_Value(a);
      Domain * d = scope_var[i-1]->Get_Domain();      
      
      for (unsigned int b = 0; b < d->Get_Size(); )
        if (automaton->Exists_Transition(q_l,d->Get_Remaining_Real_Value(b),q_k))
        {
          if (out_arcs[i-1][q_l].Remove(q_k,ds))
            Update_Backup_Out_Arcs_Data (i-1,q_l,ds);
          
          unsigned int j = d->Get_Remaining_Value(b);

          if (Q[i-1][j].Remove(q_l,ds))
          {
            Update_Backup_Q_Data (i-1,j,ds);
          
            if (Q[i-1][j].Get_Size() == 0)
            {
              ds->Add_Element (scope_var[i-1]);
              d->Filter_Value(j);
              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (scope_var[i-1],this);
                return;
              }
            }
            else b++;
          }
          else b++;
          
          Update_Out_Arcs (pb,i-1,q_l,ds);
          if (pb->Get_Last_Conflict_Constraint() != 0)
            return;
        }
        else b++;
    }
    in_arcs[i][q_k].Clear(ds);
    Update_Backup_In_Arcs_Data (i,q_k,ds);
  }
}


void Regular_Global_Constraint::Update_In_Arcs (CSP * pb, unsigned int i, state q_k, Deletion_Stack * ds)
// updates the in_arcs data structure when an arc to q_k is deleted
{
  if ((i < arity) && (in_arcs[i][q_k].Get_Size() == 0))
  {
    for (unsigned int a = 0; a < out_arcs[i][q_k].Get_Size(); a++)
    {
      state q_l = out_arcs[i][q_k].Get_Value(a);     
      Domain * d = scope_var[i]->Get_Domain();
      
      for (unsigned int b = 0; b < d->Get_Size(); )
      {
        if (automaton->Exists_Transition(q_k,d->Get_Remaining_Real_Value(b),q_l))
        {
          if (in_arcs[i+1][q_l].Remove(q_k,ds))
            Update_Backup_In_Arcs_Data (i+1,q_l,ds);

          unsigned int j = d->Get_Remaining_Value(b);

          if (Q[i][j].Remove(q_k,ds))
          {
            Update_Backup_Q_Data (i,j,ds);
          
            if (Q[i][j].Get_Size() == 0)
            {
              ds->Add_Element (scope_var[i]);
              d->Filter_Value(j);
              
              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (scope_var[i],this);
                return;
              }
            }
            else b++;
          }
          else b++;
        
          Update_In_Arcs (pb,i+1,q_l,ds);
          if (pb->Get_Last_Conflict_Constraint() != 0)
            return;
        }
        else b++;
      }
    }
    out_arcs[i][q_k].Clear(ds);
    Update_Backup_Out_Arcs_Data (i,q_k,ds);
  }
}


void Regular_Global_Constraint::Update_Backup_Out_Arcs_Data (unsigned int i, state q_l, Deletion_Stack * ds)
// memorizes the out arcs to be restored
{
  unsigned long timestamp = ds->Get_Timestamp();
  
  if ((backup_data.empty()) || (backup_data.top().first != timestamp))
  {
    set<pair<unsigned int,state>> s1;
    set<pair<unsigned int,state>> s2;
    set<pair<unsigned int,unsigned int>> s3;
    s1.insert(make_pair(i,q_l));
    backup_data.push(make_pair(timestamp,make_tuple(s1,s2,s3)));
  }
  else get<0>(backup_data.top().second).insert(make_pair(i,q_l));
  
  ds->Add_Element (this);
}


void Regular_Global_Constraint::Update_Backup_In_Arcs_Data (unsigned int i, state q_l, Deletion_Stack * ds)
// memorizes the in arcs to be restored
{
  unsigned long timestamp = ds->Get_Timestamp();
  
  if ((backup_data.empty()) || (backup_data.top().first != timestamp))
  {
    set<pair<unsigned int,state>> s1;
    set<pair<unsigned int,state>> s2;
    set<pair<unsigned int,unsigned int>> s3;
    s2.insert(make_pair(i,q_l));
    backup_data.push(make_pair(timestamp,make_tuple(s1,s2,s3)));
  }
  else get<1>(backup_data.top().second).insert(make_pair(i,q_l));
  
  ds->Add_Element (this);
}


void Regular_Global_Constraint::Update_Backup_Q_Data (unsigned int i, unsigned int j, Deletion_Stack * ds)
// memorizes the states from Q to be restored
{
  unsigned long timestamp = ds->Get_Timestamp();
  
  if ((backup_data.empty()) || (backup_data.top().first != timestamp))
  {
    set<pair<unsigned int,state>> s1;
    set<pair<unsigned int,state>> s2;
    set<pair<unsigned int,unsigned int>> s3;
    s3.insert(make_pair(i,j));
    backup_data.push(make_pair(timestamp,make_tuple(s1,s2,s3)));
  }
  else get<2>(backup_data.top().second).insert(make_pair(i,j));
  
  ds->Add_Element (this);
}


void Regular_Global_Constraint::Restore ()
// restores the constraint in its previous state
{
  for (set<pair<unsigned int,state>>::iterator iter = get<0>(backup_data.top().second).begin(); iter != get<0>(backup_data.top().second).end(); iter++)
    out_arcs[iter->first][iter->second].Restore();
    
  for (set<pair<unsigned int,state>>::iterator iter = get<1>(backup_data.top().second).begin(); iter != get<1>(backup_data.top().second).end(); iter++)
    in_arcs[iter->first][iter->second].Restore();
    
  for (set<pair<unsigned int,unsigned int>>::iterator iter = get<2>(backup_data.top().second).begin(); iter != get<2>(backup_data.top().second).end(); iter++)
    Q[iter->first][iter->second].Restore();
  
  backup_data.pop();
}


string Regular_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<regular> ";
  expr += "  <list>";
  for (unsigned int i = 0; i < arity; i++)
    expr +=   " " + scope_var[i]->Get_Name();
  expr += " </list>\n";
  
  expr += automaton->Get_XCSP3_Expression();
  
  expr += "</regular>";
  return expr;  
}
