/// \file 
/// \brief Definitions related to the Bound_Domain class

#ifndef BOUND_DOMAIN_H
#define BOUND_DOMAIN_H

#include "domain.h"


class Bound_Domain: public Domain    /// This class implements singleton domains with event management (these domains are related to the bounds of a COP instance) \ingroup core
{
	protected :
    long value;     ///< the single value of the domain

	public :
		// constructor
    Bound_Domain (Event_Manager * evm, unsigned int num_var, long val);   		///< constructs a domain defined by the values of [a,b]
		
		// basic functions	
    int Get_Value (long real_val) override;	  ///< returns the value corresponding to the real value real_val if it exists, -1 otherwise
    long Get_Real_Value (int num) override;		///< returns the real value corresponding to the value num
    void Update_Value (long val);             ///< updates the value with val
};


//-----------------------------
// inline function definitions
//-----------------------------

inline int Bound_Domain::Get_Value (long real_val)
// returns the value corresponding to the real value real_val if it exists, -1 otherwise
/// \param[in] real_val the real value for which we want to get the corresponding value in the domain
{
  if (real_val == value)
    return 0;
  else return -1;  
}


inline long Bound_Domain::Get_Real_Value (int num)
// returns the real value corresponding to the value num
/// \param[in] num the number of the wished real value
{
  if (num == 0)
    return value;
  else
    throw ("Invalid index");
}


inline void Bound_Domain::Update_Value (long val)
// updates the value with val
{
  value = val;
}

#endif
