/// \file 
/// \brief Definitions related to the Domain class

#ifndef DOMAIN_H
#define DOMAIN_H

using namespace std;

#include <vector>
#include <set>
#include <map>
#include <cassert>
#include "event_manager.h"


class Domain               /// This class allows to represent domains with event management \ingroup core
{
	protected :
		unsigned int initial_size;  		        ///< initial size of the domain
		unsigned int size; 						          ///< current size of the domain
		vector<int> dom; 											  ///< the list of the values (values located before the size index are present, while ones located after have been removed)
		vector<unsigned int> position; 				  ///< the position of each value in the vector dom 
		vector<int> min;	    								  ///< the index of the current minimum
		vector<int> max;			    						  ///< the index of the current maximum
    unsigned int var;                       ///< the number of the variable to which the domain is related
    Event_Manager * event_manager;          ///< the event manager to which events are sent
		
    
	public :
		// constructors and destructor
    Domain (Event_Manager * evm, unsigned int num_var, unsigned int init_size);  ///< constructs a domain for the variable num_var and the event manager evm
    virtual ~Domain (){}; 				    	    ///< destructor
    	
		// basic functions
		bool Is_Empty ();					 		 				            ///< returns true if the domain is empty, false otherwise
		unsigned int Get_Initial_Size (); 		            ///< returns the initial size of the domain
		unsigned int Get_Size (); 						            ///< returns the current size of the domain 
		bool Is_Present (int val);						            ///< returns true if the value val is present in the domain, false otherwise
		virtual int Get_Value (int real_val) = 0;			    ///< returns the value corresponding to the real value real_val if it exists, -1 otherwise
    virtual int Get_Real_Value (int num) = 0;					///< returns the real value corresponding to the value num
    int Get_Deleted_Value (int i);                    ///< returns the value of the i th deleted value
    int Get_Deleted_Real_Value (int i);               ///< returns the real value of the i th deleted value
		int Get_Remaining_Value (int i);			            ///< returns the value of the i th remaining value
		int Get_Remaining_Real_Value (int i);	            ///< returns the real value corresponding to the i th remaining value
		void Filter_Value (int val);           	          ///< removes by filtering the value val
    virtual void Filter_Value_Before (int real_val);  ///< removes by filtering all the values which are smaller than the real value val (val included)
		virtual void Filter_Value_From (int real_val);   	///< removes by filtering all the values which are greater than the real value val (val included)
		void Assign (int val);			        		          ///< removes all the remaining values except the value val
    void WipeOut ();		         	                    ///< removes all the remaining values
		void Unfilter_Value (int val); 			              ///< restores the value val deleted by filtering
		void Restore (unsigned int new_size);	            ///< restores the last removed values in order to the domain has new_size remaining values (if new_size > size)
		virtual int Get_Min ();								            ///< returns the lowest value currently in the domain
		virtual int Get_Max ();								            ///< returns the largest value currently in the domain
		int Get_Real_Min ();  								            ///< returns the lowest real value currently in the domain
		int Get_Real_Max ();												      ///< returns the largest real value currently in the domain
		int Next_Value (int i);								            ///< returns the available value following the i th value in the growing ordering if any, -1 otherwise
		int Previous_Value (int i);						            ///< returns the available value preceding the i th value in the growing ordering if any, -1 otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline bool Domain::Is_Empty ()
// returns true if the domain is empty, false otherwise
{
	return size == 0;
}


inline unsigned int Domain::Get_Initial_Size ()
// returns the initial size of the domain
{
	return initial_size;
}


inline unsigned int Domain::Get_Size ()
// returns the current size of the domain
{
	return size;
}


inline bool Domain::Is_Present (int val)
// returns true if the value val is present in the domain, false otherwise
/// \param[in] val the value whose presence we want to test
{
	return position[val] < size;
}


inline int Domain::Get_Remaining_Value (int i)
// returns the value of the i th remaining value
/// \param[in] i the position of the wished value among the remaining values
{
	return dom[i];
}


inline int Domain::Get_Deleted_Value (int i)
// returns the value of the i th deleted value
/// \param[in] i the position of the wished value among the deleted values
{
	return dom[i+size];
}


inline int Domain::Get_Deleted_Real_Value (int i)
// returns the real value of the i th deleted value
/// \param[in] i the position of the whished value among the deleted values
{
  return Get_Real_Value(dom[i+size]);
}


inline int Domain::Get_Remaining_Real_Value (int i)
// returns the real value corresponding to the i th remaining value
/// \param[in] i the position of the whished real value among the remaining values
{
	return Get_Real_Value(dom[i]);
}


inline void Domain::Filter_Value (int val)
// removes by filtering the value val
/// \param[in] val the value we want to remove
{
  assert (position[val] < size);
 	size--;	

 	int pos = position[val];
 	dom[pos] = dom[size];
 	dom[size] = val;
 	
 	position[dom[pos]] = pos;
 	position[val] = size;
  
  if (size > 0)
  {
    if (size == 1)
    {
      event_manager->Add_Event_Fix (var,dom[0]);
      
      min[size-1] = dom[0];
      max[size-1] = dom[0];
    }
    else 
      {
        event_manager->Add_Event_Dmc (var,val);
        min[size-1] = min[size];
        max[size-1] = max[size];
      }
    
    if (min[size] == val)
      event_manager->Add_Event_Lbc (var);
      
    if (max[size] == val)
      event_manager->Add_Event_Ubc (var);
  }
  else event_manager->Add_Event_Dmc (var,val);
}



inline void Domain::Filter_Value_Before (int real_val)
// removes by filtering all the values which are smaller than the real value real_val (real_val included)
/// \param[in] real_val the real value from which the smaller values will be removed{
{
  int init_size = size;
 	
  for (int i = size-1; i >= 0; i--)
    if (Get_Real_Value(dom[i]) <= real_val)
    {
      event_manager->Add_Event_Dmc (var,dom[i]);
      
      size--;	

      int aux = dom[i];
      dom[i] = dom[size];
      dom[size] = aux;
      
      position[dom[i]] = i;
      position[aux] = size;
      
    }
  
  if (size == 1)
  {
    if (init_size != 1)
      event_manager->Add_Event_Fix (var,dom[0]);
    
    min[size-1] = dom[0];
    max[size-1] = dom[0];
  }
  else 
    if (size > 1)
    {
      min[size-1] = min[init_size-1];
      if (position[min[size-1]] >= size)
        event_manager->Add_Event_Lbc (var);
      
      max[size-1] = max[init_size-1];
      if (position[max[size-1]] >= size)
        event_manager->Add_Event_Ubc (var);
    }
}


inline void Domain::Filter_Value_From (int real_val)
// removes by filtering all the values which are greater than the real value real_val (real_val included)
/// \param[in] real_val the real value from which the greater real values will be removed
{
  int init_size = size;
 	
  for (int i = size-1; i >= 0; i--)
    if (Get_Real_Value(dom[i]) >= real_val)
    {
      event_manager->Add_Event_Dmc (var,dom[i]);
      
      size--;	

      int aux = dom[i];
      dom[i] = dom[size];
      dom[size] = aux;
      
      position[dom[i]] = i;
      position[aux] = size;
    }
  
  if (size == 1)
  {
    if (init_size != 1)
      event_manager->Add_Event_Fix (var,dom[0]);
    
    min[size-1] = dom[0];
    max[size-1] = dom[0];
  }
  else 
    if (size > 1)
    {
      min[size-1] = min[init_size-1];
      if (position[min[size-1]] >= size)
        event_manager->Add_Event_Lbc (var);
      
      max[size-1] = max[init_size-1];
      if (position[max[size-1]] >= size)
        event_manager->Add_Event_Ubc (var);
    }
}

inline void Domain::Assign (int val)
// removes all the remaining values except the value val
/// \param[in] val the value we want to assign
{
  if (position[val] < size)
  {
    if (size > 1)
    {
      event_manager->Add_Event_Fix (var,val);

      if (min[size-1] != val)
        event_manager->Add_Event_Lbc (var);
        
      if (max[size-1] != val)
        event_manager->Add_Event_Ubc (var);
      
      size = 1;	
      
      int pos = position[val];
      dom[pos] = dom[0];
      dom[0] = val;
      
      position[dom[pos]] = pos;
      position[val] = 0;

      min[0] = val;
      max[0] = val;
    }
  }
  else 
    {
      size = 0;
      event_manager->Add_Event_Fix (var,val);
    }
}


inline void Domain::WipeOut ()
// removes all the remaining values
{
  event_manager->Add_Event_Fix (var,-1);
  size = 0;
}


inline void Domain::Unfilter_Value (int val)
// restores the value val deleted by filtering
/// \param[in] val the value we want to restore
{
	int pos = position[val];
 	dom[pos] = dom[size];
 	dom[size] = val;
 	
 	position[dom[pos]] = pos;
 	position[val] = size;	
 	
 	size++;
}


inline void Domain::Restore (unsigned int new_size)
// restores the last removed values in order to the domain has new_size remaining values (if new_size > size)
/// \param[in] new_size the size we want the domain to have after restoring
{
	if (new_size > size)
		size = new_size;
}


inline int Domain::Get_Min ()
// returns the lowest value currently in the domain
{
	int i = min[size-1];
  while (position[i] >= size)
		i++;
    
	return min[size-1] = i;
}


inline int Domain::Get_Max ()
// returns the largest value currently in the domain
{
	int i = max[size-1];
  while (position[i] >= size)
		i--;
    
	return max[size-1] = i;
}


inline int Domain::Get_Real_Min ()
// returns the lowest real value currently in the domain
{
  return Get_Real_Value(Get_Min());
}


inline int Domain::Get_Real_Max ()
// returns the largest real value currently in the domain
{
  return Get_Real_Value(Get_Max());
}


inline int Domain::Next_Value (int i)
// returns the available value following the i th value in the growing ordering if any, -1 otherwise
/// \param[in] i the position before the wished value (in the growing ordering)
{
  do
    i++;
	while (((unsigned) i < initial_size) && (position[i] >= size));
  
	if ((unsigned) i < initial_size)
		return i;
	else return -1;
}


inline int Domain::Previous_Value (int i)
// returns the available value preceding the i th value in the growing ordering if any, -1 otherwise
/// \param[in] i the position after the wished value (in the growing ordering)
{
  do
    i--;
	while ((i >= 0) && (position[i] >= size));

	if (i >= 0)
		return i;
	else return -1;
}

#endif
