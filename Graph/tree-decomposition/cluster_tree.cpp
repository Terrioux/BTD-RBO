#include "cluster_tree.h"
#include <iostream>
#include <queue>
#include <stack>
#include <typeinfo>
#include <cassert>

//-----------------------------
// constructors and destructor
//-----------------------------

Cluster_Tree::Cluster_Tree (Graphical_Structure * gs)
// constructs an empty cluster-tree
{
	nb_clusters = 0;
	root = 0;
	root_num = -1;
	max_label = -1;
	g = gs;
	nb_vertices = 0;
}


Cluster_Tree::Cluster_Tree (Cluster_Tree & ct)
// constructs a cluster-tree by copying the cluster-tree ct
{
	root = ct.root;
	root_num = ct.root_num;
	nb_clusters = ct.nb_clusters;
	max_label = ct.max_label;
	g = ct.g;
	nb_vertices = ct.nb_vertices;
	
	map<int,Cluster_Node *>::iterator iter;
	Cluster_Node * cl;
	
	// copy of the cluster_list
	for (iter = ct.cluster_list.begin(); iter != ct.cluster_list.end(); iter++)
	{
		cl = new Cluster_Node (*(iter->second));
		cluster_list.insert(pair<int,Cluster_Node *>(iter->first,cl));	
		adjacency_list.insert (pair<int,deque<Child_Node *> >(iter->first,deque<Child_Node *>()));
	}
	
	// copy of the adjacency_list
	map<int,deque<Child_Node *> >::iterator iter2;
	deque<Child_Node *>::iterator iter3;
	Child_Node * intersection;
	  	
	
	for (iter2 = ct.adjacency_list.begin(); iter2 != ct.adjacency_list.end(); iter2++)
	{
		for (iter3 = iter2->second.begin(); iter3 != iter2->second.end(); iter3++)
		{
			intersection = new Child_Node (**iter3);
			intersection->Set_Cluster (cluster_list.find(intersection->Get_Cluster_Num())->second);
			adjacency_list.find(iter2->first)->second.push_back (intersection);
			
			cl = cluster_list.find(iter2->first)->second;
			if (intersection->Get_Cluster_Num() == cl->Get_Parent_Num())
				cl->Set_Separator (intersection);
		}
	}	
}


Cluster_Tree::~Cluster_Tree ()
// destructor
{
	map<int,Cluster_Node *>::iterator iter;
	deque<Child_Node *>::iterator iter2;
	
	for (iter = cluster_list.begin(); iter != cluster_list.end(); iter++)
	{
		for (iter2 = adjacency_list.find (iter->first)->second.begin(); iter2 != adjacency_list.find (iter->first)->second.end(); iter2++)
			delete *iter2;
 		delete iter->second;
 	}
}


//----------
// operator
//----------


Cluster_Tree & Cluster_Tree::operator= (Cluster_Tree & ct)
// operator =
{
	if (this != &ct)
	{
		// we free the allocated memory
		map<int,Cluster_Node *>::iterator iter;
		deque<Child_Node *>::iterator iter3;
		
		for (iter = cluster_list.begin(); iter != cluster_list.end(); iter++)
		{
			for (iter3 = adjacency_list.find (iter->first)->second.begin(); iter3 != adjacency_list.find (iter->first)->second.end(); iter3++)
				delete *iter3;
			delete iter->second;
		}
		
		cluster_list.clear();
		adjacency_list.clear();

		// we copy the cluster-tree ct
		root = ct.root;
		nb_clusters = ct.nb_clusters;
		max_label = ct.max_label;
		nb_vertices = ct.nb_vertices;	
		g = ct.g;	
		
		Cluster_Node * cl;
		
		// copy of the cluster_list
		for (iter = ct.cluster_list.begin(); iter != ct.cluster_list.end(); iter++)
		{
			cl = new Cluster_Node (*(iter->second));
			cluster_list.insert(pair<int,Cluster_Node *>(iter->first,cl));	
			adjacency_list.insert (pair<int,deque<Child_Node *> >(iter->first,deque<Child_Node *>()));
		}
		
		// copy of the adjacency_list
		map<int,deque<Child_Node *> >::iterator iter2;
		Child_Node * intersection;
				
		
		for (iter2 = ct.adjacency_list.begin(); iter2 != ct.adjacency_list.end(); iter2++)
		{
			for (iter3 = iter2->second.begin(); iter3 != iter2->second.end(); iter3++)
			{
				intersection = new Child_Node (**iter3);
				intersection->Set_Cluster (cluster_list.find(intersection->Get_Cluster_Num())->second);
				adjacency_list.find(iter2->first)->second.push_back (intersection);
				
				cl = cluster_list.find(iter2->first)->second;
				if (intersection->Get_Cluster_Num() == cl->Get_Parent_Num())
					cl->Set_Separator (intersection);
			}
		}
	}
	return *this;
}


