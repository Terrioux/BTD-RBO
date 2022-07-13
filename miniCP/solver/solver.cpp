#include "solver.h"
#include "basic_deletion_stack.h"
#include "no_deletion_stack.h"
#include "nogood_base_global_constraint.h"
#include "unary_nogood_base_global_constraint.h"

#include <stack>
#include <queue>
#include <deque>
#include <typeinfo>


//----------------------------
// constructor and destructor
//----------------------------


Solver::Solver (CSP * inst, Variable_Heuristic * var_heur, Value_Heuristic * val_heur, AC * cc, Deletion_Stack * stack, Tree_Decomposition * TD, Root_Heuristic * root_heur, Cluster_Heuristic * cluster_heur, Restart_Policy * rp, Restart_Policy * local_rp, Root_Heuristic * rs_root_heur, Merge_Policy * local_merge_p, double limit): A (inst->Get_N())
// constructs a solver in order to solve the instance inst by using the variable heuristic var_heur, the value heuristic val_heur, the consistency check,the deletion stack stack, the tree-decomposition TD, the root cluster heuristic root_heur, the cluster heuristic cluster_heur, the global restart policy rp, the local restart policy local_rp, the cluster root heuristic rs_root_heur for choosing the root cluster at each restart and the policy local_merge_p for merging clusters. The runtime is limited to limit. Child nodes rely on lexicographical trees.
{
 	pb = inst;
	variable_heuristic = var_heur;
	value_heuristic = val_heur;
	ds = stack;
	time_limit = limit;
	state = NOT_RUN;

	td = new CSP_Tree_Decomposition (TD,pb, false);
	root_heuristic = root_heur;
	cluster_heuristic = cluster_heur;
  candidates = new vector<Variable*> [td->Get_Max_Label ()+1];

	ac = cc;
	restart_policy = rp;
	local_restart_policy = local_rp;
	restart_root_heuristic = rs_root_heur;
  Init ();
  
	merge_policy = local_merge_p; 		

	original_td_merge = new CSP_Tree_Decomposition(td, pb, false);
}


Solver::~Solver ()
// destructor
{
  delete value_heuristic;
  delete variable_heuristic;
  delete ds;

	delete cluster_heuristic;
	delete root_heuristic;
	delete td;
  delete [] candidates;

  
	delete original_td_merge;
	delete merge_policy;

	delete restart_policy;
	if (restart_root_heuristic != root_heuristic)
		delete restart_root_heuristic;
	delete local_restart_policy;

	for (int i = 0; i < max_ct_size; i++)
		delete [] reset_good[i];
		
	delete [] nb_reset_good;
	delete [] reset_good;
	delete [] is_negative_node;
	delete [] variable_node;
	delete [] value_node;
	delete [] cluster_begin_depth;
	delete [] unassigned_variable_number;
	delete [] is_good;
	delete [] nogood;
	delete [] empty_domain;	
}

//-----------------
// basic functions
//-----------------

void Solver::Init()
// initializes general data-structure
{
  max_value_number = 0;
  
  max_ct_size = 0;			// the maximal number of variables
	for (unsigned int num_ct = 0; num_ct < td->Get_Cluster_Tree_Number(); num_ct++)
		if (max_ct_size < td->Get_Cluster_Tree(num_ct)->Get_Nb_Vertices())
			max_ct_size = td->Get_Cluster_Tree(num_ct)->Get_Nb_Vertices();

	is_negative_node = 0;
	variable_node = 0;
	value_node = 0;
  
	reset_good = new int * [max_ct_size];
  nb_reset_good = new int [max_ct_size];

  int max_cluster_number = td->Get_Max_Label ()+1;	// the maximum number of clusters in the current tree-decomposition
	for (int i = 0; i < max_ct_size; i++)
		reset_good[i] = new int [max_cluster_number];
  
  cluster_begin_depth = new int [max_cluster_number];

	empty_domain = new Domain * [max_ct_size];
  
  // data structure related to (no)good management

  nogood = new pair<int,int> [max_ct_size];	
	unassigned_variable_number = new int [max_cluster_number];
	is_good = new bool [max_cluster_number];
  
  h = dynamic_cast<Multi_Hypergraph*>(td->Get_Graphical_Structure());		// a pointer on the constraint hypergraph
}


