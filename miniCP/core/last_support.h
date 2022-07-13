/// \file 
/// \brief Definitions related to the Last_Support class

#ifndef LAST_SUPPORT_H
#define LAST_SUPPORT_H

#include "support.h"
#include "csp.h"

class Last_Support: public Support      /// This class implements the last supports (cf. AC3.1 / AC2001) \ingroup core
{
	protected:
		CSP * pb;									///< the CSP to which the structure is related 
		bool *** is_support_set;	///< is_support_set[num][x][v] is true if the support (i.e. a tuple) of the variable x for the value v w.r.t. the constraint num is set, false otherwise
	
	public:
		// constuctor and destructor
		Last_Support (CSP * pb_init);																		///< constructs an empty support for each variable x for each value v w.r.t. each constraint num
		Last_Support (CSP * pb_init, Last_Support * ls);								///< constructs for each variable x for each value v w.r.t. each constraint num by copying the supports of ls
		~Last_Support ();																								///< destructor
	
		// basic functions
		void Resize (); 																											///< resizes the data structure
		void Reset_Support (int num, int pos, int val); 											///< resets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
		void Change_Support (int num, int pos, int val, int * t, int arity); 	///< sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
		int * Get_Support (int num, int pos, int val);  			 								///< returns a reference to the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
		bool Continue_From_Scratch (); 																			  ///< returns true if the search for a support must continue from scratch, false if it can continue from the next tuple
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Last_Support::Reset_Support (int num, int pos, int val)
// resets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
{
	is_support_set [num][pos][val] = false;
}

inline void Last_Support::Change_Support (int num, int pos, int val, int * t, int arity)
// sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
{
	if (!is_support_set[num][pos][val])
	{
		is_support_set[num][pos][val] = true;
		if (support[num][pos][val] == 0)
			support[num][pos][val] = new int [arity];
	}
	
	for (int i = 0; i < arity; i++)
		support[num][pos][val][i] = t[i];
}


inline int * Last_Support::Get_Support (int num, int pos, int val)
// returns a reference to the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
{
	if (((unsigned) num < m) && (is_support_set[num] != 0) && (is_support_set[num][pos][val]))
		return support[num][pos][val];
	else return 0;
}


inline bool Last_Support::Continue_From_Scratch ()
// returns true if the search for a support must continue from scratch, false if it can continue from the next tuple
{
	return false;
}
#endif

