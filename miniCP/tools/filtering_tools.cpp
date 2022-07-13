#include "filtering_tools.h"

// constraint selection header files
#include "all_constraint_selection.h"

// last support header files
#include "last_support.h"
#include "no_support.h"
#include "residue_support.h"
#include "multiple_residue_support.h"

// AC data structure header files
#include "filtering_data_structure.h"

// AC header files
#include "ac.h"


// deletion_stack header files
#include "no_deletion_stack.h"
#include "basic_deletion_stack.h"
#include "support_deletion_stack.h"


AC * AC_initialization (CSP * pb)
// initialization of AC filtering
{
	return new AC (pb,new Multiple_Residue_Support (pb),new All_Constraint_Selection,new Filtering_Data_Structure<int>);
}

