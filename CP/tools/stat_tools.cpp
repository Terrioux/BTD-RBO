#include "stat_tools.h"
#include <typeinfo>
#include <iostream>

// constraint types
#include "predicate_constraint.h"
#include "positive_nary_extension_constraint.h"
#include "negative_nary_extension_constraint.h"
#include "binary_extension_constraint.h"
#include "sum_global_constraint.h"
#include "weighted_sum_global_constraint.h"
#include "all_equal_global_constraint.h"
#include "all_different_global_constraint.h"
#include "nvalues_global_constraint.h"
#include "count_global_constraint.h"
#include "minimum_global_constraint.h"
#include "maximum_global_constraint.h"
#include "cardinality_global_constraint.h"
#include "lex_global_constraint.h"
#include "ordered_global_constraint.h"
#include "regular_global_constraint.h"
#include "element_global_constraint.h"
#include "circuit_global_constraint.h"
#include "cumulative_global_constraint.h"
#include "no_overlap_global_constraint.h"
#include "length_no_overlap_global_constraint.h"
#include "k_dim_no_overlap_global_constraint.h"
#include "length_k_dim_no_overlap_global_constraint.h"
#include "constant_element_global_constraint.h"
#include "constant_index_element_global_constraint.h"
#include "index_element_global_constraint.h"
#include "positive_compact_table_extension_constraint.h"
#include "short_positive_compact_table_extension_constraint.h"
#include "negative_compact_table_extension_constraint.h"
#include "binary_constraint.h"
#include <typeinfo>

