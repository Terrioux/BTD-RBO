#include "geometric_sequence.h"


//-------------
// constructor 
//-------------


Geometric_Sequence::Geometric_Sequence (unsigned long first_term, double sc_factor)
// constructs a geometric sequence based on the factor sc_factor and the first term first_term
{
  if (sc_factor < 1) throw "Backtrack_Restart_Policy error: the scaling factor must be greater than or equal to 1";
  initial_term = first_term;
  current_term = first_term;
  scaling_factor = sc_factor;
}


