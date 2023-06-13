/// \file 
/// \brief Definitions related to the Set_Domain class

#ifndef SET_DOMAIN_H
#define SET_DOMAIN_H

#include "domain.h"


class Set_Domain: public Domain    /// This class implements enumerated domains with event management \ingroup core
{
	protected :
		vector<long> real_value;  		 					  ///< maps each value to its real value (useful for some predicate or global constraints)
		map<long,int> real_value_to_value;			  ///< maps each real value to its value (useful for some global constraints)

	public :
		// constructor
    Set_Domain (Event_Manager * evm, unsigned int num_var, set<long> & value_set);   		///< constructs a domain defined by the set value_set
		
		// basic functions	
    int Get_Value (long real_val) override;	  ///< returns the value corresponding to the real value real_val if it exists, -1 otherwise
    long Get_Real_Value (int num) override;		///< returns the real value corresponding to the value num
};


//-----------------------------
// inline function definitions
//-----------------------------

inline int Set_Domain::Get_Value (long real_val)
// returns the value corresponding to the real value real_val if it exists, -1 otherwise
/// \param[in] real_val the real value for which we want to get the corresponding value in the domain
{
	map<long,int>::iterator iter = real_value_to_value.find (real_val);
	if (iter == real_value_to_value.end())
		return -1;
	else return iter->second;

}


inline long Set_Domain::Get_Real_Value (int num)
// returns the real value corresponding to the value num
/// \param[in] num the number of the wished real value
{
	return real_value[num];
}


#endif
