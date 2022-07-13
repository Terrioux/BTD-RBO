#include "csp_tree_decomposition.h"
#include "csp_cluster_tree.h"
#include "nogood_base_global_constraint.h"
#include <graph_auxiliary_functions.h>
#include <stack>


//--------------
// constructors
//--------------


CSP_Tree_Decomposition::CSP_Tree_Decomposition (Tree_Decomposition * td, CSP * pb, bool with_nogood_set): Tree_Decomposition (pb->Get_Structure())
// constructs a CSP tree-decomposition from a tree-decomposition
{
	ct_number = td->Get_Cluster_Tree_Number();
	cluster_tree.resize(ct_number);
		
	for (unsigned int i = 0; i < ct_number; i++)
		cluster_tree[i] = new CSP_Cluster_Tree (td->Get_Cluster_Tree(i),pb,with_nogood_set);
}



//----------------
// basic function
//----------------


bool CSP_Tree_Decomposition::Is_Valid (CSP * pb)
//returns true if the current tree-decomposition is valid, false otherwise (nogood base constraints of pb are ignored)
{
	bool ok = true;
	map<int,Cluster_Node *>::iterator iter;
	
	int * is_present;
	is_present = new int [g->N()];
	for (vertex v = 0; v < g->N(); v++)
		is_present [v] = -1;
	unsigned int nb_vertices = 0;
		
	// we check the presence of all the vertices
	
	for (unsigned int i = 0; (i < ct_number) && (ok); i++)
	{
		for (iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
		{
			for (vector<vertex>::iterator iter_v = iter->second->begin(); iter_v != iter->second->end(); iter_v++)
				if (is_present[*iter_v] == -1)
				{
					is_present[*iter_v] = i;
					nb_vertices++;
				}
		}
	}
	
	
	// we check the presence of all the edges
	if (nb_vertices == g->N())
	{
    ok = true;
    for (vertex x = 0;  (x < g->N()) && (ok); x++)
      for (list<Edge *>::iterator iter_edge = dynamic_cast<Multi_Hypergraph *>(g)->Begin_Edge_List(x); (iter_edge != dynamic_cast<Multi_Hypergraph *>(g)->End_Edge_List(x)) && (ok); iter_edge++)
      if (((*iter_edge)->Get_Element (0) == x) && (dynamic_cast<Nogood_Base_Global_Constraint*>(pb->Get_Constraint((*iter_edge)->Get_Num())) == 0))
      {
        bool not_found = true;
        for (iter = cluster_tree[is_present[x]]->Begin_List(); (iter != cluster_tree[is_present[x]]->End_List()) && (not_found); iter++)
        {
          set< vertex >::iterator iter_v = (*iter_edge)->Begin();
          while ((iter_v != (*iter_edge)->End()) && (iter->second->Is_Element(*iter_v)))
            iter_v++;
          
          not_found = iter_v != (*iter_edge)->End();
        }
        ok = ! not_found;
      }
	}
	else ok = false;
	
	// we check the structure
	
	for (unsigned int i = 0; (i < ct_number) && (ok); i++)
	{
		// we first check whether the cluster tree is acyclic
		Light_Graph A;
		int parent;
		
		A.Init(cluster_tree[i]->Get_Max_Label()+1);
		for (iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
		{
			parent = iter->second->Get_Parent_Num();
			if (parent != -1)
				A.Add_Edge (iter->first,parent);
		}
		
		ok = Is_Acyclic(&A);
		
		// we check whether each intersection is present in both direction
		if (ok)
		{
			deque<Child_Node *>::iterator iter2, iter3;
			int num2;
			for (iter = cluster_tree[i]->Begin_List(); (iter != cluster_tree[i]->End_List()) && ok; iter++)
			{
				for (iter2 = cluster_tree[i]->Begin_Neighbor (iter->first); iter2 != cluster_tree[i]->End_Neighbor (iter->first); iter2++)
				{
					num2 = (*iter2)->Get_Cluster_Num();
					iter3 = cluster_tree[i]->Begin_Neighbor (num2);
					while ((iter3 != cluster_tree[i]->End_Neighbor (num2)) && ((*iter3)->Get_Cluster_Num() != iter->first))
						iter3++;
					ok = iter3 != cluster_tree[i]->End_Neighbor (num2);
				}
			}
		
			// we check whether the vertices of the intersection with the parent cluster are in the first positions in the vector elements
			if (ok)
			{
				int sep_size, j;
				Child_Node * sep;
				vector<vertex>::iterator iter4;
				
				for (iter = cluster_tree[i]->Begin_List(); (iter != cluster_tree[i]->End_List()) && ok; iter++)
				{
					sep = iter->second->Get_Separator();
					if (sep != 0)
					{
						sep_size = sep->Size();
						iter4 = sep->begin();
						j = 0;
						while ((iter4 != sep->end()) && (sep->Is_Element(*iter4)))
						{
							iter4++;
							j++;
						}
						
						ok = j==sep_size;
					}
				}
			}
			
			// we check whether the part of the cluster tree containing a given vertex forms a subtree (running intersection property)
			bool * visited;
			visited = new bool [cluster_tree[i]->Get_Max_Label ()+1];
			
			for (vertex x = 0; (x < g->N()) && (ok); x++)
				if (is_present[x] == (signed) i)
				{
					int nb = 0;
					int first = -1;
					for (iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
					{
						if ((*iter).second->Is_Element (x))
						{
							if (nb == 0)
								first = (*iter).first;
							nb++;
						}
						visited[(*iter).first] = false;
					}
					
					stack<int> S;
					S.push (first);
					visited [first] = true;
					int nb_visited = 1;
					
					while (! S.empty())
					{
						int num;
						num = S.top();
						S.pop();
						
						for (deque< Child_Node * >::iterator iter_n = cluster_tree[i]->Begin_Neighbor (num); iter_n != cluster_tree[i]->End_Neighbor (num); iter_n++)
							if ((! visited[(*iter_n)->Get_Cluster_Num()]) && ((*iter_n)->Get_Cluster()->Is_Element (x)))
							{
								S.push ((*iter_n)->Get_Cluster_Num());
								visited [(*iter_n)->Get_Cluster_Num()] = true;
								nb_visited++;
							}
					}
					
					ok = nb == nb_visited;
				}
			delete [] visited;
		}
	}
	
	delete [] is_present;
	
	return ok;
}


void CSP_Tree_Decomposition::Select_First_Cluster_Tree (int num)
// selects as a first cluster tree one which contains the variables whose number is num
{
  bool found;
  int num_ct = 0;
  
  do
  {
    map<int,Cluster_Node *>::iterator iter = cluster_tree[num_ct]->Begin_List(); 
    while ((iter != cluster_tree[num_ct]->End_List()) && (! iter->second->Is_Element(num)))
      iter++;
    found = iter != cluster_tree[num_ct]->End_List();
    
    num_ct++;
  }
  while (! found);
  
  num_ct--;
  
  Cluster_Tree * aux = cluster_tree[num_ct];
  cluster_tree[num_ct] = cluster_tree[0];
  cluster_tree[0] = aux;
}
