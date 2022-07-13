/// \file 
/// \brief Definitions related to the Filtering_Data_Structure class

#ifndef FILTERING_DATA_STRUCTURE_H
#define FILTERING_DATA_STRUCTURE_H

using namespace std;

#include <queue>

template<class T>
class Filtering_Data_Structure   /// This class implements the queue representing the set of the constraints for which the consistency must be checked  \ingroup filtering
{
	protected:
		queue<T> elements;    ///< the elements which defines the set of constraints

	public:	
		// constructor and destructor
		Filtering_Data_Structure ();    ///< constructs an empty stack
		~Filtering_Data_Structure ();	  ///< destructor
	
		// basic functions
		bool Is_Empty();	 		 ///< returns true if the structure is empty, false otherwise
		void Insert (T elt);	 ///< adds an element in the structure
		void Remove ();			 ///< removes the current element of the structure
		T Current ();				 ///< returns the current element of the structure
		void Clear (); 			 ///< removes all the elements of the structure
};


#include "filtering_data_structure.tpp"

#endif

