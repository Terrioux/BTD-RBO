#include "restart_based_optimizer.h"
#include <climits>
#include <stack>
#include "nogood_base_global_constraint.h"
#include "unary_nogood_base_global_constraint.h"



//----------------------------
// constructor and destructor
//----------------------------


Restart_Based_Optimizer::Restart_Based_Optimizer (COP * inst, Variable_Heuristic * var_heur, Value_Heuristic * val_heur, AC * cc, Deletion_Stack * stack, Restart_Policy *rp, Update_Policy * update_pol, double limit): solution (inst->Get_N()), A (inst->Get_N())
// constructs a optimizer in order to solve the instance inst by using the variable heuristic var_heur, the value heuristic val_heur, the arc-consistency cc and the deletion stack stack. The runtime is limited to limit
{
	pb = inst;
	state = NOT_RUN_YET;
  if (pb->Get_Objective () == MINIMIZE)
    solution_cost = INT_MAX;
  else solution_cost = INT_MIN;

	variable_heuristic = var_heur;
	value_heuristic = val_heur;
  value_heuristic->Set_Solution_Assignment(&solution);
	ac = cc;
	ds = stack;
	time_limit = limit;
  update = update_pol;
  restart_policy = rp;
}


Restart_Based_Optimizer::~Restart_Based_Optimizer ()
// destructor
{	
  delete value_heuristic;
  delete variable_heuristic;
  delete ac;
  delete ds;
  delete update;
  delete restart_policy;
}
		

//-----------------
// basic functions
//-----------------