void Print_Constraint_Distribution (CSP * pb, string starter)
// prints the number of constraints of each type
{
	int nb_short_positive_table_extension_constraint = 0;
	int nb_positive_table_extension_constraint = 0;
	int nb_negative_table_extension_constraint = 0;
	int nb_binary_extension_constraint = 0;
	int nb_positive_nary_extension_constraint = 0;
	int nb_negative_nary_extension_constraint = 0;
	int nb_predicate_constraint = 0;
	int nb_sum_global_constraint = 0;
	int nb_weighted_sum_global_constraint = 0;
	int nb_element_global_constraint = 0;
	int nb_alldiff_global_constraint = 0;
	int nb_allequal_global_constraint = 0;
	int nb_other_global_constraint = 0;
  int nb_binary_constraint = 0;
  int nb_nvalues_global_constraint = 0;
  int nb_minimaxi_global_constraint = 0;
  int nb_count_global_constraint = 0;
  int nb_lex_global_constraint = 0;
  int nb_gcc_global_constraint = 0;
  int nb_ordered_global_constraint = 0;
  int nb_regular_global_constraint = 0;
  int nb_circuit_global_constraint = 0;
  int nb_cumulative_global_constraint = 0;
  int nb_no_overlap_global_constraint = 0;
	
	for (unsigned int i = 0; i < pb->Get_M(); i++)
	{
		Constraint * c = pb->Get_Constraint(i);
    
    if (dynamic_cast<Short_Positive_Compact_Table_Extension_Constraint*>(c) != 0)
			nb_short_positive_table_extension_constraint++;
    else
      if (dynamic_cast<Positive_Compact_Table_Extension_Constraint*>(c) != 0)
        nb_positive_table_extension_constraint++;
      else
        if (dynamic_cast<Negative_Compact_Table_Extension_Constraint*>(c) != 0)
          nb_negative_table_extension_constraint++;
        else
          if (dynamic_cast<Binary_Extension_Constraint*>(c) != 0)
            nb_binary_extension_constraint++;
          else
            if (dynamic_cast<Positive_Nary_Extension_Constraint*>(c) != 0)
              nb_positive_nary_extension_constraint++;
            else
              if (dynamic_cast<Negative_Nary_Extension_Constraint*>(c) != 0)
                nb_negative_nary_extension_constraint++;
              else
                if (dynamic_cast<Predicate_Constraint*>(c) != 0)
                  nb_predicate_constraint++;
                else
                  if ((dynamic_cast<Sum_Global_Constraint*>(c) != 0) && (dynamic_cast<Weighted_Sum_Global_Constraint*>(c) == 0))
                    nb_sum_global_constraint++;
                  else
                    if (dynamic_cast<Weighted_Sum_Global_Constraint*>(c) != 0)
                      nb_weighted_sum_global_constraint++;
                    else
                      if (dynamic_cast<All_Different_Global_Constraint*>(c) != 0)
                        nb_alldiff_global_constraint++;
                      else
                        if (dynamic_cast<All_Equal_Global_Constraint*>(c) != 0)
                          nb_allequal_global_constraint++;
                        else
                          if ((dynamic_cast<Element_Global_Constraint*>(c) != 0) || (dynamic_cast<Constant_Element_Global_Constraint*>(c) != 0) || (dynamic_cast<Index_Element_Global_Constraint*>(c) != 0) || (dynamic_cast<Constant_Index_Element_Global_Constraint*>(c) != 0))
                            nb_element_global_constraint++;
                          else
                            if (dynamic_cast<NValues_Global_Constraint*>(c) != 0)
                              nb_nvalues_global_constraint++;
                            else
                              if ((dynamic_cast<Minimum_Global_Constraint*>(c) != 0) || (dynamic_cast<Maximum_Global_Constraint*>(c) != 0))
                                nb_minimaxi_global_constraint++;
                              else
                                if (dynamic_cast<Count_Global_Constraint*>(c) != 0)
                                  nb_count_global_constraint++;
                                else
                                  if (dynamic_cast<Lex_Global_Constraint*>(c) != 0)
                                    nb_lex_global_constraint++;
                                  else
                                    if (dynamic_cast<Cardinality_Global_Constraint*>(c) != 0)
                                      nb_gcc_global_constraint++;
                                    else
                                      if (dynamic_cast<Ordered_Global_Constraint*>(c) != 0)
                                        nb_ordered_global_constraint++;
                                      else
                                        if (dynamic_cast<Regular_Global_Constraint*>(c) != 0)
                                          nb_regular_global_constraint++;
                                        else
                                          if (dynamic_cast<Circuit_Global_Constraint*>(c) != 0)
                                            nb_circuit_global_constraint++;
                                          else
                                            if (dynamic_cast<Cumulative_Global_Constraint *>(c) != 0)
                                              nb_cumulative_global_constraint++;
                                            else
                                              if ((dynamic_cast<No_Overlap_Global_Constraint*>(c) != 0) || (dynamic_cast<Length_No_Overlap_Global_Constraint*>(c) != 0) || (dynamic_cast<K_Dim_No_Overlap_Global_Constraint*>(c) != 0) || (dynamic_cast<Length_K_Dim_No_Overlap_Global_Constraint*>(c) != 0))
                                                nb_no_overlap_global_constraint++;
                                              else
                                                if (dynamic_cast<Global_Constraint*>(c) != 0)
                                                  nb_other_global_constraint++;
                                                else
                                                  if (dynamic_cast<Binary_Constraint*>(c) != 0)
                                                    nb_binary_constraint++;
	}
	
	cout << starter << "# Short_Positive Compact Table Extension Constraint: " << nb_short_positive_table_extension_constraint << endl;
	cout << starter << "# Positive Compact Table Extension Constraint: " << nb_positive_table_extension_constraint << endl;
	cout << starter << "# Negative Compact Table Extension Constraint: " << nb_negative_table_extension_constraint << endl;
	cout << starter << "# Binary Extension Constraint: " << nb_binary_extension_constraint << endl;
	cout << starter << "# Positive Nary Extension Constraint: " << nb_positive_nary_extension_constraint << endl;
	cout << starter << "# Negative Nary Extension Constraint: " << nb_negative_nary_extension_constraint << endl;
	cout << starter << "# Predicate Constraint: " << nb_predicate_constraint << endl;
	cout << starter << "# All-diff Constraint: " << nb_alldiff_global_constraint << endl;
	cout << starter << "# All-equal Constraint: " << nb_allequal_global_constraint << endl;
	cout << starter << "# Element Global Constraint: " << nb_element_global_constraint << endl;	
	cout << starter << "# nValues Global Constraint: " << nb_nvalues_global_constraint << endl;	
	cout << starter << "# Count Global Constraint: " << nb_count_global_constraint << endl;	
	cout << starter << "# Minimum/Maximum Global Constraint: " << nb_minimaxi_global_constraint << endl;	
  cout << starter << "# Sum Global Constraint: " << nb_sum_global_constraint << endl;
	cout << starter << "# Weighted Sum Global Constraint: " << nb_weighted_sum_global_constraint << endl;
	cout << starter << "# Lex Global Constraint: " << nb_lex_global_constraint << endl;
	cout << starter << "# Ordered Global Constraint: " << nb_ordered_global_constraint << endl;
	cout << starter << "# Cardinality Global Constraint: " << nb_gcc_global_constraint << endl;
	cout << starter << "# Regular Global Constraint: " << nb_regular_global_constraint << endl;
	cout << starter << "# Circuit Global Constraint: " << nb_circuit_global_constraint << endl;
	cout << starter << "# Cumulative Global Constraint: " << nb_cumulative_global_constraint << endl;
	cout << starter << "# No Overlap Global Constraint: " << nb_no_overlap_global_constraint << endl;

	cout << starter << "# Other Global Constraint: " << nb_other_global_constraint << endl;	
	cout << starter << "# Special Binary Constraint: " << nb_binary_constraint << endl;	
}
