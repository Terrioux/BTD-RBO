/// \file 
/// \brief Definitions related to the Lex_Value_Heuristic class

#ifndef LEX_VALUE_HEURISTIC_H
#define LEX_VALUE_HEURISTIC_H

#include "value_heuristic.h"

class Lex_Value_Heuristic: public Value_Heuristic   /// This class implements the lexicographical value heuristic \ingroup heuristics
{
	public:	
		// constructor
		Lex_Value_Heuristic (CSP * ref_pb);   ///< construct a lexicographical value heuristic for the instance ref_pb
    
		// basic functions
		int Next_Value (Assignment & A, int var, int previous_val = -1) override;	 ///< returns the value of the variable var of the CSP pb which follows the value previous_val in the lexicographical order
};


//-----------------------------
// inline function definitions
//-----------------------------


inline int Lex_Value_Heuristic::Next_Value (Assignment & A, int var, int previous_val)
// returns the value of the variable var of the CSP pb which follows the value previous_val in the lexicographical order
{
	Domain * d = pb->Get_Variable(var)->Get_Domain();
	
	int i = previous_val + 1;
	while (! d->Is_Present (i))
		i++;
		
	return i;
}


#endif