//-----------------
// basic functions 
//-----------------


void Cluster_Tree::Add_Cluster (Cluster_Node * cn, bool duplicate)
// adds a cluster to the cluster-tree without connecting it to the existing cluster-tree (the cluster is duplicated if duplicate is true)
{
	Cluster_Node * elt;

	if (duplicate)	elt = new Cluster_Node (*cn);
	else elt = cn;
	
	elt->Change_Graphical_Structure(g);
	
	max_label++;
	nb_clusters++;
	elt->Set_Num (max_label);
	
	if (root_num == -1)
	{
		root = elt;
		root_num = max_label;
	}
	
	cluster_list.insert(pair<int,Cluster_Node *>(max_label,elt));
	adjacency_list.insert (pair<int,deque<Child_Node *> >(max_label,deque<Child_Node *>()));
	
	nb_vertices = 0;
}


void Cluster_Tree::Add_Child_Node (Cluster_Node * parent, Cluster_Node * child)
// adds two child nodes in the cluster-tree such that child is a pointer on the child cluster and parent is a pointer on the parent cluster
{
	Child_Node * intersection = new Child_Node (g);

	for (vector<vertex>::iterator iter_elt = parent->begin(); iter_elt != parent->end(); iter_elt++)
		if (child->Is_Element (*iter_elt))
			intersection->Add_Vertex (*iter_elt);
	
	assert (intersection->Size() > 0);
	  
	intersection->Set_Cluster (child);
	adjacency_list.find(parent->Get_Num())->second.push_back (intersection);	
	
	Child_Node * intersection2 = new Child_Node (*intersection);
	
	intersection2->Set_Cluster (parent);
	child->Set_Separator (intersection2);	
	adjacency_list.find(child->Get_Num())->second.push_back (intersection2);

	nb_vertices = 0;
}


void Cluster_Tree::Set_Root (int root_label)
// sets the cluster whose label is root_label as root cluster of the cluster-tree
{	
	if (nb_clusters > 1)
	{
		root = Get_Cluster(root_label);
		root_num = root_label;
	
		// case of the root cluster
		root->Set_Parent (0);
		root->Set_Separator (0);
		
		// case of the other clusters
		vector<bool> mark = vector<bool> (max_label+1,false);
		queue<Cluster_Node *> next_cluster;
		Cluster_Node * cn;
		deque<Child_Node *> cn_deque, cn_deque2;
		deque<Child_Node *>::iterator iter,iter2;
		int lp, lchild;

		mark[root_label] = true;
		
		next_cluster.push (root);
		
		Cluster_Node * parent;
		
		do  // we apply a breath-first traversal of the cluster-tree
		{
			parent = next_cluster.front ();
			lp = parent->Get_Num();
			next_cluster.pop ();
			
			cn_deque = adjacency_list.find(lp)->second;
			
			for (iter= cn_deque.begin(); iter != cn_deque.end(); iter++)
			{
				lchild = (*iter)->Get_Cluster_Num();
				if (! mark[lchild])
				{
					cn = (*iter)->Get_Cluster();
			
					cn_deque2 = adjacency_list.find(lchild)->second;
					iter2 = cn_deque2.begin(); 
					while ((*iter2)->Get_Cluster_Num() != lp)
						iter2++;
						
					cn->Set_Separator (*iter2);
					
					// we add the label lchild to the queue
					mark[lchild] = true;
					next_cluster.push (cn);
				}
			}
		}
		while (next_cluster.size() > 0);
	}	
}


