#include "tree_decomposition.h"
#include "hypergraph.h"
#include <iostream>
#include <queue>
#include <stack>
#include "graph_auxiliary_functions.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Tree_Decomposition::Tree_Decomposition (Graphical_Structure * gs)
// constructs an empty tree-decomposition related to the graphical structure gs
{
	g = gs;
	ct_number = 0;
}


Tree_Decomposition::Tree_Decomposition (Tree_Decomposition & td)
// constructs a tree-decomposition by copying the tree-decomposition td
{
	g = td.g;

	cluster_tree.resize(td.ct_number);
	ct_number = td.ct_number;
	
	for (unsigned int i = 0; i < ct_number; i++)
		cluster_tree[i] = new Cluster_Tree (*(td.cluster_tree[i]));
}


Tree_Decomposition::~Tree_Decomposition ()
// destructor
{
	for (unsigned int i = 0; i < ct_number; i++)
		delete cluster_tree[i];
}
		

//----------
// operator
//----------


Tree_Decomposition & Tree_Decomposition::operator= (Tree_Decomposition & td)
// operator =
{
	if (this != &td)
	{
		// we free the allocated memory
		for (unsigned int i = 0; i < ct_number; i++)
			delete cluster_tree[i];
			
		// we copy the data
		g = td.g;
		
    cluster_tree.resize(td.ct_number);
		ct_number = td.ct_number;
		
		for (unsigned int i = 0; i < ct_number; i++)
			cluster_tree[i] = new Cluster_Tree (*(td.cluster_tree[i]));
	}
	
	return *this;
}


//-----------------
// basic functions 
//-----------------


