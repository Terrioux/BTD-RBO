#include "all_different_list_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


All_Different_List_Global_Constraint::All_Different_List_Global_Constraint (vector<Variable *> & var, unsigned int size): Global_Constraint (var)
// constructs a all-diff global constraint 
{
  list_size = size;
  list_number = var.size() / list_size;
}
		
		
All_Different_List_Global_Constraint::All_Different_List_Global_Constraint (All_Different_List_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
  list_size = c.list_size;
}


All_Different_List_Global_Constraint::~All_Different_List_Global_Constraint ()
// destructor
{
}


//-----------------
// basic functions
//-----------------


bool All_Different_List_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < list_number-1; i++)
  {
    unsigned int ind_i = list_size*i;
    for (unsigned int j = i+1; j < list_number; j++)
    {
      unsigned int ind_j = list_size*j;
      
      // we compare the two lists
      unsigned int k = 0;
      while ((k < list_size) and (scope_var[ind_i+k]->Get_Domain()->Get_Real_Value(t[ind_i+k]) == scope_var[ind_j+k]->Get_Domain()->Get_Real_Value(t[ind_j+k])))
        k++;
      if (k == list_size)
        return false;
    }
  }
  return true;
}


bool All_Different_List_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
  unsigned int j = x / list_size;
  unsigned int k = x % list_size;
  
  // we check if the variable var is the single variable of its list having a singleton domain. If not, no modification can occur
  for (unsigned int i = 0; i < list_size; i++) 
    if ((i != k) && (scope_var[j*list_size+i]->Get_Domain()->Get_Size() > 1))
      return false;

  // we identify the lists which all variables have a singleton domain
	fixed_lists.clear();
  for (unsigned int i = 0; i < list_number; i++)
  {
    unsigned int j = 0;
    unsigned int nb_non_singleton = 0;  // the number of non singleton domain
    while ((j < list_size) && (nb_non_singleton <= 1))
    {
      if (scope_var[list_size*i+j]->Get_Domain()->Get_Size() > 1)
        nb_non_singleton++;
      j++;
    }
    
    if (nb_non_singleton == 0)
    {
      fixed_lists.insert(i);
    }
  }

  j = x / list_size;
	Domain * dx = scope_var [x]->Get_Domain();
	unsigned int dx_size = dx->Get_Size();	
  unsigned int ind_j = list_size*j;
  
	for (auto i : fixed_lists)
   if (i != j)
    {
      unsigned int ind_i = list_size*i;

      // we compare the two lists
      unsigned int k = 0;
      unsigned int nb_difference = 0;
      while ((k < list_size) and (nb_difference < 1))
      {
        if (scope_var[ind_j+k]->Get_Domain()->Get_Size() == 1)
        {
          if (scope_var[ind_i+k]->Get_Domain()->Get_Remaining_Real_Value(0) != scope_var[ind_j+k]->Get_Domain()->Get_Remaining_Real_Value(0))
            nb_difference++;
        }
          
        k++;
      }
          
      if (nb_difference == 0)
      {
        k = x % list_size;
        
        // we check whether a value can be filtered
        int val = scope_var[ind_i+k]->Get_Domain()->Get_Remaining_Real_Value(0);    // the value to remove (if present)
        int v = dx->Get_Real_Value(val);

        if ((v != -1) && (dx->Is_Present(v)))
        {
          ds->Add_Element (scope_var[x]);
          dx->Filter_Value (v);
        }
        
        if (dx->Get_Size () == 0)
        {
          pb->Set_Conflict (scope_var[x],this);
          return true;
        }
      }
    }

  return dx_size != dx->Get_Size();
}


void All_Different_List_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  // we identify the lists which all variables or all variables except one have a singleton domain
	fixed_lists.clear();
	almost_fixed_lists.clear();
  for (unsigned int i = 0; i < list_number; i++)
  {
    unsigned int j = 0;
    unsigned int nb_non_singleton = 0;  // the number of non singleton domain
    while ((j < list_size) && (nb_non_singleton <= 1))
    {
      if (scope_var[list_size*i+j]->Get_Domain()->Get_Size() > 1)
        nb_non_singleton++;
      j++;
    }
    
    if (nb_non_singleton == 0)
    {
      fixed_lists.insert(i);
      almost_fixed_lists.insert(i);
    }
    else
      if (nb_non_singleton == 1)
        almost_fixed_lists.insert(i);
  }

	for (auto i : fixed_lists)
	{
    unsigned int ind_i = list_size*i;
    for (auto j : almost_fixed_lists)
      if (i != j)
      {
        unsigned int ind_j = list_size*j;
        
        // we compare the two lists
        unsigned int k = 0;
        unsigned int nb_difference = 0;
        while ((k < list_size) and (nb_difference < 1))
        {
          if (scope_var[ind_j+k]->Get_Domain()->Get_Size() == 1)
          {
            if (scope_var[ind_i+k]->Get_Domain()->Get_Remaining_Real_Value(0) != scope_var[ind_j+k]->Get_Domain()->Get_Remaining_Real_Value(0))
              nb_difference++;
          }
            
          k++;
        }
        
        if (nb_difference == 0)
        {
          k = 0;
          while (k < list_size and A.Is_Assigned(scope_var[ind_j+k]->Get_Num()))
            k++;
          
          unsigned int x;
          unsigned int y;
          
          if (k == list_size)
          {
            k = 0;
            while (k < list_size and A.Is_Assigned(scope_var[ind_i+k]->Get_Num()))
              k++;

            x = ind_i;
            y = ind_j;
          }
          else
            {
              x = ind_j;
              y = ind_i;
            }
          
          // we check whether a value can be filtered
          int val = scope_var[y+k]->Get_Domain()->Get_Remaining_Real_Value(0);    // the value to remove (if present)
          Domain * d = scope_var[x+k]->Get_Domain();
          int v = d->Get_Real_Value(val);

          if ((v != -1) && (d->Is_Present(v)))
          {
            ds->Add_Element (scope_var[x+k]);
            d->Filter_Value (v);
          }
          
          if (d->Get_Size () == 0)
          {
            pb->Set_Conflict (scope_var[x+k],this);
            return;
          }
        }
      }
  }
}


string All_Different_List_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<allDifferent>\n";
  for (unsigned int i = 0; i < list_number; i++)
  {
    expr += "  <list>";
    for (unsigned int j = 0; j < list_size; j++)
    {
      expr += scope_var[i*list_size+j]->Get_Name();
      if (j < list_size - 1)
        expr += " ";
    }
    expr += "</list>\n";
  } 
  expr += "</allDifferent>";
  return expr;
}
