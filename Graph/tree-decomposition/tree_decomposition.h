/// \file 
/// \brief Definitions related to the Tree_Decomposition class

#ifndef TREE_DECOMPOSITION_H
#define TREE_DECOMPOSITION_H

#include "graph.h"
#include "graphical_structure.h"
#include "cluster_tree.h"
#include <list>

class Tree_Decomposition                    /// This class allows to represent tree-decompositions of graphical structures (see [Robertson and Seymour, 1986])  \ingroup tree-decomposition
{
	protected:
		Graphical_Structure * g;								///< the graphical structure to which the tree-decomposition refers
		vector<Cluster_Tree *> cluster_tree;  	///< the cluster-trees of the tree-decomposition (we consider a cluster-tree per connected componed of the graphical structure g)
		unsigned int ct_number;									///< the number of cluster-trees of the tree-decomposition
		
	public:
		// constructors and destructor
		Tree_Decomposition (Graphical_Structure * gs);		      ///< constructs an empty tree-decomposition related to the graphical structure gs
		Tree_Decomposition (Tree_Decomposition & td);			      ///< constructs a tree-decomposition by copying the tree-decomposition td
		virtual ~Tree_Decomposition ();													///< destructor
		
		// operator
		Tree_Decomposition & operator= (Tree_Decomposition & td); 	///< operator =
		
		// basic functions on cluster-tree
		int Get_W ();											///< returns the width of the tree-decomposition
		int Get_HW ();										///< returns the hyperwidth of the tree-decomposition
		int Get_H ();											///< returns the height of the tree-decomposition
		int Get_S ();											///< returns the size of the largest separator
		int Nb_Clusters ();							  ///< returns the number of clusters of the tree-decomposition
		int Get_Max_Label ();							///< returns the largest label of clusters in a cluster-tree
		int Get_Max_Vertices_Number ();		///< returns the largest number of vertices in a the cluster-tree
		int Get_Diameter ();				 			///< returns the diameter of the tree-decomposition
		unsigned int Get_Cluster_Tree_Number();		///< returns the number of cluster-trees of the considered tree-decomposition
		Cluster_Tree * Get_Cluster_Tree (unsigned int i);	///< returns the i th cluster-tree of the considered tree-decomposition
		Graphical_Structure * Get_Graphical_Structure ();	///< returns the graphical structure to which the tree-decomposition is related
		bool Is_Valid ();																	///< returns true if the current tree-decomposition is valid, false otherwise
		bool Is_Connected_Tree_Decomposition (); 					///< returns true if all the clusters have a single connected component, false otherwise
    void Add_Cluster_Tree (Cluster_Tree * ct);        ///< adds the cluster-tree to the tree-decomposition
    void Remove_Cluster_Tree (unsigned int num);      ///< removes the cluster-tree whose number is num 
    void Swap_Cluster_Tree (unsigned int i, unsigned int j);    ///< swaps the cluster-trees whose indices are respectively i and j
		// function computing cluster-trees
		virtual void Make_Tree (pair<int,list<Cluster_Node *>> & lc);		///< makes a tree from the cluster list lc
		
		// functions modifying cluster-trees
		virtual void Merge_Sep (int max_allowed_sep_size);		///< merges each cluster with its parent if the size of the intersection is greater than max_allowed_sep_size
		virtual void Merge_VP (int min_allowed_vp, bool until_fix_point=false);		///< merges each cluster with its parent if the number of proper vertices is not greater than min_allowed_vp (if until_fix_point is set to true, the process is repeat by choosing each remaining cluster as root until no cluster may be merged)
};


//-----------------------------
// inline function definitions
//-----------------------------

//-----------------
// basic functions 
//-----------------


inline unsigned int Tree_Decomposition::Get_Cluster_Tree_Number()
// returns the number of cluster-trees of the considered tree-decomposition
{
	return ct_number;
}


inline Cluster_Tree * Tree_Decomposition::Get_Cluster_Tree (unsigned int i)
// returns the i th cluster-tree of the considered tree-decomposition
{
	if (i < ct_number)	return cluster_tree[i];
	else return 0;
}


inline Graphical_Structure * Tree_Decomposition::Get_Graphical_Structure ()
// returns the graphical structure to which the tree-decomposition is related
{
	return g;
}


inline void Tree_Decomposition::Add_Cluster_Tree (Cluster_Tree * ct)
// adds the cluster-tree to the tree-decomposition
{
  ct_number++;
  if ((unsigned) ct_number > cluster_tree.size ())
    cluster_tree.resize(cluster_tree.size()+50);

  cluster_tree[ct_number-1] = ct;
}


inline void Tree_Decomposition::Swap_Cluster_Tree (unsigned int i, unsigned int j)
// swaps the cluster-trees whose indices are respectively i and j
{
  if ((i < ct_number) && (j < ct_number))
  {
    Cluster_Tree * aux;
    aux = cluster_tree[i];
    cluster_tree[i] = cluster_tree[j];
    cluster_tree[j] = aux;
  }
}

inline void Tree_Decomposition::Remove_Cluster_Tree (unsigned int num)
// removes the cluster-tree whose number is num 
{
  if (num < ct_number)
  {
    delete cluster_tree[num];
    cluster_tree.erase(cluster_tree.begin()+num);
    ct_number--;
  }
}

#endif
