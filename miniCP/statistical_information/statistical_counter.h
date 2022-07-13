/// \file 
/// \brief Definitions related to the Statistical_Statistical_Counter class

#ifndef STATISTICAL_COUNTER_H
#define STATISTICAL_COUNTER_H

#include <string>

using namespace std;


template<class T>
class Statistical_Counter       /// This class implements generic counters \ingroup information
{
	protected:
		T value;				///< the current value of the counter

	public:	
		// constructor and destructor
		Statistical_Counter ();    ///< constructs a null counter
	
		// operators
		Statistical_Counter<T> & operator++ (int);			///< increases by one the value of the counter
		Statistical_Counter<T> & operator-- (int);			///< decreases by one the value of the counter
	
		// basic functions
		void Reset ();														///< resets the counter to the value 0
		T Get_Value ();														///< returns the current value of	the counter
		void Set_Value (T val);										///< sets the value of the counter to the value val
		void Add (T val);													///< adds the value val to the counter
		void Save (string filename, string information);  ///< saves the value of the counter in the file filename (the value is preceded by the string information)
};


#include "statistical_counter.tpp"

#endif