Optimizer_State Restart_Based_Optimizer::Solve () 
// solves the instance and returns the optimizer state when the solving finishes by proving (in)consistency or by reaching the timeout
{
	Timer T;
	
	T.Start ();

	int max_value_number = 0;						// the largest number of values in a connected component 
	bool * is_negative_node = 0;				// true if the node is a negative node, false otherwise
	Variable ** variable_node = 0;			// variable_node[i] is the number of the variable for the node i
	int * value_node = 0;								// value_node[i] is the value assigned to the variable of the node i

	int result = -1;      // the current status of the problem (-1 if unknown, 0 if there is no more solution, 1 if the solver finds a solution, 2 if the solver must restart)
	unsigned int n = pb->Get_N();
	stack<int> S;
	Multi_Hypergraph * h = pb->Get_Structure();
	unsigned int nb_marked = 0;
	bool * is_marked = new bool [n];
	for (unsigned int i = 0; i < n; i++)
		is_marked[i] = false;

  Nogood_Base_Global_Constraint * nogood_base = 0;
  
  vector<Variable*> candidates;     // the set of variables which can be chosen by the variable heuristic
  candidates.reserve(n);

	Variable * x_obj = pb->Get_Objective_Variable();    // the objective variable
	int var_obj = x_obj->Get_Num();                     // the number of the objective variable
  bool contains_x_obj;    // true if the current connected component contains the objective variable, false otherwise
  
	while ((Solving_Timer.Get_Duration () < time_limit) && (result == -1))
	{
		// we look for the first untreated variable, computes the connected component in which it appears and initialize the variable heuristic
		int value_number = 0;
    
    unsigned int x0 = 0;
    
    if (nb_marked == 0)
    {
      contains_x_obj = true;
      x0 = var_obj;      
    }
    else
      {
        contains_x_obj = false;
        x0 = 0;
        while ((x0 < pb->Get_N()) && (is_marked[x0]))
          x0++;
        A.Clear();
      }
		
		S.push(x0);
		is_marked[x0] = true;
		nb_marked++;
		unsigned int size_cc = 0;
		
		do
		{
			x0 = S.top();
			S.pop();
			
			candidates.push_back(pb->Get_Variable(x0));
			value_number += pb->Get_Variable(x0)->Get_Domain()->Get_Size();
			size_cc++;
			
			for (list<Edge *>::iterator iter_edge = h->Begin_Edge_List (x0); iter_edge != h->End_Edge_List (x0); iter_edge++)
				for (set<vertex>::iterator iter_v = (*iter_edge)->Begin(); iter_v != (*iter_edge)->End(); iter_v++)
					if (! is_marked[*iter_v])
					{
						S.push (*iter_v);
						is_marked[*iter_v] = true;
						nb_marked++;
					}
		}
		while (! S.empty());

		// we start solving the instance
		
		Multi_Hypergraph * h = pb->Get_Structure();
		list<Edge *>::iterator iter;
		Domain * d;
		Variable * x;

		if (value_number > max_value_number)
		{
			max_value_number = value_number;
			delete [] is_negative_node;
			delete [] variable_node;
			delete [] value_node;
			is_negative_node = new bool [max_value_number];
			variable_node = new Variable * [max_value_number];
			value_node = new int [max_value_number];
		}

		int depth = -1;
		
		int var = -1;
		int val = -1;
		pair<int,int> nogood [n];
		unsigned int initial_size = A.Get_Size();
    
    bool init = contains_x_obj;

		while ((Solving_Timer.Get_Duration () < time_limit) && (result == -1))
    {
      if (init)
      {
        result = update->Initialize_Problem (ac,ds);
        init = false;
      }

      bool is_consistent = true;							// true if the current assignment is consistent, false otherwise
      
      while ((Solving_Timer.Get_Duration () < time_limit) && (result == -1))
      {
        Solving_Timer.Update ();

        if (is_consistent)
        {
          if (A.Get_Size() == size_cc)
          {
            // all the variables are assigned consistently: we have a solution
            result = 1;            
          }
          else
            {
              // we choose a new variable and assign it
              x = variable_heuristic->Next_Variable (A,candidates);
              var = x->Get_Num();
              val = value_heuristic->Next_Value (A,var);
              
              depth++;
              is_negative_node[depth] = false;
              variable_node[depth] = x;
              value_node[depth] = val;
              
              A.Assign(var,val);
              Solving_Decision_Counter++;
              Solving_Node_Counter++;
        
              ds->New_Level();
              
              // we update the number of assigned variables in each constraint scope
              for (iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
                pb->Get_Constraint((*iter)->Get_Num())->Dec_Unassigned_Variable_Number ();
              
              // we remove all the following values of var
              d = x->Get_Domain();
              
              ds->Add_Element (x);
              d->Assign (val);
            }
        }
        else
          {
            // we backtrack classically
        
            while ((depth >= 0) && (is_negative_node[depth]))
            {
              // we restore the removed values
              ds->Restore(pb);
              depth--;
            }
            
            if (depth == -1)
            {
              // there is no solution 
              result = 0;
            }
            else
              {
                // we restore the removed values
                ds->Restore(pb);
                
                // we remove the value val from the variable var (which is the last assigned variable)
                var = A.Last_Assigned ();
                val = A.Get_Value (var);
                d = pb->Get_Variable (var)->Get_Domain();
                
                ds->New_Level();
                ds->Add_Element (pb->Get_Variable (var));
                d->Filter_Value (val);
                
                // we unassign the last assigned variable and add it as candidate for the variable heuristic
                A.Unassign ();

                // we update the number of assigned variables in each constraint scope
                for (iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
                  pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();
                  
                // the current node becomes a negative node
                is_negative_node[depth] = true;

                Solving_Decision_Counter++;
                Solving_Backtrack_Counter++;
              }
            
          if (result == -1)
          {
            if ((restart_policy != 0) && (restart_policy->Restart()))
            {
              if ((d->Get_Size() == 0) && (A.Get_Size() == initial_size))		// particular case where a domain is empty and the current assignment contains no positive decision
              {
                // there is no solution 	
                result = 0;

                for (int i = depth; i >= 0; i--)
                {
                  // we restore the removed values
                  ds->Restore(pb);

                  if (! is_negative_node[i])
                  {
                    A.Unassign ();

                    // we update the number of assigned variables in each constraint scope
                    var  = variable_node[i]->Get_Num();
                    for (iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
                      pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();
                  }
                }
                depth = -1;
              }
              else result = 2;    // the solver must be restart
            }
          }
        }

        // we check the consistency of the current assignment
        if (result == -1)
        {			
          Solving_Decision_Counter++;
          if (d->Get_Size() > 0)
          {
            variable_heuristic->Update_Before_Filtering(A,var);
            
            is_consistent = ac->Achieve (pb,A,ds,var);
            
            if (!is_consistent)
              Solving_Conflict_Counter++;
            
            variable_heuristic->Update_After_Filtering(A,var);
          }
          else
            {
              is_consistent = false;
              pb->Get_Event_Manager()->New_Level();
            }
        }
      }

      if (result == 1)
      {
        // we have a solution for the current connected component
        if (contains_x_obj)
        {
          // we have found a first solution or a better solution
          solution = A;
          solution_cost = A.Get_Value(pb->Get_Objective_Variable()->Get_Num());
          cout << "o ";
          if (pb->Get_Objective() == MINIMIZE)
            cout << pb->Get_Objective_Variable()->Get_Domain()->Get_Real_Value(solution_cost) << endl;
          else cout << -pb->Get_Objective_Variable()->Get_Domain()->Get_Real_Value(solution_cost) << endl;
        }
        else 
          {
            // we complete the solution with the assignment A
            solution.Merge(A);
          }
      }
      
      if (result >= 1)
      {
        pb->Get_Event_Manager()->New_Level();
        
        // we perform a restart
        Solving_Restart_Number_Counter++;

        // we unassign all the assigned variables
        for (int i = depth; i >= 0; i--)
        {
          // we restore the removed values
          ds->Restore(pb);

          if (! is_negative_node[i])
          {
            A.Unassign ();

            // we update the number of assigned variables in each constraint scope
            var  = variable_node[i]->Get_Num();
            for (iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
              pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();
          }
        }

        if ((result == 2) || ((result == 1) && (contains_x_obj) && (update->Record_Nogoods())))
        {
          // we record some nogoods

          int nb_positive_node = 0;
          for (int i = 0; i <= depth; i++)
          {
            if (is_negative_node[i])	
            {
              if (nb_positive_node == 0)
              {
                pb->Get_Unary_Nogood_Base ()->Add_Nogood(pb,variable_node[i]->Get_Num(),value_node[i]);
              }
              else
                {
                  nogood[nb_positive_node].first = variable_node[i]->Get_Num();
                  nogood[nb_positive_node].second = value_node[i];

                  if (nogood_base == 0)
                  {
                    nogood_base = new Nogood_Base_Global_Constraint();
                    pb->Add_Constraint (nogood_base,false);
                    variable_heuristic->Add_Constraint (nogood_base);
                  }
                  
                  nogood_base->Add_Nogood(pb,nogood,nb_positive_node+1);
                }
            }
            else
              {
                nogood[nb_positive_node].first = variable_node[i]->Get_Num();
                nogood[nb_positive_node].second = value_node[i];
                nb_positive_node++;
              }
          }
        }

        // we reset some variables
        depth = -1;
        variable_heuristic->Update_When_Restarting ();
      }

      if ((result == 0) || (result == 1))
      {
        if (contains_x_obj)
          result = update->Update_Problem(result, solution, ac,ds);
      }
      else
        if (result == 2)
          result = -1;

      if (nogood_base != 0)
        nogood_base = pb->Get_Nogood_Base();    // useful depending on the modification we have done on the nogood base
    }

		if (result == 1) 
    {
      if (nb_marked != n)
      {
        result = -1;
        if (restart_policy != 0) 
          restart_policy->Reset();
        candidates.clear();
        variable_heuristic->Update_When_New_Connected_Component();
      }
      else result = 0;
    }
	}

	delete [] is_negative_node;
	delete [] variable_node;
	delete [] value_node;
	delete [] is_marked;


	switch (result)
	{
		case -1: if (solution.Get_Size() == n)
               state = HAS_FOUND_SOLUTION;
             else state = HAS_BEEN_STOPPED;
             break;
		case  0: if (solution.Get_Size() == n)
               state = HAS_FOUND_OPTIMUM;
             else state = HAS_FOUND_INCONSISTENT;
             break;
	}
  
  T.Stop();
  Solving_Time_Counter.Add (T.Get_Duration ());
	
	return state;
}
