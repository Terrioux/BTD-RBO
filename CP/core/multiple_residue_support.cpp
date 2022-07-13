#include "multiple_residue_support.h"
#include <typeinfo>

//---------------------------
// constuctor and destructor
//---------------------------

Multiple_Residue_Support::Multiple_Residue_Support (CSP * pb_init): Residue_Support (pb_init)
// constructs an empty support for each variable x for each value v w.r.t. each constraint num
{
}


Multiple_Residue_Support::Multiple_Residue_Support (CSP * pb_init, Support * ls): Residue_Support (pb_init,ls)
// constructs for each variable x for each value v w.r.t. each constraint num by copying the supports of ls
{
}


Multiple_Residue_Support::~Multiple_Residue_Support ()
// destructor
{
}
