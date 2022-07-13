#include "no_support.h"

//---------------------------
// constuctor and destructor
//---------------------------

No_Support::No_Support (CSP * pb)
// constructs an empty support
{
	support = 0;
	m = pb->Get_M();
}