Solver_State Solver::Solve ()
// solves the instance and returns the solver state when the solving finishes by proving (in)consistency or by reaching the timeout
{	
	Multi_Hypergraph * h = pb->Get_Structure();		// a pointer on the constraint hypergraph
	Domain * d = 0;
	Variable * x; 
	Variable * x_sim;
	int var;
	int val;

	// data structure related to the solving
	
	bool BJ = false;															// true if we have to backjump after having recorded a nogood, false otherwise
	Child_Node * last_nogood_separator = 0;				// the separator over which was recorded the last nogood
	Child_Node * sep;															// a separator
	int root_cluster_num;													// the number of the root cluster
	int new_root_cluster_num;											// the number of the new root cluster
	int current_cluster_num;											// the number of the current cluster
	int parent_cluster_num;												// the number of the parent cluster of the current cluster (used only for updating the data structure when restarting with a new root cluster)
	Cluster_Node * current_cluster;								// the current cluster
	Cluster_Node * next_cluster;
	
	int depth = -1;										// the depth of the current branch in the search tree

	bool has_not_performed_restart = true;		// true if a restart has just been triggered, false otherwise
	bool is_consistent;												// true if the current assignment is consistent, false otherwise	

	bool is_merged = false;

	bool again; 																			// true if another child cluster must be chosen, false otherwise

	int result = -1;		// 0 if we have proved that no solution exists, 1 if we have found a solution, -1 if the consistency of pb is unknown

  int max_cluster_number = td->Get_Max_Label ()+1;	// the maximum number of clusters in the current tree-decomposition
	vector<Cluster_Node *> * merged_clusters = new vector<Cluster_Node *> [max_cluster_number];
	
	for (int i = 0; i < max_cluster_number; i++)
			merged_clusters[i].reserve (original_td_merge->Nb_Clusters());

  Nogood_Base_Global_Constraint * nogood_base = pb->Get_Nogood_Base();
  
  vector<Variable *> simulation_candidates;   // the set of variables which can be chosen by the variable heuristic when simulating a merging
  simulation_candidates.reserve(pb->Get_N());
	
	for (unsigned int num_ct = 0; (num_ct < td->Get_Cluster_Tree_Number()) && (result != 0) && (Solving_Timer.Get_Duration () < time_limit); num_ct++)
	{			
		// we consider the current cluster tree of the tree-decomposition td
		Cluster_Tree * ct = td->Get_Cluster_Tree(num_ct);
		
		Cluster_Tree * original_ct = original_td_merge ->Get_Cluster_Tree(num_ct);
		
		for (int i = 0; i <= ct->Get_Max_Label(); i++)
			merged_clusters[i].clear();
		
		for (int i = 0; i <= ct->Get_Max_Label(); i++)
			merged_clusters[i].push_back (original_ct->Get_Cluster(i));

		if (ct->Get_Nb_Vertices() == 1)
		{
			x = pb->Get_Variable (*(ct->Get_Cluster(0)->begin()));
			var = x->Get_Num();
			val = value_heuristic->Next_Value (A,var);
			A.Assign(var,val);
			Solving_Node_Counter++;
      Solving_Decision_Counter++;

			result = 1;
		}
		else
			{			
        variable_heuristic->Update_When_New_Connected_Component();

				// choice of the root cluster
				ct->Set_Root (root_heuristic->Choose_Root (ct,pb));
				root_cluster_num = ct->Get_Root_Num ();
				original_ct->Set_Root (root_cluster_num);
				
				int value_number = 0;
        
				for (map<int,Cluster_Node *>::iterator iter_cl = ct->Begin_List (); iter_cl != ct->End_List (); iter_cl++)
				{
					// initialization of the cluster and variable heuristics
					if (iter_cl->first == root_cluster_num)
						unassigned_variable_number [iter_cl->first] = iter_cl->second->Size();
					else unassigned_variable_number [iter_cl->first] = iter_cl->second->Size() - iter_cl->second->Get_Separator()->Size();
											
					if (merge_policy != 0)
						merge_policy->Reset(iter_cl->first);
		
					// initialization of the variable heuristic
          candidates[iter_cl->first].clear();
					for (vector<vertex>::iterator iter_var = iter_cl->second->begin(); iter_var != iter_cl->second->end(); iter_var++)
					{
						candidates[iter_cl->first].push_back (pb->Get_Variable(*iter_var));
						if ((iter_cl->first == root_cluster_num) || (! iter_cl->second->Get_Separator()->Is_Element(*iter_var)))
							value_number += pb->Get_Variable(*iter_var)->Get_Domain()->Get_Size();
					}
				}
				
				if (max_value_number < value_number)
				{
					max_value_number = value_number;
					
					delete [] is_negative_node;
					delete [] variable_node;
          delete [] value_node;
          is_negative_node = new bool [max_value_number];
					variable_node = new Variable * [max_value_number];
					value_node = new int [max_value_number];
				}


				int empty_domain_nb = 0;

				int max_depth;
				result = -1;
				
				for (int i = ct->Get_Nb_Vertices()-1; i >=0; i--)
					nb_reset_good[i] = 0;
				
				unsigned int initial_size = A.Get_Size();
				
				if (restart_policy != 0)
					restart_policy->Reset();

				while ((Solving_Timer.Get_Duration () < time_limit) && (result == -1))			// this loop allows us to run again the resolution when a restart is triggered
				{
					// initialization of is_good data structure
					for (int i = 0; i <= ct->Get_Max_Label(); i++)
						is_good[i] = false;
								
					// we start solving the instance
					var = -1;
					val = -1;
					depth = -1;
					is_consistent = true;
					
					current_cluster_num = root_cluster_num;
					current_cluster = ct->Get_Root();
					cluster_begin_depth[root_cluster_num] = -1;
					
					
					if (merge_policy != 0)
						merge_policy->Reset(current_cluster_num);

					while ((result == -1) && (has_not_performed_restart) && (Solving_Timer.Get_Duration () < time_limit))
					{
						if (is_consistent)
						{
							if (unassigned_variable_number [current_cluster_num] == 0)
							{
								do
								{
									next_cluster = cluster_heuristic->Choose_Next_Cluster (ct,current_cluster,pb,is_good);
									if (next_cluster == 0) 
                    again = false;
									else
										{
											sep = next_cluster->Get_Separator();
											if (dynamic_cast<CSP_Child_Node *>(sep)->Is_Good (A))
											{
												is_good[next_cluster->Get_Num()] = true;
												
												max_depth = -1;
												for (vector<vertex>::iterator iter_v = sep->begin(); iter_v != sep->end(); iter_v++)
													if (A.Get_Position(*iter_v) > max_depth)
														max_depth = A.Get_Position(*iter_v);

												max_depth -= initial_size;
												reset_good[max_depth][nb_reset_good[max_depth]] = next_cluster->Get_Num();
												nb_reset_good[max_depth]++;
												
												again = true;
											}
											else again = false;
										}
								}
								while (again);
								
								while ((next_cluster == 0) && (result == -1))
								{
									// we record a new good (if the current cluster is not the root cluster)
									if (current_cluster_num == root_cluster_num)
									{
										// we have found a assignment which is consistent on each cluster: we have a solution
										result = 1;
									}
									else
										{
											// we record the good on the current tree-decomposition
											sep = current_cluster->Get_Separator();
											dynamic_cast<CSP_Child_Node *>(sep)->Record_Good (A);
											
											// we record the good on the original tree-decomposition
											for (vector<Cluster_Node *>::iterator iter_cn = merged_clusters[current_cluster_num].begin(); iter_cn != merged_clusters[current_cluster_num].end(); iter_cn++)
												dynamic_cast<CSP_Child_Node *>((*iter_cn)->Get_Separator())->Record_Good(A);
											
											
											is_good [current_cluster_num] = true;
											
											max_depth = -1;
											for (vector<vertex>::iterator iter_v = sep->begin(); iter_v != sep->end(); iter_v++)
												if (A.Get_Position(*iter_v) > max_depth)
													max_depth = A.Get_Position(*iter_v);
											
											max_depth -= initial_size;
											reset_good[max_depth][nb_reset_good[max_depth]] = current_cluster_num;
											nb_reset_good[max_depth]++;
																														
											// we unassign each variable of the current cluster before going back to its parent cluster
											while (depth != cluster_begin_depth [current_cluster_num])
											{
												// we restore the removed values
												ds->Restore(pb);
												
												if (! is_negative_node[depth])
												{
													var = A.Last_Assigned ();
													
													// we restore the removed values
													A.Unassign ();
													unassigned_variable_number [current_cluster_num]++;

													// we update the number of assigned variables in each constraint scope
													for (list<Edge *>::iterator iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
														pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();
														
													// we reset some goods if necessary
													max_depth = A.Get_Size() - initial_size;
													for (int i = 0; i < nb_reset_good[max_depth]; i++)
														is_good[reset_good[max_depth][i]] = false;
													nb_reset_good[max_depth] = 0;
												}
												
												depth--;
											}
										
											// we go back to the parent cluster 
																
											current_cluster_num = current_cluster->Get_Parent_Num ();
											current_cluster = current_cluster->Get_Parent();
											
											// we look for a new child cluster of the current cluster
											do
											{
												next_cluster = cluster_heuristic->Choose_Next_Cluster (ct,current_cluster,pb,is_good);
												if (next_cluster == 0) again = false;
												else
													{
														sep = next_cluster->Get_Separator();
														if (dynamic_cast<CSP_Child_Node *>(sep)->Is_Good (A))
														{
															is_good[next_cluster->Get_Num()] = true;
															
															max_depth = -1;
															for (vector<vertex>::iterator iter_v = sep->begin(); iter_v != sep->end(); iter_v++)
																if (A.Get_Position(*iter_v) > max_depth)
																	max_depth = A.Get_Position(*iter_v);

															max_depth -= initial_size;
															reset_good[max_depth][nb_reset_good[max_depth]] = next_cluster->Get_Num();
															nb_reset_good[max_depth]++;
															
															again = true;
														}
														else again = false;
													}
											}
											while (again);
										}
								}
								
								if (result == -1)
								{
									current_cluster = next_cluster;
									current_cluster_num =  current_cluster->Get_Num();
									cluster_begin_depth[current_cluster_num] = depth;
									
									// init verification
									if (merge_policy != 0)
										merge_policy->Reset(current_cluster_num);
								}
							}
						}

						if (result == -1)
						{
							if (is_consistent)
							{
								// we choose a new variable and assign it
								x = variable_heuristic->Next_Variable (A,candidates[current_cluster_num]);
								var = x->Get_Num();
								val = value_heuristic->Next_Value (A,var);
								
								// we use simulation to know which variable would be chosen if we had more freedom
								
								if (merge_policy != 0)
								{
									simulation_candidates.clear();
									
									simulation_candidates.push_back(x);	
									
									// adding the winner of each child
									for (deque<Child_Node *>::iterator iter2 = ct->Begin_Neighbor(current_cluster_num); iter2 != ct->End_Neighbor (current_cluster_num); iter2++)
									{	
										if ((*iter2)->Get_Cluster_Num() != current_cluster->Get_Parent_Num())		// we consider only the separator with a cluster different from the parent cluster
										{
											x_sim = variable_heuristic->Next_Variable (A,candidates[(*iter2)->Get_Cluster_Num()]);
											simulation_candidates.push_back(x_sim);	
										}						
									}
									
									// choose among the winners
									x_sim = variable_heuristic->Next_Variable (A,simulation_candidates);
									
									// if the variable is not on the current cluster increment the skip value of the corresponding separator
									if (x_sim->Get_Num() != x->Get_Num())
									{
										bool found = false;
										for (deque<Child_Node *>::iterator iter2 = ct->Begin_Neighbor(current_cluster_num); iter2 != ct->End_Neighbor (current_cluster_num) && !found; iter2++)
										{	
											if ((*iter2)->Get_Cluster_Num() != current_cluster->Get_Parent_Num())		// we consider only the separator with a cluster different from the parent cluster
											{
												if ((*iter2)->Get_Cluster()->Is_Element(x_sim->Get_Num()))
												{
													found = true;	
													merge_policy->Increment_Skip_Number((*iter2)->Get_Cluster_Num());	
												}	
											}						
										}
									}
								}	
								
								depth++;
								is_negative_node[depth] = false;
								variable_node[depth] = x;
								value_node[depth] = val;
								
								A.Assign(var,val);
								unassigned_variable_number [current_cluster_num]--;
								Solving_Node_Counter++;
                Solving_Decision_Counter++;

								ds->New_Level();
								
								// we update the number of assigned variables in each constraint scope
								for (list<Edge *>::iterator iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
									pb->Get_Constraint((*iter)->Get_Num())->Dec_Unassigned_Variable_Number ();
										
								// we remove all the following values of var
								d = x->Get_Domain();
                
                ds->Add_Element (x);
                d->Assign (val);
							}
							else
								{
									BJ = false;
									
									while ((result == -1) && (depth >= 0) && ((is_negative_node[depth]) || BJ))
									{						
										if (is_negative_node[depth])
										{
											// we restore the removed values
											ds->Restore(pb);
											depth--;
										}
										else
											{
												var = A.Last_Assigned ();
												
												if (last_nogood_separator->Is_Element (var))
												{
													BJ = false;
												}
												else
													{
														// we restore the removed values
														ds->Restore(pb);

														A.Unassign ();
														unassigned_variable_number [current_cluster_num]++;

														// we update the number of assigned variables in each constraint scope
														for (list<Edge *>::iterator iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
															pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();

														// we reset some goods if necessary
														max_depth = A.Get_Size() - initial_size;
														for (int i = 0; i < nb_reset_good[max_depth]; i++)
															is_good[reset_good[max_depth][i]] = false;
														nb_reset_good[max_depth] = 0;

														depth--;
													}
											}

										if (depth == cluster_begin_depth[current_cluster_num])
										{
											// we have unassigned all the variables of the current cluster because no solution have been found
											if (current_cluster_num == root_cluster_num)
											{
												// there is no solution 		
												result = 0;
											}
											else
												{
													if (!BJ)
													{
														// we record a new nogood
														last_nogood_separator = current_cluster->Get_Separator();
														
														int nb_positive_node = 0;
														for (vector<vertex>::iterator iter3 = last_nogood_separator->begin(); iter3 != last_nogood_separator->end(); iter3++)
														{
															nogood[nb_positive_node].first = *iter3;
															nogood[nb_positive_node].second = A.Get_Value (*iter3);
															nb_positive_node++;
														}

														if (nb_positive_node == 1)
														{
															d = pb->Get_Variable (nogood[0].first)->Get_Domain();
															
															empty_domain[empty_domain_nb] = d;
															empty_domain_nb++;
                              
                              pb->Get_Unary_Nogood_Base ()->Add_Nogood(pb,nogood[0].first,nogood[0].second);
														}
                            else
                              {
                                if (nogood_base == 0)
                                {
                                  nogood_base = new Nogood_Base_Global_Constraint();
                                  pb->Add_Constraint (nogood_base,false);
                                  variable_heuristic->Add_Constraint (nogood_base);
                                }
                                nogood_base->Add_Nogood(pb,nogood,nb_positive_node,A);
                              }
													}

													BJ = true;
														
													// we go back to the parent cluster
													current_cluster_num = current_cluster->Get_Parent_Num ();
													current_cluster = current_cluster->Get_Parent();
												}
										}
									}
						
									if (result == -1)
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
                    											
										if (empty_domain_nb > 0)
										{
											for (int i = empty_domain_nb-1; i >= 0 ; i--)
											{
												if (empty_domain[i]->Get_Size() > 0)
												{
													empty_domain_nb--;
													empty_domain[i] = empty_domain[empty_domain_nb];
												}
											}
										}

										// we unassign the last assigned variable and add it as candidate for the variable heuristic
										A.Unassign ();
										
										unassigned_variable_number [current_cluster_num]++;

										// we update the number of assigned variables in each constraint scope
										for (list<Edge *>::iterator iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
											pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();
											
										// we reset some goods if necessary
										max_depth = A.Get_Size() - initial_size;
										for (int i = 0; i < nb_reset_good[max_depth]; i++)
											is_good[reset_good[max_depth][i]] = false;
										nb_reset_good[max_depth] = 0;

										// the current node becomes a negative node
										is_negative_node[depth] = true;					
										
                    Solving_Decision_Counter++;
										Solving_Backtrack_Counter++;
											
										if ((merge_policy != 0) && merge_policy->Verify (current_cluster_num))
										{
											pair<int,int> merge_two_clusters;
											
											merge_two_clusters = merge_policy->Select_Clusters_To_Merge (pb,ct, current_cluster_num);
												
											if (merge_two_clusters.first != -1)
											{
				
												if ((d->Get_Size() == 0) && (A.Get_Size() == initial_size))		// particular case where a domain is empty and the current assignment contains no positive decision
												{
													// there is no solution
													result = 0;
												}
												else
												{
                          pb->Get_Event_Manager()->New_Level();
                          
													is_merged = true;
													
													/* TODO */ // unassign the instanciated variables
													// we unassign all the assigned variables of the current cluster (and maybe the parent if the current is merged with its parent
													
													int undo;
													if (merge_two_clusters.second == current_cluster_num)
															undo = 1;
													else undo = 2;
														
													int j;		
													while (undo > 0)
													{
														j = depth;
														do
														{
															// we restore the removed values
															ds->Restore(pb);
															
															if (! is_negative_node[j])
															{
																A.Unassign ();

																var  = variable_node[j]->Get_Num();

																// we restore the variable heuristic
																unassigned_variable_number [current_cluster_num]++;

																// we update the number of assigned variables in each constraint scope
																for (list<Edge *>::iterator iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
																	pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();
																
																// we update the structure is_good	
																max_depth = A.Get_Size() - initial_size;
															
																for (int i = 0; i < nb_reset_good[max_depth]; i++)
																	is_good[reset_good[max_depth][i]] = false;
																
																nb_reset_good[max_depth] = 0;
															}
															
															j--;
														}
														while (j != cluster_begin_depth[current_cluster_num]);

														
														// we record some nogoods
														int nb_positive_node = 0;
														
														// we add the variables of the separator with the parent cluster (these variables are positively assigned by construction)
														sep = current_cluster->Get_Separator();
														if (sep != 0)
														{
															for (vector<vertex>::iterator iter3 = sep->begin(); iter3 != sep->end(); iter3++)
															{
																nogood[nb_positive_node].first = *iter3;
																nogood[nb_positive_node].second = A.Get_Value (*iter3);
																nb_positive_node++;
															}
														}

														// we now consider the proper variables of the current cluster and build the corresponding nogoods
														for (int i = j+1; i <= depth; i++)
														{
															if (is_negative_node[i])	
															{
																if (nb_positive_node == 0)
																{
																	d = variable_node[i]->Get_Domain();
																	if (d->Is_Present(value_node[i]))
																	{
																		d->Filter_Value (value_node[i]);
																		
																		if (d->Get_Size() == 0)
																		{
																			empty_domain[empty_domain_nb] = d;
																			empty_domain_nb++;
																		}
																	}
                                  
                                  pb->Get_Unary_Nogood_Base()->Add_Nogood(pb,variable_node[i]->Get_Num(),value_node[i]);
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
                                    nogood_base->Add_Nogood(pb,nogood,nb_positive_node+1,A);
                                  }
															}
															else
																{
																	nogood[nb_positive_node].first = variable_node[i]->Get_Num();
																	nogood[nb_positive_node].second = value_node[i];
																	nb_positive_node++;
																}
														}
														
														// we backtrack to the parent cluster
														depth = cluster_begin_depth[current_cluster_num];

														if (current_cluster_num != root_cluster_num)
														{
															current_cluster_num = current_cluster->Get_Parent_Num ();
															current_cluster = current_cluster->Get_Parent();
														}
														
														undo --;
									
													}
													
													// we check if some domains are empty
													for (int i = empty_domain_nb-1; i >= 0 ; i--)
													{
														if (empty_domain[i]->Get_Size() > 0)
														{
															empty_domain_nb--;
															empty_domain[i] = empty_domain[empty_domain_nb];
														}
													}
                          
                          if (empty_domain_nb == 0)
                          {
                            No_Deletion_Stack nds;

                            variable_heuristic->Update_Before_Filtering(A);
                            
                            is_consistent = ac->Achieve (pb,A,&nds,var);
                            
                            variable_heuristic->Update_After_Filtering(A);
                          }
                          
                          if ((empty_domain_nb > 0) || (! is_consistent))
                          {
                            // there is no solution 	
                            result = 0;
                          }
                          else
                            {
                              pb->Get_Event_Manager()->New_Level();

                              // do real merging
                              merge_policy->Merge_Clusters (ct, merge_two_clusters.first);
                              
                              // we update the data structure used to manage the tree-decomposition
                              candidates[merge_two_clusters.second].clear();
                              
                              // initialization of the variable heuristic
                              Cluster_Node * cluster = ct->Get_Cluster(merge_two_clusters.second);
                              for (vector<vertex>::iterator iter_var = cluster->begin(); iter_var != cluster->end(); iter_var++)
                                candidates[merge_two_clusters.second].push_back (pb->Get_Variable(*iter_var));

                              if (merge_two_clusters.second == root_cluster_num)
                                unassigned_variable_number [merge_two_clusters.second] = cluster->Size();
                              else unassigned_variable_number [merge_two_clusters.second] = cluster->Size() - cluster->Get_Separator()->Size();

                              // we update the merged_clusters data structure for the original tree-decomposition
                              for (vector<Cluster_Node *>::iterator iter_cn = merged_clusters[merge_two_clusters.first].begin(); iter_cn != merged_clusters[merge_two_clusters.first].end(); iter_cn++)
                                merged_clusters[merge_two_clusters.second].push_back (*iter_cn);		
                                
                              // comme si on est revenu au début
                              // avant je faisais ca plutot, si juste le cluster courant ok mais les fils pas encore mis à jour
                            
                              if (depth == -1)
                                merge_policy->Reset(current_cluster_num);
                            }
												}
											}
										}
									}
								}
						}

            if ((restart_policy != 0) && (restart_policy->Restart()))
            {
              if ((d->Get_Size() == 0) && (A.Get_Size() == initial_size))		// particular case where a domain is empty and the current assignment contains no positive decision
              {
                // there is no solution
                result = 0;
              }
              
              has_not_performed_restart = false;
            }


						// we check the consistency of the current assignment
						// N.B : if we did a backtrack to last cluster or to the last cluster (due to verif and merge) we are sure that this is consistent ? no we aren't but most probably yes

						if ((has_not_performed_restart) && (result == -1) && (!is_merged))
            {
              if ((d->Get_Size() > 0) && (empty_domain_nb == 0))
              {
                variable_heuristic->Update_Before_Filtering(A,var);
                
                is_consistent = ac->Achieve (pb,A,ds,var);
                
                variable_heuristic->Update_After_Filtering(A,var);
              }
              else
                {
                  is_consistent = false;
                  pb->Get_Event_Manager()->New_Level();
                }
						}
            
						if (is_merged)
						{
							is_merged = false;
							if (depth == -1)
							{	
								if (empty_domain_nb > 0) // comme si je fais un restart
									result = 0;
								else
								{	
									is_consistent = true;
									var = -1;
									val = -1;
									depth = -1;
								}
							}
							else
								is_consistent = (empty_domain_nb == 0); // le coup de AC fait avant reste valide
																		// il manque un peu de filtrage (si nogood unaire) mais on va les rattraper
						}												// sauf le cas où le domaine est vide et ca on va pouvoir le rattraper
					}
					
					if ((result == -1) && (Solving_Timer.Get_Duration () < time_limit))
					{
						if (! has_not_performed_restart)
						{
              pb->Get_Event_Manager()->New_Level();

							int j;
							// we perform a restart
							Solving_Restart_Number_Counter++;
              
              variable_heuristic->Update_When_Restarting ();
										
							// we unassign all the assigned variables
							while (depth >= 0)
							{
								j = depth;
								do
								{
									// we restore the removed values
									ds->Restore(pb);
									
									if (! is_negative_node[j])
									{
										A.Unassign ();

										var  = variable_node[j]->Get_Num();

										// we restore the variable heuristic
										unassigned_variable_number [current_cluster_num]++;			// utile ?

										// we update the number of assigned variables in each constraint scope
										for (list<Edge *>::iterator iter = h->Begin_Edge_List(var); iter != h->End_Edge_List (var); iter++)
											pb->Get_Constraint((*iter)->Get_Num())->Inc_Unassigned_Variable_Number ();
											
										nb_reset_good[A.Get_Size() - initial_size] = 0;
									}
									j--;
								}
								while (j != cluster_begin_depth[current_cluster_num]);

								
								// we record some nogoods
								int nb_positive_node = 0;
								
								// we add the variables of the separator with the parent cluster (these variables are positively assigned by construction)
								sep = current_cluster->Get_Separator();
								if (sep != 0)
								{
									for (vector<vertex>::iterator iter3 = sep->begin(); iter3 != sep->end(); iter3++)
									{
										nogood[nb_positive_node].first = *iter3;
										nogood[nb_positive_node].second = A.Get_Value (*iter3);
										nb_positive_node++;
									}
								}

								// we now consider the proper variables of the current cluster and build the corresponding nogoods
								for (int i = j+1; i <= depth; i++)
								{
									if (is_negative_node[i])	
									{
										if (nb_positive_node == 0)
										{
											d = variable_node[i]->Get_Domain();
											if (d->Is_Present(value_node[i]))
											{
												d->Filter_Value (value_node[i]);
												
												if (d->Get_Size() == 0)
												{
													empty_domain[empty_domain_nb] = d;
													empty_domain_nb++;
												}
											}
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
								
								// we backtrack to the parent cluster
								depth = cluster_begin_depth[current_cluster_num];
								if (current_cluster_num != root_cluster_num)
								{
									current_cluster_num = current_cluster->Get_Parent_Num ();
									current_cluster = current_cluster->Get_Parent();
								}
							}
							
							// we check if some domains are empty
							for (int i = empty_domain_nb-1; i >= 0 ; i--)
							{
								if (empty_domain[i]->Get_Size() > 0)
								{
									empty_domain_nb--;
									empty_domain[i] = empty_domain[empty_domain_nb];
								}
							}
							
              if (empty_domain_nb == 0)
              {
                No_Deletion_Stack nds;
                
                variable_heuristic->Update_Before_Filtering(A,var);
                
                is_consistent = ac->Achieve (pb,A,&nds,var);
                
                variable_heuristic->Update_After_Filtering(A,var);
              }
              
							if ((empty_domain_nb > 0) || (! is_consistent))
							{
								// there is no solution 	
								result = 0;
							}
							else
								{
                  pb->Get_Event_Manager()->New_Level();
              
									// we reset some variables
									is_consistent = true;
									has_not_performed_restart = true;
																	
									// we choose a new root cluster
									new_root_cluster_num = restart_root_heuristic->Choose_Root (ct,pb);

									if (new_root_cluster_num != root_cluster_num)
									{
										// we save some informations on the previous tree-decomposition
										current_cluster = ct->Get_Root();
										current_cluster_num = root_cluster_num;
										
										// we modify the root cluster
										ct->Set_Root (new_root_cluster_num);	
										root_cluster_num = new_root_cluster_num;
										
										original_ct->Set_Root (root_cluster_num);
												
										// we update the data structure related to the tree-decomposition and the (no)good management							
										do
										{			
											parent_cluster_num = current_cluster->Get_Parent_Num ();
											
											unassigned_variable_number [current_cluster_num] = current_cluster->Size() - current_cluster->Get_Separator()->Size();
											
											// we keep on with the parent cluster
											current_cluster_num = parent_cluster_num;
											current_cluster = current_cluster->Get_Parent();
										}
										while (current_cluster_num != root_cluster_num);
										
										unassigned_variable_number [root_cluster_num] = ct->Get_Cluster(root_cluster_num)->Size();
									}
									
									// option : re-init the skips to 0
                  if (merge_policy != 0)
                    merge_policy->Reset_Skip_Number ();					
								}
						}
					}
				}	
			}
	}

	delete [] merged_clusters;
	
	switch (result)
	{
		case -1: state = STOPPED; break;
		case  0: state = HAS_PROVED_INCONSISTENCY; break;
		case  1: state = HAS_PROVED_CONSISTENCY; break;
	}
	
	return state;
}


Assignment & Solver::Get_Solution ()
// returns the solution if the CSP is consistent or an empty assignment if the CSP is inconsistent or has not been solved yet
{
	if (state == HAS_PROVED_CONSISTENCY)
	{
		if (A.Get_Size() < pb->Get_N())
		{
			stack<Cluster_Node *> cluster_stack;
			for (unsigned int num_ct = 0; num_ct < original_td_merge->Get_Cluster_Tree_Number(); num_ct++)
			{
				// we consider the current cluster tree of the tree-decomposition original_td_merge
				Cluster_Tree * ct = original_td_merge->Get_Cluster_Tree(num_ct);
				
				// we have to complete the assignment A by considering the recorded goods
				cluster_stack.push (ct->Get_Root());
				Cluster_Node * cluster;
				int cluster_num;
				deque<Child_Node *>::iterator iter2;
				
				do
				{
					cluster = cluster_stack.top();
					cluster_num = cluster->Get_Num();
					cluster_stack.pop();

					for (iter2 = ct->Begin_Neighbor(cluster_num); iter2 != ct->End_Neighbor (cluster_num); iter2++)
						if ((*iter2)->Get_Cluster_Num() != 	cluster->Get_Parent_Num())
						{
							dynamic_cast<CSP_Child_Node *>((*iter2)->Get_Cluster()->Get_Separator())->Add_Good_To_Assignment (A);
							cluster_stack.push ((*iter2)->Get_Cluster());
						}
				}
				while (! cluster_stack.empty ());
				
			}
		}
		
		return A;
	}
	else throw ("No solution available");
}

