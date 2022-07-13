#include "expression_tools.h"
#include "rewrite_tools.h"

// constraint types
#include "predicate_constraint.h"

// rewriting rules
#include "predicate_to_absolute_distance_rewriting_rule.h"
#include "predicate_to_binary_sum_rewriting_rule.h"
#include "predicate_to_no_overlap_rewriting_rule.h"
#include "predicate_to_absolute_rewriting_rule.h"
#include "predicate_to_equal_distance_rewriting_rule.h"
#include "predicate_to_product_rewriting_rule.h"
#include "predicate_to_all_different_rewriting_rule.h"
#include "predicate_to_interval_rewriting_rule.h"
#include "predicate_to_sum_rewriting_rule.h"
#include "predicate_to_all_equal_rewriting_rule.h"
#include "predicate_to_minimal_frequency_rewriting_rule.h"



void Replace_Predicate_By_Global_Constraint (CSP * pb)
// replaces some predicate constraints by global constraints
{	
  vector<Rewriting_Rule *> rules;
  
	for (unsigned int i = 0; i < pb->Get_M(); i++)
	{
		Constraint * c = pb->Get_Constraint (i);

		if (dynamic_cast<Predicate_Constraint *>(c) != 0)
		{
      if (rules.size() == 0)
      {
        rules.push_back(new Predicate_To_Sum_Rewriting_Rule);
        rules.push_back(new Predicate_To_All_Different_Rewriting_Rule);
        rules.push_back(new Predicate_To_All_Equal_Rewriting_Rule);
        rules.push_back(new Predicate_To_No_Overlap_Rewriting_Rule);
        rules.push_back(new Predicate_To_Product_Rewriting_Rule);
        rules.push_back(new Predicate_To_Minimal_Frequency_Rewriting_Rule);
        rules.push_back(new Predicate_To_Interval_Rewriting_Rule);
        rules.push_back(new Predicate_To_Equal_Distance_Rewriting_Rule);
        rules.push_back(new Predicate_To_Absolute_Distance_Rewriting_Rule);
        rules.push_back(new Predicate_To_Absolute_Rewriting_Rule);
        rules.push_back(new Predicate_To_Binary_Sum_Rewriting_Rule);
      }
      
      vector<Rewriting_Rule *>::iterator iter = rules.begin();
      while ((iter != rules.end()) && (! (*iter)->Rewrite(pb,i)))
        iter++;
		}
	}
  
  for (vector<Rewriting_Rule *>::iterator iter = rules.begin(); iter != rules.end(); iter++)
    delete *iter;
}
