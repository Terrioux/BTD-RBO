#include "equal_weighted_sum_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Equal_Weighted_Sum_Global_Constraint::Equal_Weighted_Sum_Global_Constraint (vector<Variable *> & var, vector<int> & w, int cst): Weighted_Sum_Global_Constraint (var,w,cst)
// constructs a new constraint which involves the variable in var and whose relation compares the sum of the values of the variables in var weighted by weight from w to the constant cst with respect to =
{	
	values = new int [arity];
}


Equal_Weighted_Sum_Global_Constraint::Equal_Weighted_Sum_Global_Constraint (Equal_Weighted_Sum_Global_Constraint & c): Weighted_Sum_Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	values = new int [arity];
}


Equal_Weighted_Sum_Global_Constraint::~Equal_Weighted_Sum_Global_Constraint ()
// destructor
{
	delete [] values;
}


//-----------------
// basic functions
//-----------------


bool Equal_Weighted_Sum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	Domain * dy;	
	
	int i = 0;
	int j;
	int real_v;
	
  int * ts;
  bool has_no_support;
  bool record_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
	bool exist_more_tuples;
	int sum = 0;
	int sum_max = 0;
	int sum_min = 0;
  
  for (unsigned int j = 0; j < arity; j++)
    if (j != x)
    {
      dy = scope_var[j]->Get_Domain();
      assert (dy->Get_Size() > 0);
      if (weights[j] > 0)
      {
        sum_min += weights[j] * dy->Get_Real_Min();
        sum_max += weights[j] * dy->Get_Real_Max();
      }
      else
        {
          sum_min += weights[j] * dy->Get_Real_Max();
          sum_max += weights[j] * dy->Get_Real_Min();
        }
    }

	do
	{
		values[x] = dx->Get_Remaining_Value (i);
		real_v = dx->Get_Real_Value (values[x]) * weights[x];
		
		ts = ls->Get_Support (num,x,values[x]);
		
		if (ts == 0) 
		{
			// we start with the minimum value of each variable except for var

			if (sum_max + real_v < constant)
			{
				has_no_support = true;
				record_support = false;
				exist_more_tuples = false;
			}
			else
				{
     			for (unsigned int j = 0; j < arity; j++)
            if (j != x)
            {
              dy = scope_var[j]->Get_Domain();
              if (weights[j] > 0)
                values[j] = dy->Get_Min();
              else values[j] = dy->Get_Max();							
            }

					record_support = true;
					exist_more_tuples = true;
          sum = sum_min + real_v;
					has_no_support = sum != constant;
				}
		}
		else
			{
				sum = constant;
				j = 0; 
				while (((unsigned) j < arity) && (scope_var[j]->Get_Domain()->Is_Present(ts[j])))
					j++;

				if (j == (signed) arity)		// the tuple is still valid
				{
					has_no_support = false;
					record_support = false;
					exist_more_tuples = true;
				}
				else 
					{
						has_no_support = true;
						record_support = true;
						
						if (ls->Continue_From_Scratch())
						{
							if (sum_max + real_v < constant)
							{
								has_no_support = true;
								record_support = false;
								exist_more_tuples = false;
							}
							else
								{
     							for (unsigned int j = 0; j < arity; j++)
                    if (j != x)
                    {
                      dy = scope_var [j]->Get_Domain();
                      if (weights[j] > 0)
                        values[j] = dy->Get_Min();
                      else values[j] = dy->Get_Max();
                    }

									record_support = true;
									exist_more_tuples = true;
                  sum = sum_min + real_v;
									has_no_support = sum != constant;
								}
						}
						else 
							{
								sum = constant;
								for (unsigned int j = 0; j < arity; j++)
									values[j] = ts[j];
								record_support = true;
								exist_more_tuples = true;
								has_no_support = true;
							}
					}
			}
		
		unsigned int current_var_pos = 0;
		
		while ((has_no_support) && (exist_more_tuples))
		{
			// we look for the next compatible tuple by changing the value of the variable at position current_var_pos
				
			if (current_var_pos == x)
			{
				current_var_pos++;
				exist_more_tuples = current_var_pos < arity;
			}
			else
				{
					sum -= weights[current_var_pos] * scope_var[current_var_pos]->Get_Domain()->Get_Real_Value(values[current_var_pos]);
					
					if (weights[current_var_pos] > 0)
						values[current_var_pos] = scope_var[current_var_pos]->Get_Domain()->Next_Value (values[current_var_pos]);
					else values[current_var_pos] = scope_var[current_var_pos]->Get_Domain()->Previous_Value (values[current_var_pos]);

					if ((values[current_var_pos] == -1) || (sum + weights[current_var_pos] * scope_var[current_var_pos]->Get_Domain()->Get_Real_Value (values[current_var_pos]) > constant))
					{
						if (weights[current_var_pos] > 0)
							values[current_var_pos] = scope_var[current_var_pos]->Get_Domain()->Get_Min();
						else values[current_var_pos] = scope_var[current_var_pos]->Get_Domain()->Get_Max();
						sum += weights[current_var_pos] * scope_var[current_var_pos]->Get_Domain()->Get_Real_Value (values[current_var_pos]);
						
						j = current_var_pos-1;
						
						while (j >= 0)
						{
							if (j != (signed) x)
							{
								sum -= weights[j] * scope_var[j]->Get_Domain()->Get_Real_Value (values[j]);
								if (weights[j] > 0)
									values[j] = scope_var[j]->Get_Domain()->Get_Min();
								else values[j] = scope_var[j]->Get_Domain()->Get_Max();
								sum += weights[j] * scope_var[j]->Get_Domain()->Get_Real_Value (values[j]);
							}
							j--;
						}
						current_var_pos++;
						exist_more_tuples = current_var_pos < arity;
					}
					else 
						{
							sum += weights[current_var_pos] * scope_var[current_var_pos]->Get_Domain()->Get_Real_Value (values[current_var_pos]);
							current_var_pos = 0;
							has_no_support = sum != constant;
						}
				}
		}
		
		if (has_no_support)
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (values[x]);
			dx_size--;
			modif = true;
		}
		else 
			{
				if (record_support)
				{
					ds->Add_Support (num,x,values[x],arity,ts);
					ls->Change_Support (num,x,values[x],values,arity);
				}
				i++;
			}
  }
  while (i < dx_size);

  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);  
  return modif;
}


