#include "nogood_base_global_constraint.h"
#include <iostream>

//-----------------------------
// constructors and destructor
//-----------------------------

Nogood_Base_Global_Constraint::Nogood_Base_Global_Constraint (): Global_Constraint ()
// constructs a new constraint which initially involves no variable and is universal
{
  nary_nogood_base = 0;
  n = 0;
}


Nogood_Base_Global_Constraint::Nogood_Base_Global_Constraint (Nogood_Base_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  n = c.n;

  // we copy the nary nogood base
  if (c.nary_nogood_base == 0)
  {
    nary_nogood_base = 0;
  }
  else    
    {
      nary_nogood_base = new list <element *> * [n];
      for (unsigned int i = 0; i < n; i++)
        nary_nogood_base[i] = 0;
      
      for (unsigned int i = 0; i < arity; i++)
      {
        int num = scope_var[i]->Get_Num();
        Domain * d = scope_var[i]->Get_Domain();
        nary_nogood_base[num] = new list <element *> [d->Get_Initial_Size()];
        for (unsigned int j = 0; j < d->Get_Initial_Size(); j++)
        {
          for (list<element*>::iterator iter = c.nary_nogood_base[num][j].begin (); iter != c.nary_nogood_base[num][j].end (); iter++)
          {
            element * e = new element;
            e->size = (*iter)->size;
            e->nogood = new pair<unsigned int,int> [e->size];
            for (int k = 0; k < e->size; k++)
              e->nogood[k] = (*iter)->nogood[k];
            e->watch1 = (*iter)->watch1;
            e->watch2 = (*iter)->watch2;
            
            nary_nogood_base[num][j].push_back(e);
          }
        }
      }
    }
}


Nogood_Base_Global_Constraint::~Nogood_Base_Global_Constraint()
// destructor
{
  if (nary_nogood_base != 0)
  {
    for (vector<Variable *>::iterator iter_var = scope_var.begin(); iter_var != scope_var.end(); iter_var++)
    {
      int var = (*iter_var)->Get_Num();
      
      if (nary_nogood_base[var] != 0)
      {
        Domain * dx = (*iter_var)->Get_Domain();

        for (unsigned int val = 0; val < dx->Get_Initial_Size(); val++)
        {
          for (list<element*>::iterator iter = nary_nogood_base[var][val].begin (); iter != nary_nogood_base[var][val].end (); iter++)
            if ((*iter)->size == -1)
            {
              delete [] ((*iter)->nogood);
              delete *iter;
            }
            else (*iter)->size = -1;
        }
      }
    }
    
    for (unsigned int i = 0; i < n; i++)
      delete [] nary_nogood_base[i];
  }
  delete [] nary_nogood_base;
}


//------------------
// basics functions
//------------------


bool Nogood_Base_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	if (nary_nogood_base != 0)
  {
    for (unsigned int i = 0; i < arity; i++)
    {
      int num = scope_var[i]->Get_Num();
      for (list <element *>::iterator iter = nary_nogood_base[num][t[i]].begin(); iter != nary_nogood_base[num][t[i]].end(); iter++)
      {
        int j = 0;
        while ((j < (*iter)->size) && (t[position.find((*iter)->nogood[j].first)->second] == (*iter)->nogood[j].second))
          j++;
          
        if (j == (*iter)->size)   // t contains a nogood
          return false;
      }
    }
  }

  return true;
}


void Nogood_Base_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  if (nary_nogood_base != 0)
  {
    Event_Manager * event_manager = pb->Get_Event_Manager();
    
    bool again;   // true if a modification may trigger other modifications, false otherwise
    
    do
    {
      again = false;
      
      for (set<unsigned int>::iterator iter_num = scope.begin(); iter_num != scope.end(); iter_num++)
        if ((event_manager->Exist_Event_Fix (*iter_num,ref)) && (pb->Get_Variable(*iter_num)->Get_Domain()->Get_Size() == 1))
        {
          unsigned int var = *iter_num;
          Variable * x = pb->Get_Variable(*iter_num);
          Domain * dx = x->Get_Domain();

          int val = dx->Get_Remaining_Value(0);

          Domain * d;
          int i, position;
          
          list<element*>::iterator iter2;

          if (nary_nogood_base[var] != 0)
          {
            for (list<element*>::iterator iter = nary_nogood_base[var][val].begin (); iter != nary_nogood_base[var][val].end (); )
            {
              // we look for another watch 
              position = -1;
              i = 0;
              
              while ((i < (*iter)->size) && (position == -1))
              {
                if (((*iter)->watch1 != i) && ((*iter)->watch2 != i))
                {
                  d = pb->Get_Variable((*iter)->nogood[i].first)->Get_Domain();
                  if ((! d->Is_Present((*iter)->nogood[i].second)) || (d->Get_Size() > 1))
                    position = i;
                }
                i++;
              }
              
              if (position == -1)
              {
                if ((*iter)->nogood[(*iter)->watch1].first == var)
                  position = (*iter)->watch2;
                else position = (*iter)->watch1;

                d = pb->Get_Variable((*iter)->nogood[position].first)->Get_Domain();
                if (d->Is_Present((*iter)->nogood[position].second))
                {
                  ds->Add_Element (pb->Get_Variable((*iter)->nogood[position].first));
                  d->Filter_Value((*iter)->nogood[position].second);
                  
                  if (d->Get_Size() == 0)  
                  {
                    pb->Set_Conflict (pb->Get_Variable((*iter)->nogood[position].first),this);
                    return;
                  }
                  
                  if (*iter_num > (*iter)->nogood[position].first)
                    again = true;
                }
                iter++;
              }
              else
                {
                  element * elt = *iter;
                  
                  iter2 = iter;
                  iter++;

                  nary_nogood_base[var][val].erase (iter2);
                  
                  if (elt->nogood[elt->watch1].first == var)
                    elt->watch1 = position;
                  else elt->watch2 = position;
                  
                  nary_nogood_base[elt->nogood[position].first][elt->nogood[position].second].push_front (elt);
                }
            }
          }
      }
    }
    while (again);
  }
}


