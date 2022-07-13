#include "enhanced_heuristic_tree_decomposition.h"
#include "connected_component.h"
#include <typeinfo>
#include <iostream>
#include <tuple>


//----------------------------
// constructor and destructor
//----------------------------


Enhanced_Heuristic_Tree_Decomposition::Enhanced_Heuristic_Tree_Decomposition (Graphical_Structure * gs, Clique_First_Cluster_Heuristic * fch, Sep_K_Neighbors_Next_Vertices_Heuristic * nvh) : Tree_Decomposition (gs)
// constructs a tree-decomposition related to the graphical structure gs by exploiting the heuristic fc for computing the first cluster and the heuristic nv for choosing the next vertices to consider
// this tree decomposition is done without triangulation
{
	first_cluster = fch;
	next_vertices = nvh;
	Compute();
}


Enhanced_Heuristic_Tree_Decomposition::~Enhanced_Heuristic_Tree_Decomposition ()
// destructor
{
	delete first_cluster;
	delete next_vertices;
}


//-----------------
// basic functions
//-----------------


void Enhanced_Heuristic_Tree_Decomposition::Compute ()
{
  if ((g->N()*(g->N()-1))/2 == g->Get_Primal_Graph()->M())
  {
    // the primal graph is complete
    cluster_tree.resize(1);
    cluster_tree[0] = new Cluster_Tree(g);
    ct_number = 1;
    Cluster_Node * cluster;
    cluster = new Cluster_Node (g);
    
    for (vertex x = 0; x < g->N(); x++)
      cluster->Add_Vertex(x);

    cluster_tree[0]->Add_Cluster (cluster);
  }
  else
    {
      Connected_Component CC (g);	
      int n = g->N();
      bool * is_untreated = new bool [n];
      int remaining_vertices = n;
      
      for (int i = 0; i < n; i++)
        is_untreated[i] = true;
      
      int current_w = -1;
      
      do
      {
        // we take into account a new cluster tree
        if ((unsigned) ct_number+1 > cluster_tree.size ())
          cluster_tree.resize(cluster_tree.size ()+50);
        cluster_tree[ct_number] = new Cluster_Tree(g);

        // we compute the first cluster
        Cluster_Node * cluster;
        
        cluster = first_cluster->Choose (is_untreated);

        if (cluster->Size()-1 > current_w)
          current_w = cluster->Size() - 1; 
          
        remaining_vertices -= cluster->Size();
        
        cluster_tree[ct_number]->Add_Cluster (cluster);

        // we now process the remaining part of the current connected component (if any)
        if (remaining_vertices > 0)
        {
          stack<tuple<pair<int,vertex *>,Cluster_Node *,pair<int,vertex *>>> S;
          
          CC.Get_Connected_Components_From_Set (cluster,S);
              
          pair<int,vertex *> X_i;
          pair<int,vertex *> V_i;

          list<vertex>::iterator iter;
          list<Cluster_Node *>::iterator iter_cluster;
          Cluster_Node * parent;
          
          while (! S.empty())
          {
            // we consider a new set X_i with its parent cluster and its neighborhood V_i 
            X_i = get<0>(S.top());
            parent = get<1>(S.top());
            V_i = get<2>(S.top());
            S.pop();
            
            // we check if V_i corresponds to an existing cluster	
            if (V_i.first == parent->Size())
            {
              // V_i and parent represent the same set of vertices. So we add the vertices of E_i \ V_i in parent
              cluster = parent;
            }
            else
              {
                // we add E_i as a new cluster
                cluster = new Cluster_Node (g);
                for (int i = 0; i < V_i.first; i++)
                  cluster->Add_Vertex (V_i.second[i]);

                cluster_tree[ct_number]->Add_Cluster (cluster);
                cluster_tree[ct_number]->Add_Child_Node (parent,cluster);
              }
            
            // we construct the new cluster
            next_vertices->Choose (X_i,cluster,V_i,current_w,is_untreated);
            
            remaining_vertices -= cluster->Size() - V_i.first;
              
            if (cluster->Size() -1 > current_w)
              current_w = cluster->Size() - 1; 

            // we compute the connected components of g [X_i \ E_i] and their neighborhood
            CC.Get_Connected_Components_With_Neighborhood (X_i,cluster,S);
          
            delete [] X_i.second;
            delete [] V_i.second;
          }
        }
        ct_number++;
      }
      while (remaining_vertices > 0);

      delete [] is_untreated;	
    }
}
