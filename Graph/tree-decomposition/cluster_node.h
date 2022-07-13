/// \file 
/// \brief Definitions related to the Cluster_Node class

#ifndef CLUSTER_NODE_H
#define CLUSTER_NODE_H

#include <vector>
#include "graph.h"
#include "vertex_set.h"

class Child_Node;

class Cluster_Node: public Vertex_Set     /// This class allows to represent the nodes of cluster-trees (i.e. clusters)  \ingroup tree-decomposition
{
	private:
		int num;													///< the number of the cluster
		Child_Node * separator;						///< the separator between the current cluster and its parent
		Cluster_Node * parent;            ///< the parent of the current cluster

	public:
		// constructors and destructor	
		Cluster_Node (Graphical_Structure * gs);																  	///< constructs an empty cluster node related to the graphical structure gs
		Cluster_Node (vector<vertex> element_set, Graphical_Structure * gs);	///< constructs a vertex set containing the vertices of element_set and related to the graphical structure gs
		Cluster_Node (Cluster_Node & cn);			                 ///< constructs a cluster node by copying the cluster node cn
		virtual ~Cluster_Node ();															 ///< destructor
		
		// operator
		Cluster_Node & operator= (const Cluster_Node & cn);    ///< operator =
		
		// basic functions on cluster node
		int Get_Num ();							   											 ///< returns the number of the cluster
		void Set_Num (int number);		 											 ///< sets the number of the cluster
		int Get_Depth (); 																	 ///< returns the depth
		Cluster_Node * Get_Parent ();												 ///< returns the parent
		int Get_Parent_Num ();															 ///< returns the parent number
		void Set_Parent (Cluster_Node * p);									 ///< sets the parent to the cluster p
		Child_Node * Get_Separator ();											 ///< returns the separator between the current cluster and its parent
		void Set_Separator (Child_Node * cn);								 ///< sets the separator between the current cluster and its parent
};


//-----------------------------
// inline function definitions
//-----------------------------


//-----------------
// basic functions
//-----------------


inline int Cluster_Node::Get_Num ()
// returns the number of the cluster
{
	return num;
}


inline void Cluster_Node::Set_Num (int number)
// sets the number of the cluster
{
	num = number;		 											 
}


inline Cluster_Node * Cluster_Node::Get_Parent ()
// returns the parent
{
	return parent;
}


inline int Cluster_Node::Get_Parent_Num ()
// returns the parent number
{
  if (parent == 0)
    return -1;
	else return parent->Get_Num();
}


inline void Cluster_Node::Set_Parent (Cluster_Node * p)
// sets the parent to the cluster p
{
	parent = p;
}


inline Child_Node * Cluster_Node::Get_Separator ()
// returns the separator between the current cluster and its parent
{
	return separator;
}
#endif

