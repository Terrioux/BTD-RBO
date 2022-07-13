#include "ac.h"
#include "no_deletion_stack.h"
#include "nogood_base_global_constraint.h"
#include "unary_nogood_base_global_constraint.h"
#include "information.h"

//----------------------------
// constructor and destructor
//----------------------------

AC::AC (CSP * pb, Support * ls, Constraint_Selection * cs, Filtering_Data_Structure<int> * acds)
// constructs an AC algorithm using support structure ls, the constraint selection policy cs and the AC data structure acds which will be achieved in one-pass if one_pass is true, until the fix-point is reached otherwise
{
 	structure = acds;
	is_selected.resize (pb->Get_M(),false);
	last_check.resize (pb->Get_M(),0);
	supports = ls;
	selection = cs;

}


AC::~AC ()
// destructor
{
	delete supports;
	delete selection;
  delete structure;
}

//----------------
// basic function
//----------------


bool AC::Achieve (CSP * pb, Assignment & A, Deletion_Stack * ds, int var)
// achieves, from the variable var (if any), the current AC algorithm on the CSP pb under the assignment A and returns true if the problem is consistent w.r.t. this algorithm, false otherwise (the deletion of values are recorded in the stack ds)
{
	int num;
	Multi_Hypergraph * h = pb->Get_Structure();
	vector<Variable *>::iterator iter2;
	Constraint * c;	

  pb->Reset_Conflict();

  supports->Resize();
  is_selected.resize (pb->Get_M(),false);
  last_check.resize (pb->Get_M(),0);
  
  Event_Manager * event_manager = pb->Get_Event_Manager();
  timestamp current_level_time = event_manager->Get_Current_Level_Time() - 1;

  set<int> modified;
    
  if (var == -1) 
  {    
    for (unsigned int i = 0; i < pb->Get_M(); i++)
    {
      structure->Insert (i);
      is_selected[i] = true;
    }
    pb->Get_Unary_Nogood_Base()->Propagate (pb,A,supports,ds,0);
  }
	else
    {
      for (unsigned int i = 0; i < pb->Get_M(); i++)
        is_selected[i] = false;

      if ((A.Get_Size() == 0) || (var != A.Last_Assigned()))
      {
        pb->Get_Unary_Nogood_Base()->Propagate (pb,A,supports,ds,0);

        if (pb->Get_Last_Conflict_Variable() != 0)
        {
          event_manager->New_Level();          
          return false;
        }
        
        if (pb->Get_Nogood_Base() != 0)
        {
          pb->Get_Nogood_Base()->Propagate (pb,A,supports,ds,0);

          if (pb->Get_Last_Conflict_Variable() != 0)
          {
            event_manager->New_Level();          
            return false;
          }
        }
      }

      modified = event_manager->Get_Current_Modified_Variables ();
           
      for (set<int>::iterator iter_var = modified.begin(); iter_var != modified.end(); iter_var++)
        for (list<Edge*>::iterator iter = h->Begin_Edge_List(*iter_var); iter != h->End_Edge_List(*iter_var); iter++)
        {
          num = (*iter)->Get_Num();

          if ((pb->Get_Constraint(num)->Impacted_By_Last_Events(pb,current_level_time)) && (! is_selected[num]) && (selection->Is_Eligible (pb->Get_Constraint(num),var)))
          {
            last_check[num] = current_level_time;
            structure->Insert (num);
            is_selected[num] = true;
          }
        }
    }
    
	while (! structure->Is_Empty ())
	{
  	int num = structure->Current ();
  	structure->Remove();

		c = pb->Get_Constraint (num);

    is_selected[num] = false;
    
    Solving_Constraint_Check_Counter++;

    event_manager->Update ();

    c->Propagate (pb,A,supports,ds,last_check[num]);

    last_check[num] = event_manager->Get_Current_Time();

    if (pb->Get_Last_Conflict_Variable() != 0)
    {
      structure->Clear ();
      event_manager->New_Level();
      
      return false;
    }

    modified = event_manager->Get_Current_Modified_Variables ();

    for (set<int>::iterator iter_var = modified.begin(); iter_var != modified.end(); iter_var++)
      for (list<Edge*>::iterator iter = h->Begin_Edge_List(*iter_var); iter != h->End_Edge_List(*iter_var); iter++)
      {
        int num2 = (*iter)->Get_Num();
        if (num != num2)
        {
          if (last_check[num2] < current_level_time)
            last_check[num2] = current_level_time;
          
          if ((pb->Get_Constraint(num2)->Impacted_By_Last_Events(pb,last_check[num2])) && (! is_selected[num2]) && (selection->Is_Eligible (pb->Get_Constraint(num2),var)))
          {
            structure->Insert (num2);
            is_selected[num2] = true;
          }
        }
      }
  }
  
  event_manager->New_Level();
    
  return true;
}


bool AC::Achieve (CSP * pb)
// achieves the current AC algorithm on the CSP pb and returns true if the problem is consistent w.r.t. this algorithm, false otherwise 
{
	Assignment A (pb->Get_N());
	Deletion_Stack * ds = new No_Deletion_Stack ();

	try
	{
		bool res = Achieve (pb,A,ds,-1);
		delete ds;
		return res;
	}
	
	catch (...)
	{
		delete ds;
		throw;
	}
}