void Cluster_Tree::Set_Root (Cluster_Node * root_cluster)
// sets the cluster root_cluster as root cluster of the cluster-tree
{	
	if (nb_clusters > 1)
	{
		root = root_cluster;
		root_num = root->Get_Num();
	
		// case of the root cluster
		root->Set_Parent (0);
		root->Set_Separator (0);
		
		// case of the other clusters
		vector<bool> mark = vector<bool> (max_label+1,false);
		queue<Cluster_Node *> next_cluster;
		Cluster_Node * cn;
		deque<Child_Node *> cn_deque, cn_deque2;
		deque<Child_Node *>::iterator iter,iter2;
		int lp, lchild;

		mark[root_num] = true;
		
		next_cluster.push (root);
		
		Cluster_Node * parent;
		
		do  // we apply a breath-first traversal of the cluster-tree
		{
			parent = next_cluster.front ();
			lp = parent->Get_Num();
			next_cluster.pop ();
			
			cn_deque = adjacency_list.find(lp)->second;
			
			for (iter= cn_deque.begin(); iter != cn_deque.end(); iter++)
			{
				lchild = (*iter)->Get_Cluster_Num();
				if (! mark[lchild])
				{
					cn = (*iter)->Get_Cluster();
			
					cn_deque2 = adjacency_list.find(lchild)->second;
					iter2 = cn_deque2.begin(); 
					while ((*iter2)->Get_Cluster_Num() != lp)
						iter2++;
						
					cn->Set_Separator (*iter2);
					
					// we add the label lchild to the queue
					mark[lchild] = true;
					next_cluster.push (cn);
				}
			}
		}
		while (next_cluster.size() > 0);
	}	
}


void Get_Descent_Rec (Cluster_Tree * CT, Cluster_Node * cl, Vertex_Set * descent)
// returns the set of vertices which belong to the descent rooted in the cluster num whose parent number is parent_num
{
  int num = cl->Get_Num();
  int parent_num = cl->Get_Parent_Num();
  
	for (vector<vertex>::iterator iter = cl->begin(); iter != cl->end(); iter++)
		descent->Add_Vertex (*iter);

	deque<Child_Node *>::iterator iter2;
	for (iter2 = CT->Begin_Neighbor(num); iter2 != CT->End_Neighbor(num); iter2++)
		if ((*iter2)->Get_Cluster_Num() != parent_num)
			Get_Descent_Rec (CT,(*iter2)->Get_Cluster(),descent);
}


Vertex_Set Cluster_Tree::Get_Descent (int num)
// returns the set of vertices which belong to the descent rooted in the cluster num
{
	Vertex_Set descent (g);

	Get_Descent_Rec (this,Get_Cluster(num),&descent);
	
	return descent;
}


bool Cluster_Tree::Contains_Vertex (vertex x)
// returns true whether the cluster-tree contains the vertex x, false otherwise
{
  for (map<int,Cluster_Node *>::iterator iter = cluster_list.begin(); iter != cluster_list.end(); iter++)
    if (iter->second->Is_Element(x))
      return true;
  return false;
}


