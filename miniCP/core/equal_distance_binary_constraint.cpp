#include "equal_distance_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Equal_Distance_Binary_Constraint::Equal_Distance_Binary_Constraint (vector<Variable *> & var, int cst): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that the distance must ne equal to cst
{
	constant = cst;
	
	equal = new pair<int,int*> * [2];
	equal[0] = new pair<int,int*> [scope_var[0]->Get_Domain()->Get_Initial_Size()];
	equal[1] = new pair<int,int*> [scope_var[1]->Get_Domain()->Get_Initial_Size()];
	int val;

	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
	{
		equal[1][w].first = 0;
		equal[1][w].second = new int [2];
	}
	
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
	{
		equal[0][v].first = 0;
		equal[0][v].second = new int [2];
		val = scope_var[0]->Get_Domain()->Get_Real_Value(v);
		for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
			if (abs (val - scope_var[1]->Get_Domain()->Get_Real_Value(w)) == constant)
			{
				equal[0][v].second[equal[0][v].first] = w;
				equal[0][v].first++;
				
				equal[1][w].second[equal[1][w].first] = v;
				equal[1][w].first++;
			}
	}
}


Equal_Distance_Binary_Constraint::Equal_Distance_Binary_Constraint (Equal_Distance_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	constant = c.constant;

	equal = new pair<int,int*> * [2];
	equal[0] = new pair<int,int*> [scope_var[0]->Get_Domain()->Get_Initial_Size()];
	equal[1] = new pair<int,int*> [scope_var[1]->Get_Domain()->Get_Initial_Size()];
	
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
	{
		equal[0][v].first = c.equal[0][v].first;
		equal[0][v].second = new int [2];
		for (int i = 0; i < c.equal[0][v].first; i++)
			equal[0][v].second[i] = c.equal[0][v].second[i];
	}
	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
	{
		equal[1][w].first = c.equal[1][w].first;
		equal[1][w].second = new int [2];
		for (int i = 0; i < c.equal[1][w].first; i++)
			equal[1][w].second[i] = c.equal[1][w].second[i];
	}
}


Equal_Distance_Binary_Constraint::~Equal_Distance_Binary_Constraint ()
// destructor
{
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
		delete [] equal[0][v].second;
		
	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
		delete [] equal[1][w].second;
	delete [] equal[0];
	delete [] equal[1];
	delete [] equal;
}


//-----------------
// basic functions
//-----------------


bool Equal_Distance_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return abs(scope_var[0]->Get_Domain()->Get_Real_Value(t[0]) - scope_var[1]->Get_Domain()->Get_Real_Value(t[1])) == constant;
}


bool Equal_Distance_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	int y = 1 - x;
	Domain * dx = scope_var[x]->Get_Domain();
	Domain * dy = scope_var[y]->Get_Domain();
	
	bool modif = false;    // true if a deletion is achieved, false otherwise
	int dx_size = dx->Get_Size();

	int v;
  int i = 0;
	int j;
	do
	{
		v = dx->Get_Remaining_Value(i);
	
		j = 0;
		while ((j < equal[x][v].first) && (! dy->Is_Present (equal[x][v].second[j])))
			j++;

		if (j == equal[x][v].first)
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			modif = true;
			dx_size--;
		}
		else i++;
	}
	while (i < dx_size);
	
  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);
  
	return modif;
}


void Equal_Distance_Binary_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  if (ref == 0)
  {
    for (vector<Variable *>::iterator iter = Begin_Var(); iter != End_Var(); iter++)
      if ((! A.Is_Assigned ((*iter)->Get_Num())) && (Revise (pb,(*iter)->Get_Num(),ls,ds)) && ((*iter)->Get_Domain()->Get_Size() == 0))
        return;
  }
  else
  {
    Event_Manager * event_manager = pb->Get_Event_Manager();
    int fix_var;   // a variable which has a singleton domain
    
    if (event_manager->Exist_Event_Fix (scope_var[0]->Get_Num(), ref))
      fix_var = 0;
    else
      if (event_manager->Exist_Event_Fix (scope_var[1]->Get_Num(), ref))
        fix_var = 1;
      else fix_var = -1;
          
    timestamp current_time = event_manager->Get_Current_Time();
    
    for (unsigned int i = 0; (i < arity) && (fix_var == -1); i++)
    {
      int num_var = scope_var[i]->Get_Num();
      if (event_manager->Exist_Event_Dmc (num_var, ref))
      {
        Domain * dx = scope_var[i]->Get_Domain();
        
        for (unsigned int v = 0; v < dx->Get_Initial_Size(); v++)
          if ((event_manager->Exist_Event_Dmc (num_var,v,ref)) && (! event_manager->Exist_Event_Dmc (num_var,v,current_time)))
          {
            for (int k = 0; k < equal[i][v].first; k++)
            {
              unsigned int w = equal[i][v].second[k];

              if (scope_var[1-i]->Get_Domain()->Is_Present(w))
              {
                bool to_be_deleted = false;
                
                if (equal[1-i][w].first == 1)
                  to_be_deleted = true;
                else
                  if (equal[1-i][w].first == 2)
                  {
                    if (equal[1-i][w].second[0] == (signed) v)
                      to_be_deleted = ! dx->Is_Present(equal[1-i][w].second[1]);
                    else
                      to_be_deleted = ! dx->Is_Present(equal[1-i][w].second[0]);
                  }
                  
                if (to_be_deleted)
                {
                  ds->Add_Element (scope_var[1-i]);
                  scope_var[1-i]->Get_Domain()->Filter_Value(w);
                }
              }
            }
            
            if (scope_var[1-i]->Get_Domain()->Get_Size() == 0)
            {
              pb->Set_Conflict (scope_var[1-i],this);
              return;
            }
            else
              if (scope_var[1-i]->Get_Domain()->Get_Size() == 1)
                fix_var = 1-i;
          }
      }
    }
    
    if (fix_var != -1)
    {
      assert (scope_var[fix_var]->Get_Domain()->Get_Size() == 1);
      int v = scope_var[fix_var]->Get_Domain()->Get_Remaining_Value(0);     // the remaining value of the variable scope_var[fix_var]
      
      Domain * d = scope_var[1-fix_var]->Get_Domain();

      if (equal[fix_var][v].first == 1)
      {
        ds->Add_Element (scope_var[1-fix_var]);
        d->Assign (equal[fix_var][v].second[0]);
      }
      else
        {
          unsigned int size = d->Get_Size();
          unsigned int j = 0;
          
          while (j < size)
          {
            int w = d->Get_Remaining_Value(j);
            
            int k = 0; 
            while ((k < equal[fix_var][v].first) && (equal[fix_var][v].second[k] != w))
              k++;
            
            if (k == equal[fix_var][v].first)
            {
              ds->Add_Element (scope_var[1-fix_var]);
              d->Filter_Value (w);
              size--;
            }
            else j++;
          }
        }
      
      if (d->Get_Size() == 0)
        pb->Set_Conflict (scope_var[1-fix_var],this);
    }
  }
}


string Equal_Distance_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> eq(dist(" + scope_var[0]->Get_Name() + "," + scope_var[1]->Get_Name()+")," + to_string(constant) + ") </intension>";
}
