#include "csp_cluster_tree.h"
#include "csp_child_node.h"
#include "tree_csp_child_node.h"
#include <stack>


CSP_Cluster_Tree::CSP_Cluster_Tree (Cluster_Tree * ct, CSP * pb, bool with_nogood_set): Cluster_Tree(pb->Get_Structure())
// constructs a CSP cluster tree from an existing cluster tree
{
	root_num = ct->Get_Root_Num();
	nb_clusters = ct->Nb_Clusters();
	max_label = ct->Get_Max_Label();
	nb_vertices = ct->Get_Nb_Vertices ();
	
	Cluster_Node * cl;
	
	// copy of the cluster_list
	for (map<int,Cluster_Node *>::iterator iter = ct->Begin_List(); iter != ct->End_List(); iter++)
	{
		cl = new Cluster_Node (*(iter->second));
		cluster_list.insert(pair<int,Cluster_Node *>(iter->first,cl));	
		adjacency_list.insert (pair<int,deque<Child_Node *> >(iter->first,deque<Child_Node *>()));
	}
	
	// copy of the adjacency_list
	Child_Node * intersection;
	  		
	for (map<int,Cluster_Node *>::iterator iter = ct->Begin_List(); iter != ct->End_List(); iter++)
	{
		for (deque<Child_Node *>::iterator iter3 = ct->Begin_Neighbor(iter->first); iter3 != ct->End_Neighbor(iter->first); iter3++)
		{
			intersection = new Tree_CSP_Child_Node (**iter3,iter->second,pb,with_nogood_set);
			intersection->Set_Cluster (cluster_list.find(intersection->Get_Cluster_Num())->second);
			adjacency_list.find(iter->first)->second.push_back (intersection);
						
			cl = cluster_list.find(iter->first)->second;
			if (intersection->Get_Cluster_Num() == cl->Get_Parent_Num())
				cl->Set_Separator (intersection);
		}
	}
	root = cluster_list.find(root_num)->second;
}


CSP_Cluster_Tree::~CSP_Cluster_Tree ()
// destructor
{
}


//-----------------
// basic functions
//-----------------

void CSP_Cluster_Tree::Merge_Clusters (Cluster_Node * cn)
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
		
		// we merge the cluster cn with its parent cluster
		parent->Merge(cn);
		
		// we remove the separator between cn and its parent from the parent's adjacency list
		for (iter = cn_deque_parent_ref.begin(); (*iter)->Get_Cluster_Num() != num; iter++);
		
		delete *iter;
		
		cn_deque_parent_ref.erase (iter);
			
		
		// we pass this merging on the cluster tree
		cn_deque = adjacency_list.find(num)->second;
		for (iter = cn_deque.begin(); iter != cn_deque.end(); iter++)
		{
			lchild = (*iter)->Get_Cluster ();
			if (lchild->Get_Parent_Num() == num)
			{
				// the parent cluster of the current child is now the parent cluster of cn
				lchild->Set_Parent(parent);
				
				// the current child node of cn is added in the adjacency of its parent cluster
				cn_deque_parent_ref.push_back (*iter);
						
				// we modify the child node related to cn in the adjacency list of the current child in such a way that the related cluster is now the parent cluster of cn
				cn_deque2 = adjacency_list.find((*iter)->Get_Cluster_Num())->second;
				iter2 = cn_deque2.begin();
				while ((iter2 != cn_deque2.end()) && ((*iter2)->Get_Cluster_Num() != num))
					iter2++;
				
				(*iter2)->Set_Cluster(parent);
			}
		}
	
		// we remove the cluster cn from the cluster list and the adjacency list related to the current cluster tree
		delete cn->Get_Separator();
		delete cn;
		adjacency_list.erase(num);
		cluster_list.erase(num);
		
		nb_clusters--;
	}
}