void Cluster_Tree::Print ()
// prints the cluster-tree
{
	map<int,Cluster_Node *>::iterator iter;
	deque<Child_Node *>::iterator iter2;
	
	for (iter = cluster_list.begin(); iter != cluster_list.end(); iter++)
	{
		cout << "Cluster " << iter->first << ": parent = " << iter->second->Get_Parent() << ", depth = " << iter->second->Get_Depth() << ", child =";
		cout.flush();
		
		for (iter2 = adjacency_list.find (iter->first)->second.begin(); iter2 != adjacency_list.find (iter->first)->second.end(); iter2++)
			if ((*iter2)->Get_Cluster()->Get_Parent_Num() == iter->first)
				cout << " " << (*iter2)->Get_Cluster_Num();
		
		cout << endl;
	}
  
  for (pair<int,Cluster_Node *> cl : cluster_list)
  {
    cout << "Cluster "  << cl.first << " " << cl.second->Size() << "/" << (cl.second->Get_Separator() == 0 ? 0 : cl.second->Get_Separator()->Size()) << ":";
    for (vector<vertex>::iterator iter = cl.second->begin(); iter != cl.second->end(); iter++)
      cout << " " << *iter ;
    cout << endl;
  }
}


//-----------------------------------
// functions modifying cluster-trees
//-----------------------------------


void Cluster_Tree::Merge_Clusters (Cluster_Node * cn)
// merges the cluster num with its parent
{
	int num = cn->Get_Num();

	if ((root_num != -1) && (num != root_num))
	{
		Cluster_Node * parent = cn->Get_Separator()->Get_Cluster();
		int parent_num = parent->Get_Num();
		deque<Child_Node *> cn_deque,cn_deque2, cn_deque_parent;
		deque<Child_Node *>::iterator iter,iter2;
		deque<Child_Node *> &cn_deque_parent_ref = adjacency_list.find(parent_num)->second;
		Cluster_Node *lchild;
		Child_Node *copy;	
		
		// we merge the cluster cn with its parent cluster
		parent->Merge(cn);
		
		// we remove the separator between cn and its parent from the parent's adjacency list
		for (iter = cn_deque_parent_ref.begin(); (*iter)->Get_Cluster_Num() != num; iter++);
		
		delete *iter;
		
		cn_deque_parent_ref.erase (iter);
			
		
		// we pass this merging on the cluster-tree
		cn_deque = adjacency_list.find(num)->second;
		for (iter = cn_deque.begin(); iter != cn_deque.end(); iter++)
		{
			lchild = (*iter)->Get_Cluster ();
			if (lchild->Get_Parent_Num() == num)
			{
				// the parent cluster of the current child is now the parent cluster of cn
				lchild->Set_Parent(parent);
				
				// the current child node of cn is added in the adjacency of its parent cluster
				copy = new Child_Node (**iter);
				cn_deque_parent_ref.push_back (copy);
						
				// we modify the child node related to cn in the adjacency list of the current child in such a way that the related cluster is now the parent cluster of cn
				cn_deque2 = adjacency_list.find((*iter)->Get_Cluster_Num())->second;
				iter2 = cn_deque2.begin();
				while ((iter2 != cn_deque2.end()) && ((*iter2)->Get_Cluster_Num() != num))
					iter2++;
				
				(*iter2)->Set_Cluster(parent);
				delete *iter;
			}
		}
		
				
		// we remove the cluster cn from the cluster list and the adjacency list related to the current cluster-tree
		delete cn->Get_Separator();
		delete cn;
		adjacency_list.erase(num);
		cluster_list.erase(num);
		
		nb_clusters--;
	}
}


void Cluster_Tree::Merge_Sep (int max_allowed_sep_size)
// merges each cluster with its parent if the size of the intersection is greater than max_allowed_sep_size
{
	if (root_num != -1)
	{
		stack<Cluster_Node *> next_cluster;
		deque<Child_Node *> cn_deque;

		Cluster_Node * lc, *lchild;
		int num;
		unsigned int i;
				
		next_cluster.push (root);

		do  // we apply a depth-first traversal of the cluster-tree
		{
			lc = next_cluster.top ();
			next_cluster.pop ();
	
			num = lc->Get_Num ();
						
			deque<Child_Node *> &cn_deque_ref = adjacency_list.find(num)->second;
			
			i = 0;

			while  (i < cn_deque_ref.size())
			{
				lchild = cn_deque_ref[i]->Get_Cluster();
				if (lchild->Get_Parent_Num() == num) 
				{
					if (lchild->Get_Separator()->Size() > max_allowed_sep_size)
					{
						// we merge the cluster lchild with its parent cluster
						Merge_Clusters (lchild);
					}	
					else
						{
							// we add the label lchild to the stack
							next_cluster.push (lchild);
							i++;
						}
				}
				else i++;
			}
		}
		while (next_cluster.size() > 0);
	}
}