void Nogood_Base_Global_Constraint::Add_Nogood (CSP * pb, pair <int,int> * ng, int size)
// adds the nogood ng involving size variables into the nogood base and related to the instance pb
{
  if (size == 1)
    throw ("Error: the nogood base cannot take into account unary nogood");
  else
    {
      if (nary_nogood_base == 0)
      {
        n = pb->Get_N();
        nary_nogood_base = new list <element *> * [n];
        for (unsigned int i = 0; i < n; i++)
          nary_nogood_base[i] = 0;
      }
      
      element * e = new element;

      e->nogood = new pair <unsigned int,int> [size];
      
      for (int i = 0; i < size; i++)
      {
        if (nary_nogood_base[ng[i].first] == 0)
        {
          nary_nogood_base[ng[i].first] = new list <element *> [pb->Get_Variable(ng[i].first)->Get_Domain()->Get_Initial_Size()];
                    
          scope.insert (ng[i].first);
 
          if (arity < scope.size())   // we add a new variable to the scope
          {
            // we add a new variable to the scope
            scope_var.push_back (pb->Get_Variable (ng[i].first));
            position.insert (make_pair(ng[i].first,arity));
            arity++;
            
            // we update the constraint hypergraph
            pb->Get_Structure()->Add_Vertex_In_Edge(ng[i].first,num);
            unassigned_variable_number++;
          }
        }
        e->nogood[i] = ng[i];
      }

      e->size = size;
      e->watch1 = 0;
      e->watch2 = size-1;

      nary_nogood_base[ng[e->watch1].first][ng[e->watch1].second].push_front (e);
      nary_nogood_base[ng[e->watch2].first][ng[e->watch2].second].push_front (e);
    }
}


void Nogood_Base_Global_Constraint::Add_Nogood (CSP * pb, pair <int,int> * ng, int size, Assignment & A)
// adds the nogood ng involving size variables into the nogood base and selects the watch literals depending on the assignment A
{
  if (size == 1)
    throw ("Error: the nogood base cannot take into account unary nogood");
  else
    {
      if (nary_nogood_base == 0)
      {
        n = pb->Get_N();
        nary_nogood_base = new list <element *> * [n];
        for (unsigned int i = 0; i < n; i++)
          nary_nogood_base[i] = 0;
      }

      element * e = new element;

      e->nogood = new pair <unsigned int,int> [size];
      for (int i = 0; i < size; i++)
      {
        if (nary_nogood_base[ng[i].first] == 0)
        {
          nary_nogood_base[ng[i].first] = new list <element *> [pb->Get_Variable(ng[i].first)->Get_Domain()->Get_Initial_Size()];
          
          scope.insert (ng[i].first);
          if (arity < scope.size())   // we add a new variable to the scope
          {
            // we add a new variable to the scope
            scope_var.push_back (pb->Get_Variable (ng[i].first));
            position.insert (make_pair(ng[i].first,arity));
            arity++;
            
            // we update the constraint hypergraph
            pb->Get_Structure()->Add_Vertex_In_Edge(ng[i].first,num);
          }
        }
        e->nogood[i] = ng[i];
      }
      e->size = size;
      
      if (A.Get_Position(ng[0].first) < A.Get_Position(ng[1].first))
      {
        e->watch1 = 0;
        e->watch2 = 1;
      }
      else
        {
          e->watch1 = 1;
          e->watch2 = 0;
        }

      for (int i = 2; i < size; i++)
        if (A.Get_Position(ng[i].first) > A.Get_Position(ng[e->watch2].first))
        {
          e->watch1 = e->watch2;
          e->watch2 = i;
        }
        else
          if (A.Get_Position(ng[i].first) > A.Get_Position(ng[e->watch1].first))
            e->watch1 = i;
      
      nary_nogood_base[ng[e->watch1].first][ng[e->watch1].second].push_front (e);
      nary_nogood_base[ng[e->watch2].first][ng[e->watch2].second].push_front (e);
    }
}


void Nogood_Base_Global_Constraint::Reset (CSP * pb)
// resets the nogood base
{
  if (arity > 0)
  {
    if (nary_nogood_base != 0)
    {
      for (vector<Variable *>::iterator iter_var = scope_var.begin(); iter_var != scope_var.end(); iter_var++)
      {
        unsigned int var = (*iter_var)->Get_Num();
        
        if (nary_nogood_base[var] != 0)
        {
          Domain * dx = (*iter_var)->Get_Domain();

          for (unsigned int val = 0; val < dx->Get_Initial_Size(); val++)
          {
            for (list<element*>::iterator iter = nary_nogood_base[var][val].begin (); iter != nary_nogood_base[var][val].end (); iter++)
              if ((*iter)->size == -1)
              {
                delete [] ((*iter)->nogood);
                delete *iter;
              }
              else (*iter)->size = -1;

            nary_nogood_base[var][val].clear();
          }
        }
      }      
    }

    pb->Get_Structure()->Reset_Edge(num);
    
    scope.clear();
    scope_var.clear();
    position.clear();
    arity = 0;
    unassigned_variable_number = 0;
  }
}
