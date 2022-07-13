/// \file 
/// \brief Definitions related to the Child_Node class

#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <vector>
#include "graph.h"
#include "cluster_node.h"
#include "vertex_set.h"

class Child_Node: public Vertex_Set   /// This class allows to represent the intersection between two clusters of a tree-decomposition  \ingroup tree-decomposition
{
	protected:
		Cluster_Node * cluster;						///< a pointer on the child cluster whose number is cluster_num

	public:
		// constructors and destructor
		Child_Node (Graphical_Structure * gs);															  ///< constructs an empty child node related to the graphical structure gs
		Child_Node (vector<vertex> element_set, Graphical_Structure * gs);  	///< constructs a vertex set containing the vertices of element_set and related to the graphical structure gs
		Child_Node (Child_Node & cn);			                 	 ///< constructs a child node by copying the child node cn
		virtual ~Child_Node ();															 ///< destructor
		
		// operator
		Child_Node & operator= (const Child_Node & cn);   	 ///< operator =
		
		// basic functions on cluster node
		int Get_Cluster_Num (); 														 ///< returns the number of the child cluster
		Cluster_Node * Get_Cluster (); 											 ///< returns a pointer on the child cluster
		void Set_Cluster (Cluster_Node * cn); 							 ///< sets a pointer on the child cluster and the number of the child cluster
};

//-----------------------------
// inline function definitions
//-----------------------------

//-----------------
// basic functions
//-----------------

inline int Child_Node::Get_Cluster_Num ()
// returns the number of the child cluster
{
  if (cluster == 0)
    return -1;
  else return cluster->Get_Num();
}


inline Cluster_Node * Child_Node::Get_Cluster ()
// returns a pointer on the child cluster
{
	return cluster;
}


inline void Child_Node::Set_Cluster (Cluster_Node * cn)
// sets a pointer on the child cluster and the number of the child cluster
{
	cluster = cn;
}

#endif

