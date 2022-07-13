#include <fstream>
#include <iostream>


//----------------------------
// constructor and destructor
//----------------------------

template<class T>
Statistical_Counter<T>::Statistical_Counter ()
// constructs a null counter
{
	value = 0;
}


//-----------
// operators
//-----------

template<class T>
inline Statistical_Counter<T> & Statistical_Counter<T>::operator++ (int)
// increases by one the value of the counter
{
	value++;
	return *this;
}


template<class T>
inline Statistical_Counter<T> & Statistical_Counter<T>::operator-- (int)
// decreases by one the value of the counter
{
	value--;
	return *this;
}


//-----------------
// basic functions
//-----------------

template<class T>
inline void Statistical_Counter<T>::Reset ()
// resets the counter to the value 0
{
	value = 0;
}


template<class T>
inline T Statistical_Counter<T>::Get_Value ()
// returns the current value of	the counter
{
	return value;
}


template<class T>
inline void Statistical_Counter<T>::Set_Value (T val)
// sets the value of the counter to the value val
{
	value = val;
}


template<class T>
inline void Statistical_Counter<T>::Add (T val)
// adds the value val to the counter
{
	value += val;	
}


template<class T>
void Statistical_Counter<T>::Save (string filename, string information)
// saves the value of the counter in the file filename (the value is preceded by the string information)
{
  ofstream file (filename.c_str(), ios::app);
	
	if (file.fail())
	{
	  cerr << "Error: cannot create the file " << filename << "\n";
	  return;
	}
	
	// we write the average value of the counter preceded by the string information
	
	file << information << Get_Value() << endl;
	
	file.close();

}
