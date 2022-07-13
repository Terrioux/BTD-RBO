//----------------------------
// constructor and destructor
//----------------------------

template<class T>
Filtering_Data_Structure<T>::Filtering_Data_Structure ()
// constructs an empty stack
{
}


template<class T>
Filtering_Data_Structure<T>::~Filtering_Data_Structure ()
// destructor
{
}
	

//-----------------
// basic functions
//-----------------

template<class T>
inline bool Filtering_Data_Structure<T>::Is_Empty()
// returns true if the structure is empty, false otherwise
{
	return elements.empty();
}


template<class T>
inline void Filtering_Data_Structure<T>::Insert (T elt)
// adds an element in the structure
{
	elements.push(elt);
}


template<class T>
inline void Filtering_Data_Structure<T>::Remove ()
// removes the current element of the structure
{
	elements.pop();
}


template<class T>
inline T Filtering_Data_Structure<T>::Current ()
// returns the current element of the structure
{
	return elements.front ();
}



template<class T>
inline void Filtering_Data_Structure<T>::Clear ()
// removes all the elements of the structure
{
	while  (! elements.empty())
		elements.pop();
}
