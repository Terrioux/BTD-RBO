/// \file 
/// \brief Definitions related to the Graphical_Structure class

#ifndef GRAPHICAL_STRUCTURE_H
#define GRAPHICAL_STRUCTURE_H

#include <vector>
#include <set>

using namespace std;

typedef unsigned int vertex;      ///< the type allowing to represent vertices  \ingroup core

class Light_Graph;
class Graph;

class Graphical_Structure      /// This class allows to represent graphical structures
{
	protected :
		unsigned int n;  									 	///< number of vertices
		unsigned int m;   							 		///< numer of edges
		vector<unsigned int> degree;  ///< the degree of each vertex
		Light_Graph * plg;							 	  ///< the primal graph represented as a light graph
		Graph * pg;													///< the primal graph represented as a light graph
		
	public :
		// constructor and destructor
		Graphical_Structure();							///< builds an empty graphical structure
		virtual ~Graphical_Structure();			///< destructor
	
		// basic functions	
		unsigned int N ();  								///< returns the number of vertices
		unsigned int M ();  								///< returns the number of edges
		unsigned int Degree (vertex x);   	///< returns the degree of the vertex x
		vector<unsigned int> & Degrees (); ///< returns the degree of for each vertex
		
		virtual void Init (unsigned int nb_vertices)=0; 					  ///< initializes the structure with n vertices and 0 edge
		virtual void Add_Vertex ()=0;   														///< adds a vertex to the graphical structure
		virtual void Add_Edge (vertex elements[], int arity)=0;   	///< adds the vertices of elements as a new edge
		void Add_Edge (set<vertex> & e); 													 	///< adds the vertices of e as a new edge
		
		
		// miscellaneous functions
		virtual Light_Graph * Get_Primal_Light_Graph () = 0;	 ///< returns the primal graph related to the graphical structures as a pointer on a Light_Graph object
		virtual Graph * Get_Primal_Graph () = 0;							 ///< returns the primal graph related to the graphical structures as a pointer on a Graph object
};


//-----------------------------
// inline function definitions
//-----------------------------


inline unsigned int Graphical_Structure::N ()  
// return the number of vertices
{
	return n;
}


inline unsigned int Graphical_Structure::M ()
// return the number of edges
{
	return m;
}


inline unsigned int Graphical_Structure::Degree (vertex x)
// returns the degree of the vertex x
{
	return degree[x];
}


inline vector<unsigned int> & Graphical_Structure::Degrees ()
// returns the degree of for each vertex
{
	return degree;
}

#endif
