/// \file 
/// \brief Definitions related to the Interval_Domain class

#ifndef INTERVAL_DOMAIN_H
#define INTERVAL_DOMAIN_H

#include "domain.h"


class Interval_Domain: public Domain    /// This class implements enumerated domains with event management \ingroup core
{
	protected :
    int value_min;      ///< the smallest value of the interval
    int value_max;      ///< the largest value of the interval

	public :
		// constructor
    Interval_Domain (Event_Manager * evm, unsigned int num_var, int a, int b);   		///< constructs a domain defined by the values of [a,b]
		
		// basic functions	
    int Get_Value (int real_val) override;	  ///< returns the value corresponding to the real value real_val if it exists, -1 otherwise
    int Get_Real_Value (int num) override;		///< returns the real value corresponding to the value num
};


//-----------------------------
// inline function definitions
//-----------------------------

inline int Interval_Domain::Get_Value (int real_val)
// returns the value corresponding to the real value real_val if it exists, -1 otherwise
/// \param[in] real_val the real value for which we want to get the corresponding value in the domain
{
  if ((value_min <= real_val) && (real_val <= value_max))
    return real_val - value_min;
  else return -1;  
}


inline int Interval_Domain::Get_Real_Value (int num)
// returns the real value corresponding to the value num
/// \param[in] num the number of the wished real value
{
	return value_min+num;
}


#endif