void Equal_Weighted_Sum_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
	int sum_min = 0;
	int sum_max = 0;

	for (unsigned int i = 0; i < arity; i++)
  {
    Domain * dy = scope_var [i]->Get_Domain();
    if (weights[i] > 0)
    {
      sum_min += dy->Get_Real_Min() * weights[i];
      sum_max += dy->Get_Real_Max() * weights[i];
    }
    else 
      {
        sum_min += dy->Get_Real_Max() * weights[i];
        sum_max += dy->Get_Real_Min() * weights[i];
      }
  }

  bool modif;
  do
  {
    modif = false;
    
    for (unsigned int i = 0; i < arity; i++)
    {
      Domain * dx = scope_var[i]->Get_Domain();
      
      if (weights[i] > 0)
      {
        sum_min -= dx->Get_Real_Min() * weights[i];
        sum_max -= dx->Get_Real_Max() * weights[i];
      }
      else 
        {
          sum_min -= dx->Get_Real_Max() * weights[i];
          sum_max -= dx->Get_Real_Min() * weights[i];
        }
      
      for (int j = dx->Get_Size()-1; j >= 0; j--)
      {
        int v = dx->Get_Remaining_Real_Value(j);
        
        if ((v * weights[i] + sum_max < constant) || (v * weights[i] + sum_min > constant))
        {
          ds->Add_Element (scope_var[i]);
          dx->Filter_Value (dx->Get_Remaining_Value(j));
          modif = true;
        }
      }

      if (dx->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[i],this);
        return;
      }
      
      if (weights[i] > 0)
      {
        sum_min += dx->Get_Real_Min() * weights[i];
        sum_max += dx->Get_Real_Max() * weights[i];
      }
      else 
        {
          sum_min += dx->Get_Real_Max() * weights[i];
          sum_max += dx->Get_Real_Min() * weights[i];
        }   
    }
  }
  while (modif);
}