int Tree_Decomposition::Get_W ()
// returns the width of the tree-decomposition
{
	int w = -1;
	
	for (unsigned int i = 0; i < ct_number; i++)
	{
		for (map<int,Cluster_Node *>::iterator iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
			if (w < iter->second->Size()-1)
				w = iter->second->Size()-1;
	}

	return w;
}


int Tree_Decomposition::Get_HW ()
// returns the hyperwidth of tree-decomposition
{
	int hw = 0;
	
	for (unsigned int i = 0; i < ct_number; i++)
	{
		for (map<int,Cluster_Node *>::iterator iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
			if (hw < iter->second->Get_Nb_Edges())
				hw = iter->second->Get_Nb_Edges();
	}
	
	return hw;
}


int Tree_Decomposition::Get_H ()
// returns the height of the tree-decomposition
{
	int h = -1;

	for (unsigned int i = 0; i < ct_number; i++)
	{
		for (map<int,Cluster_Node *>::iterator iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
			if (h < iter->second->Get_Depth())
				h = iter->second->Get_Depth();
	}

	return h;
}


int Tree_Decomposition::Get_S ()
// returns the size of the largest separator
{
	int s = 0;

	for (unsigned int i = 0; i < ct_number; i++)
	{
		for (map<int,Cluster_Node *>::iterator iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
			if ((iter->second->Get_Separator() != 0) && (s < iter->second->Get_Separator()->Size()))
				s = iter->second->Get_Separator()->Size();
	}

	return s;
}


int Tree_Decomposition::Nb_Clusters ()
// returns the number of clusters of the cluster-tree
{
	int sum = 0;
	
	for (unsigned int i = 0; i < ct_number; i++)
		sum += cluster_tree[i]->Nb_Clusters ();
	
	return sum;
}


int Tree_Decomposition::Get_Max_Label ()
// returns the largest label of clusters in a cluster-tree
{
	int max = 0;
	
	for (unsigned int i = 0; i < ct_number; i++)
		if (max < cluster_tree[i]->Get_Max_Label ())
			max = cluster_tree[i]->Get_Max_Label ();
			
	return max;
}


int Tree_Decomposition::Get_Max_Vertices_Number ()
// returns the largest number of vertices in a cluster-tree
{
	int max = 0;
	
	for (unsigned int i = 0; i < ct_number; i++)
	{ 
		int nb = cluster_tree[i]->Get_Nb_Vertices ();
		if (max < nb)
			max = nb;
	}	
	return max;
}


int Tree_Decomposition::Get_Diameter ()
// returns the diameter of the tree-decomposition
{
	int diameter = -1;

	for (unsigned int i = 0; i < ct_number; i++)
	{
		int degree;
		map<int,Cluster_Node *>::iterator iter, iter2;
		deque<Child_Node *>::iterator iter3;
				
		for (iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
		{		
			// we check whether the degree 
			
			degree = 0;
			for (iter3 = cluster_tree[i]->Begin_Neighbor(iter->first); (iter3 != cluster_tree[i]->End_Neighbor(iter->first)) && (degree < 2); iter3++)
				degree++;
			
			if (degree == 1)	// the current cluster is a leaf
			{
				int nb_max_clusters = cluster_tree[i]->Get_Max_Label()+1;  
				vector<bool> is_unmarked (nb_max_clusters,true);
				vector<int> distance (nb_max_clusters,-1);

				is_unmarked[iter->first] = false;
		
				queue<int> queue_node;
				int num,num2;
				
				queue_node.push (iter->first);
				distance[iter->first] = 0;
				
				do
				{
					num = queue_node.front();
					queue_node.pop();
					for (iter3 = cluster_tree[i]->Begin_Neighbor(num); iter3 != cluster_tree[i]->End_Neighbor(num); iter3++)
					{
						num2 = (*iter3)->Get_Cluster_Num();
						if (distance[num2] == -1)
						{
							is_unmarked[num2] = false;
							
							distance[num2] = distance[num] + 1;
							if (distance[num2] > diameter)
								diameter = distance[num2];
							queue_node.push (num2);
						}
					}
				}
				while (! queue_node.empty());
			}
		}		
	}
	
	return diameter;
}


bool Tree_Decomposition::Is_Valid ()
// returns true if the current tree-decomposition is valid, false otherwise
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
		if (dynamic_cast<Light_Graph *>(g) != 0)
		{
			unsigned int nb_edges = 0;
			
			for (unsigned int i = 0; (i < ct_number) && (ok); i++)
				for (iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
				{
					Child_Node * sep = iter->second->Get_Separator();

					for (vector<vertex>::iterator iter_v = iter->second->begin(); iter_v != iter->second->end(); iter_v++)
						if ((sep == 0) || (! sep->Is_Element (*iter_v)))
						{
							for (Light_Graph::Neighbor_iterator iter_n = dynamic_cast<Light_Graph *>(g)->Begin_Neighbor (*iter_v); iter_n != dynamic_cast<Light_Graph *>(g)->End_Neighbor (*iter_v); iter_n++)
								if ((iter->second->Is_Element(*iter_n)) && (((sep != 0) && (sep->Is_Element (*iter_n))) || (*iter_v < *iter_n)))
									nb_edges++;
						}
				}
			ok = nb_edges == g->M();
		}
		else
			if (dynamic_cast<Multi_Hypergraph *>(g) != 0)
			{
				ok = true;
				for (vertex x = 0;  (x < g->N()) && (ok); x++)
					for (list<Edge *>::iterator iter_edge = dynamic_cast<Multi_Hypergraph *>(g)->Begin_Edge_List(x); (iter_edge != dynamic_cast<Multi_Hypergraph *>(g)->End_Edge_List(x)) && (ok); iter_edge++)
					if ((*iter_edge)->Get_Element (0) == x)
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
	}
	else ok = false;
	
	// we check the structure
	
	for (unsigned int i = 0; (i < ct_number) && (ok); i++)
	{
		// we first check whether the cluster-tree is acyclic
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
			
			// we check whether the part of the cluster-tree containing a given vertex forms a subtree (running intersection property)
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


bool Tree_Decomposition::Is_Connected_Tree_Decomposition ()
// returns true if all the clusters have a single connected component, false otherwise
{
	for (unsigned int i = 0; i < ct_number; i++)
	{
		for (map<int,Cluster_Node *>::iterator iter = cluster_tree[i]->Begin_List(); iter != cluster_tree[i]->End_List(); iter++)
			if (iter->second->Get_Nb_Connected_Components() > 1)
				return false;
	}
	return true;
}


//-----------------------------------
// function computing cluster-trees
//-----------------------------------


void Tree_Decomposition::Make_Tree (pair<int,list<Cluster_Node *>> & lc)
// makes a tree from the cluster list lc
{  
	// we count the number of clusters in the list lc
	int nb_clusters = lc.first;

  // we compute the cluster-tree by connecting, at each step, a new cluster from lc to a cluster chosen previously with which it shares the largest intersection (we use the Prim's algorithm)
  	
  // initialization of keys
  int * key;
  key = new int [nb_clusters];
  Cluster_Node ** parent;
  parent = new Cluster_Node * [nb_clusters];
  int * index ;
  index = new int [nb_clusters];
  Cluster_Node ** clusters;
  clusters = new Cluster_Node * [nb_clusters];
  
	list<Cluster_Node *>::iterator iter_cl = lc.second.begin();
  for (int i = 0; i < nb_clusters; i++)
  {
    key[i] = 0;
    parent[i] = 0;
    index[i] = i;
    clusters[i] = *iter_cl;
    iter_cl++;
  }
  
	// we apply the Prim's Algorithm
	int ind_max = -1;
	int max;
	list<pair<vertex,int> >::iterator neighbor_iter;
	vector<vertex>::iterator iter_v;
	int inter, j, choice;
	  
  while (nb_clusters > 0)
  {
    max = -1;
    for (j = 0; j < nb_clusters; j++)
      if (key[index[j]] > max)
      {
        max = key[index[j]];
        ind_max = j;
      }
      else
				if ((key[index[j]] == max) && (index[j] < index[ind_max]))
					ind_max = j;

		choice = index[ind_max];
		
		// we update the cluster-tree
		if (max == 0)
		{
			// we create a new cluster-tree
			ct_number++;
			if ((unsigned) ct_number > cluster_tree.size ())
				cluster_tree.resize(cluster_tree.size()+50);

			cluster_tree[ct_number-1] = new Cluster_Tree(g);
				
			// we add the selected cluster to the current cluster-tree
			cluster_tree[ct_number-1]->Add_Cluster (clusters[choice]);
		}
		else
			{
				// we add the selected cluster to the current cluster-tree
				cluster_tree[ct_number-1]->Add_Cluster (clusters[choice]);
				
				// we link the selected cluster with its parent cluster
				cluster_tree[ct_number-1]->Add_Child_Node (parent[choice],clusters[choice]);
			}
				
		// we update the index data structure
		nb_clusters--;
    index[ind_max] = index[nb_clusters];
     
    // we update the key values
    for (j = 0; j < nb_clusters; j++)
    {
			// we compute the intersection between the last selected cluster and the unchosen clusters
      
      inter = 0;
      for (iter_v = clusters[choice]->begin(); iter_v != clusters[choice]->end(); iter_v++)
        if (clusters[index[j]]->Is_Element (*iter_v))
          inter++;

      if (inter > key[index[j]])
      {
        key[index[j]] = inter;
        parent[index[j]] = clusters[choice];
      }
    }
  }

  delete [] key;
  delete [] parent;
  delete [] index;
  delete [] clusters;
}



//-----------------------------------
// functions modifying cluster-trees
//-----------------------------------


void Tree_Decomposition::Merge_Sep (int max_allowed_sep_size)
// merges each cluster with its parent if the size of the intersection is greater than max_allowed_sep_size
{
	for (unsigned int i = 0; i < ct_number; i++)
		cluster_tree[i]->Merge_Sep (max_allowed_sep_size);
}


void Tree_Decomposition::Merge_VP (int min_allowed_vp, bool until_fix_point)
// merges each cluster with its parent if the number of proper vertices is not greater than min_allowed_vp (if until_fix_point is set to true, the process is repeat by choosing each remaining cluster as root until no cluster may be merged)
{
	if (min_allowed_vp > 0)
	{
		for (unsigned int i = 0; i < ct_number; i++)
			if (until_fix_point)
			{
				int nb;
				do
				{
					nb = cluster_tree[i]->Nb_Clusters();
					for (int j = 0; j < cluster_tree[i]->Get_Max_Label(); j++)
						if (cluster_tree[i]->Get_Cluster(j) != 0)
						{
							cluster_tree[i]->Set_Root (j);
							cluster_tree[i]->Merge_VP (min_allowed_vp);
						}
				}
				while (nb != cluster_tree[i]->Nb_Clusters());
			}
			else cluster_tree[i]->Merge_VP (min_allowed_vp);

	}
}
