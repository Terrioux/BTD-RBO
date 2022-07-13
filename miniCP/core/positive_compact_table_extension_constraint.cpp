#include "positive_compact_table_extension_constraint.h"
#include "real_tuple.h"
#include <cmath>


// ---------------------------
// constructor and destructor
// ---------------------------


Positive_Compact_Table_Extension_Constraint::Positive_Compact_Table_Extension_Constraint (vector<Variable *> & var): Compact_Table_Extension_Constraint (var)
// constructs an extension constraint based on compact table whose scope is defined by the variables in var
{
  residues.resize(arity);
  for (unsigned int i = 0; i < arity; i++)
    residues[i].resize(scope_var[i]->Get_Domain()->Get_Initial_Size(),-1);
}


Positive_Compact_Table_Extension_Constraint::Positive_Compact_Table_Extension_Constraint (Positive_Compact_Table_Extension_Constraint & c): Compact_Table_Extension_Constraint (c)
// constructs a new constraint by copying the constraint c 
{
  residues = c.residues;
}


//-----------------
// basic functions
//-----------------


Constraint * Positive_Compact_Table_Extension_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
  return new Positive_Compact_Table_Extension_Constraint(*this);
}


bool Positive_Compact_Table_Extension_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
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
            return true;
        }
      }
    }
    word_num++;
  }

  return false;
}


bool Positive_Compact_Table_Extension_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
  
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	int i = 0;
	int v;
		
  int * ts;
  bool has_no_support;
  bool record_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
	
	do
	{    
		v = dx->Get_Remaining_Value (i);
		
		ts = ls->Get_Support (num,x,v);
		
		if (ts == 0) 
		{
			has_no_support = true;
			record_support = true;
		}
		else
			{	
				if (Is_Valid (scope_var,ts))
				{
					has_no_support = false;
					record_support = false;
				}
				else 
					{
						has_no_support = true;
						record_support = true;
					}
			}
    
    if (has_no_support)
    {
      vector<unsigned long>::iterator iter = data[x][v].begin();
      unsigned int num_word = 0;
      int t[arity];
      t[x] = v;
      
      while ((has_no_support) && (iter != data[x][v].end()))
      {     
        for (unsigned int i = 0; (i < WORD_SIZE) && (has_no_support); i++)
        {
          if ((((*iter)>> i) & 1ul) == 1)
          {
            bool ok = true;
            for (unsigned int j = 0; (j < arity) && (ok); j++)
              if (j != x)
              {
                unsigned int k = 0;
                unsigned int size = scope_var[j]->Get_Domain()->Get_Size();
               
                while ((k < size) && (((data[j][scope_var[j]->Get_Domain()->Get_Remaining_Value(k)][num_word] >> i) & 1ul) != 1))
                  k++;
              
                ok = k < size;
                t[j] = scope_var[j]->Get_Domain()->Get_Remaining_Value(k);
              }
              
            if (ok)
              has_no_support = false;
          }
        }
     
        iter++;			
        num_word++;
      }
      
      if (has_no_support)
      {
        ds->Add_Element (scope_var[x]);
        dx->Filter_Value (v);
        dx_size--;

        modif = true;
      }
      else 
        {
          if (record_support)
          {
            ds->Add_Support  (num,x,v,arity,t);
            ls->Change_Support (num,x,v,t,arity);
          }
          i++;
        }
    }
    else i++;
  }
  while (i < dx_size);

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
      
  return modif;
}


void Positive_Compact_Table_Extension_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
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


bool Positive_Compact_Table_Extension_Constraint::Update_Table (CSP * pb, Deletion_Stack * ds, timestamp ref)
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
        current_table.Add_To_Mask (data[*iter_pos][dx->Get_Deleted_Value(j)]);

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


void Positive_Compact_Table_Extension_Constraint::Filter_Domain (Deletion_Stack * ds)
// filters the domains
{
  for (set<unsigned int>::iterator iter_pos = S_sup.begin(); iter_pos != S_sup.end(); iter_pos++)
  {
    Domain * dx = scope_var[*iter_pos]->Get_Domain();
   
    for (int i = dx->Get_Size()-1 ; i >= 0; i--)
    {
      int v = dx->Get_Remaining_Value(i);
      int index = residues[*iter_pos][v];
   
      if ((current_table.Get_Word(index) & data[*iter_pos][v][index]) == 0ul)
      {
        index = current_table.Intersect_Index (data[*iter_pos][v]);
        if (index != -1)
          residues[*iter_pos][v] = index;
        else 
          {
            ds->Add_Element (scope_var[*iter_pos]);
            dx->Filter_Value(v);
          }
      }
    }
    
    if (dx->Get_Size() == 0)
      throw ("Domaine vide " + to_string (num));
  }
}


string Positive_Compact_Table_Extension_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<extension>\n";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</list>\n";
  
  expr += "  <supports> ";
  
 	Real_Tuple t (arity);
	t.Complete (scope_var);
	int * real_t;
	bool again;
	
	do
	{
    if (Is_Satisfied (t.Get_Values()))
		{
      real_t = t.Get_Real_Values ();
      expr += "(";
			for (unsigned int i = 0; i < arity; i++)
      {
        expr += to_string(real_t[i]);
        if (i < arity -1)
          expr += ",";
      }
			expr += ")";
		}
		again = t.Next_Valid (scope_var);	
	}
	while (again);

  expr += "  </supports>\n";
  
  expr += "</extension>";
  
  return expr;
}