void Cluster_Tree::Merge_VP (int min_allowed_vp)
// merges each cluster with its parent if the number of proper vertices is not greater than min_allowed_vp=
{
	if ((root_num != -1) && (min_allowed_vp > 0))
	{
		vector<Cluster_Node *> cluster_order (nb_clusters);
		queue<Cluster_Node *> cluster_queue;
		deque<Child_Node *>::iterator iter;
		Cluster_Node *cn, *lchild;
		int i, num;
		 
		// we first build an order on the clusters such that we begin each descent from the leaves (thanks to a breath-first traversal of the cluster-tree)
		cluster_queue.push (root);
		i = nb_clusters-1;
		do
		{
			cn = cluster_queue.front();
			cluster_queue.pop();
			cluster_order[i] = cn;
			i--;
			
			num = cn->Get_Num();
			deque<Child_Node *> & cn_deque_ref = adjacency_list.find(num)->second;
			for (iter = cn_deque_ref.begin(); iter != cn_deque_ref.end(); iter++)
			{
				lchild = (*iter)->Get_Cluster();
				if (lchild->Get_Parent_Num() == num)
					cluster_queue.push (lchild);
			}
		}
		while (! cluster_queue.empty());
			 
		
		// we now traverse the cluster-tree from the leaves to the root
		int nb_clusters_init = nb_clusters;
		for (i = 0; i < nb_clusters_init-1; i++)
		{
			cn = cluster_order[i];
			
			if (cn->Size() - cn->Get_Separator()->Size() <= min_allowed_vp)
			{
				// we merge the cluster cn with its parent cluster
				Merge_Clusters (cn);
			}
		}
	}
}


void Cluster_Tree::Merge_Descent (Cluster_Node * cn)
// merges the descent of the cluster cn
{
	Vertex_Set cn_descent = Get_Descent (cn->Get_Num());
	int desc_size = cn_descent.Size();
	unsigned int diff_size = desc_size - cn->Size();
	
	if (diff_size > 0)
	{
		vector<vertex> descent (desc_size);
		vector<bool> belong (max_label,false);
		int size = 0;
		
		stack<Cluster_Node *> S;
		
		S.push (cn);
		
		do
		{
			Cluster_Node * current_cluster = S.top ();
			int num = current_cluster->Get_Num();
			S.pop();
			
			// we add the vertice of the current cluster 
			for (vector<vertex>::iterator iter_vertex = current_cluster->begin(); iter_vertex != current_cluster->end(); iter_vertex++)
				if (! belong[*iter_vertex])
				{
					descent[size] = *iter_vertex;
					size++;
					belong[*iter_vertex] = true;
				}
			
			// we push into the stack the children of the current cluster
			for (deque<Child_Node *>::iterator iter_cluster = Begin_Neighbor (num); iter_cluster != End_Neighbor (num); iter_cluster++)
				if ((*iter_cluster)->Get_Cluster()->Get_Parent_Num() == num)
					S.push ((*iter_cluster)->Get_Cluster());

			
			if (cn == current_cluster)
			{
				for (deque<Child_Node *>::iterator iter, iter_cluster = Begin_Neighbor (num); iter_cluster != End_Neighbor (num); )
				{
					iter = iter_cluster;
					iter_cluster++;
					adjacency_list[num].erase (iter);
				}
					
			}
			else
				{
					// we delete the current_cluster
					delete current_cluster;
					
					adjacency_list.erase(num);
					cluster_list.erase(num);
					
					nb_clusters--;
				}
		}
		while (! S.empty());
		
		*cn = Cluster_Node (descent,g);
	}
}
