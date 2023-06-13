#include "cop.h"
#include "load_XCSP3Callbacks.h"
#include "predicate_constraint.h"
#include "binary_extension_constraint.h"
#include "positive_nary_extension_constraint.h"
#include "negative_nary_extension_constraint.h"

#include "less_weighted_sum_global_constraint.h"
#include "greater_weighted_sum_global_constraint.h"
#include "equal_weighted_sum_global_constraint.h"
#include "not_equal_weighted_sum_global_constraint.h"
#include "in_weighted_sum_global_constraint.h"

#include "less_sum_global_constraint.h"
#include "greater_sum_global_constraint.h"
#include "equal_sum_global_constraint.h"
#include "not_equal_sum_global_constraint.h"
#include "in_sum_global_constraint.h"
#include "all_equal_global_constraint.h"
#include "minimum_global_constraint.h"
#include "maximum_global_constraint.h"
#include "nvalues_global_constraint.h"

#include "constant_element_global_constraint.h"
#include "constant_index_element_global_constraint.h"
#include "element_global_constraint.h"
#include "index_element_global_constraint.h"

#include "no_overlap_global_constraint.h"

#include "all_different_global_constraint.h"
#include "all_different_except_global_constraint.h"
#include "positive_compact_table_extension_constraint.h"
#include "short_positive_compact_table_extension_constraint.h"
#include "negative_compact_table_extension_constraint.h"

#include "equal_binary_constraint.h"

#include "expression_objective_function.h"
#include "maximum_objective_function.h"
#include "minimum_objective_function.h"
#include "nvalue_objective_function.h"
#include "sum_objective_function.h"


#include "expression_tools.h"

#include <climits>
#include <list>

//----------------------------
// constructor and destructor
//----------------------------


Load_XCSP3Callbacks::Load_XCSP3Callbacks(CSP * pb_ref, bool classical_constraint) : XCSP3CoreCallbacks() 
// constructor
{
	pb = pb_ref;
  use_classical_extension_constraint = classical_constraint;
  recognizeSpecialIntensionCases = false;
}


//-----------------
// basic functions
//-----------------


void Load_XCSP3Callbacks::beginInstance(InstanceType type)
// checks the type of instance
{
  if ((dynamic_cast<COP*>(pb) != 0) && (type == 0))
    throw ("Error: the selected instance is not a COP instance");
}


template<class T>
void displayList(vector<T> &list, string separator = " ") {
    if (list.size() > 8) 
    {
      for(int i = 0 ; i < 3 ; i++)
        cout << list[i] << separator;
      cout << " ... ";
      for(unsigned int i = list.size() - 4 ; i < list.size() ; i++)
        cout << list[i] << separator;
      cout << endl;
      return;
    }
    for(unsigned int i = 0 ; i < list.size() ; i++)
        cout << list[i] << separator;
    cout << endl;
}


void displayList(vector<XVariable *> &list, string separator = " ") {
    if(list.size() > 8) {
        for(int i = 0 ; i < 3 ; i++)
            cout << list[i]->id << separator;
        cout << " ... ";
        for(unsigned int i = list.size() - 4 ; i < list.size() ; i++)
            cout << list[i]->id << separator;
        cout << endl;
        return;
    }
    for(unsigned int i = 0 ; i < list.size() ; i++)
        cout << list[i]->id << separator;
    cout << endl;
}


void Load_XCSP3Callbacks::buildVariableInteger(string id, int minValue, int maxValue) 
// creates an integer variable whose domain contains the integer values between minValue and Maxvalue
{
	pb->Add_Variable (minValue,maxValue,id);
}


void Load_XCSP3Callbacks::buildVariableInteger(string id, vector<int> &values) 
// creates an integer variable whose domain contains the integer values in vector values
{
  set<long> value_set;
  
	for(unsigned int i = 0; i < values.size() ; i++)
		value_set.insert (values[i]);
		
	pb->Add_Variable (value_set,id);
}


void generate_tuples (Extension_Constraint * c, vector<Variable *> scope, int t [], bool is_star [], bool support)
{
  int arity = scope.size();
  int pos = arity-1;

  while (! is_star[pos])
    pos--;

  while (pos >= 0)
  {
    // we add the current tuple
    if (support)
      c->Allow_Tuple (t);
    else c->Forbid_Tuple (t);
    
    // we compute the next tuple    
    while ((pos >= 0) && ((! is_star[pos]) || (t[pos] == scope[pos]->Get_Domain()->Get_Max())))
      pos--;
    
    if (pos >= 0)
    {
      t[pos] = scope[pos]->Get_Domain()->Next_Value(t[pos]);
      pos++;
      while (pos < arity)
      {
        if (is_star[pos])
          t[pos] = scope[pos]->Get_Domain()->Get_Min();
        pos++;
      }
      
      do
      {
        pos--;
      }
      while (! is_star[pos]);
    }
  }
}


void Tree_to_Predicate (CSP * pb, string id, Tree * tree, vector<Variable *> & scope_var)
{
  // we get the string representing the variables
  vector<string> scope_str;

  tree->get_scope(scope_str);
  
  // we compute the scope and the minimum and maximum values of each domain
  vector<Variable *> scope;
  map<string,int> min, max;

      for (vector<string>::iterator iter = scope_str.begin(); iter != scope_str.end(); iter++)
      {
        Variable * x = pb->Get_Variable(*iter);
        scope.push_back (x);
        
        Domain * d = x->Get_Domain();
        min.insert (make_pair(*iter,d->Get_Real_Value(d->Get_Min())));
        max.insert (make_pair(*iter,d->Get_Real_Value(d->Get_Max())));
      }
      
      // we create new intermediary variable
      string id_var = id+"_"+to_string (pb->Get_N());
      
      Variable * x = pb->Add_Variable (tree->get_min(min,max),tree->get_max(min,max),id_var,false,true);
      
      scope.push_back (x);
      
      scope_var.push_back (x);
      
      string expr = "eq(" + tree->get_expr() + "," + id_var +")";
      Expression relation = string_to_expression (expr,pb,scope);

  pb->Add_Constraint (new Predicate_Constraint(scope,relation),false);  
}


void convert_variable_list (CSP * pb, vector<XVariable *> &xlist, vector<Variable*> &list)
// converts a list of XVariables to a list of Variables
{
	for (vector<XVariable*>::iterator iter = xlist.begin(); iter != xlist.end(); iter++)
		list.push_back (pb->Get_Variable((*iter)->id));
}

void convert_int_list (CSP * pb, vector<int> & ilist, string id, vector<Variable*> &list)
// converts a list of integers to a list of auxiliary variables
{
	for (vector<int>::iterator iter = ilist.begin(); iter != ilist.end(); iter++)
    list.push_back (pb->Add_Variable(*iter,*iter,id,true));
}


void convert_trees (CSP * pb, string id, vector<Tree *> &trees, vector<Variable*> &list)
// converts a list of trees to a list of Variables
{
  for(unsigned int i = 0; i < trees.size(); i++) 
    Tree_to_Predicate (pb, id, trees[i], list);
}



void Load_XCSP3Callbacks::buildConstraintExtension(string id, vector<XVariable *> list_var, vector<vector<int>> &tuples, bool support, bool hasStar) 
// creates an extension constraint where list is the scope, tuples the list of tuples which are allowad if support is set to true, forbidden otherwise (hasStar is true if * is allowed in tuples)
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list_var,scope);
	
	// we create the constraint
	Extension_Constraint * c;
	unsigned int arity = list_var.size();
	
  if (use_classical_extension_constraint)
  {
    if (list_var.size() == 2)
      c = new Binary_Extension_Constraint (scope,! support);
    else
      {
        if (support)
          c = new Positive_Nary_Extension_Constraint(scope);
        else c = new Negative_Nary_Extension_Constraint(scope);
      }
  }
  else
    {
      if (support)
      {
        if (hasStar)
          c = new Short_Positive_Compact_Table_Extension_Constraint (scope);
        else c = new Positive_Compact_Table_Extension_Constraint (scope);
      }
      else c = new Negative_Compact_Table_Extension_Constraint(scope);
    }
  
	int t [arity];

	last_tuples.clear();

  if (hasStar)
  {
    bool is_star [arity];
    int star = INT_MAX;
    bool is_star_tuple;
    for (vector<vector<int>>::iterator iter = tuples.begin(); iter != tuples.end(); iter++)
    {
      int i = 0;
      is_star_tuple = false;

      for (vector<int>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
      {
        if (*iter2 == star)
        {
          if ((use_classical_extension_constraint) || (!support))
          {
            is_star[i] = true;
            is_star_tuple = true;
            t[i] = scope[i]->Get_Domain()->Get_Min();
          }
          else t[i] = star;
        }
        else 
          {
            is_star[i] = false;
              
            t[i] = scope[i]->Get_Domain()->Get_Value ((*iter)[i]);
            
            if (t[i] == -1)
              throw ("Error: unknown value");
          }
        i++;
      }
      
      if (is_star_tuple)
        generate_tuples (c,scope,t,is_star,support);
      else
        if (support)
          c->Allow_Tuple (t);
        else c->Forbid_Tuple (t);
    
      // we save the tuples for the case where a similar constraint is defined after the current constraint
      last_tuples.push_back (*iter);
    } 
  }
  else
    {
      for (vector<vector<int>>::iterator iter = tuples.begin(); iter != tuples.end(); iter++)
      {
        for (unsigned int i = 0; i < arity; i++)
        {
          t[i] = scope[i]->Get_Domain()->Get_Value ((*iter)[i]);
            
          if (t[i] == -1)
            throw ("Error: unknown value");
        }

        if (support)
          c->Allow_Tuple (t);
        else c->Forbid_Tuple (t);
      
        // we save the tuples for the case where a similar constraint is defined after the current constraint
        last_tuples.push_back (*iter);
      }
    }

	pb->Add_Constraint (c,false);
}



void Filter_Domain (Variable * x, vector<int> &tuples, bool support)
// removes values forbidden by a unary extension constraint 
{
	for (unsigned int i = 0; i < x->Get_Domain()->Get_Initial_Size(); i++)
	{
		if (x->Get_Domain()->Is_Present(i))
		{
			vector<int>::iterator iter = tuples.begin(); 
			while ((iter != tuples.end()) && (x->Get_Domain()->Get_Real_Value(i) != *iter))
				iter++;
				
			if (iter == tuples.end())
			{
				// the value i does not belong to the vector tuples
				if (support)
					x->Get_Domain()->Filter_Value(i);
			}			
			else
				{
					// the value i belongs to the vector tuples
					if (! support)
						x->Get_Domain()->Filter_Value(i);
				}
		}	
	}
}

void Load_XCSP3Callbacks::buildConstraintExtension(string id, XVariable *variable, vector<int> &tuples, bool support, bool hasStar)
// creates an extension constraint where variable is the single variable in the scope, tuples the list of tuples which are allowad if support is set to true, forbidden otherwise (hasStar is true if * is allowed in tuples)
{
  if (hasStar)
    throw ("Error: impossible hasStar value");

	// we first consider the scope of the constraint
	Variable * x;
	
	x = pb->Get_Variable(variable->id);

	
	// we remove forbidden values
	Filter_Domain (x, tuples,support);

  if (x->Get_Domain()->Get_Size() == 0)
    pb->Set_Conflict (x,0);	
	
	// we save the tuples for the case where a similar constraint is defined after the current constraint
	last_tuples.clear();
	last_tuples.push_back(tuples);	
}


void Load_XCSP3Callbacks::buildConstraintExtensionAs(string id, vector<XVariable *> list, bool support, bool hasStar) 
// creates an extension constraint where list is the scope, the list of tuples which are allowad if support is set to true, forbidden otherwise, is the same as the previous extension constraint (hasStar is true if * is allowed in tuples)
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list,scope);
	
	// we create the constraint
	Extension_Constraint * c;
	unsigned int arity = list.size();
	
	if (list.size() == 1)
		Filter_Domain (scope[0], last_tuples[0],support);
	else
		{
      if (use_classical_extension_constraint)
      {
        if (list.size() == 2)
          c = new Binary_Extension_Constraint (scope,! support);
        else
          {
            if (support)
              c = new Positive_Nary_Extension_Constraint(scope);
            else c = new Negative_Nary_Extension_Constraint(scope);
          }
      }
      else
        {
          if (support)
          {
            if (hasStar)
              c = new Short_Positive_Compact_Table_Extension_Constraint (scope);
            else c = new Positive_Compact_Table_Extension_Constraint (scope);
          }
          else c = new Negative_Compact_Table_Extension_Constraint(scope);
        }

			int t [arity];

      if (hasStar)
      {
        bool is_star [arity];
        int star = INT_MAX;
        bool is_star_tuple;
        for (vector<vector<int>>::iterator iter = last_tuples.begin(); iter != last_tuples.end(); iter++)
        {
          int i = 0;
          is_star_tuple = false;

          for (vector<int>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
          {
            if (*iter2 == star)
            {
              if ((use_classical_extension_constraint) || (!support))
              {
                is_star[i] = true;
                is_star_tuple = true;
                t[i] = scope[i]->Get_Domain()->Get_Min();
              }
              else t[i] = star;
            }
            else 
              {
                is_star[i] = false;
              
                t[i] = scope[i]->Get_Domain()->Get_Value ((*iter)[i]);
                
                if (t[i] == -1)
                  throw ("Error: unknown value");
              }
            i++;
          }
          
          if (is_star_tuple)
            generate_tuples (c,scope,t,is_star,support);
          else
            if (support)
              c->Allow_Tuple (t);
            else c->Forbid_Tuple (t);
        } 
      }
      else
        {
          for (vector<vector<int>>::iterator iter = last_tuples.begin(); iter != last_tuples.end(); iter++)
          {
            for (unsigned int i = 0; i < arity; i++)
            {
              t[i] = scope[i]->Get_Domain()->Get_Value ((*iter)[i]);
            
              if (t[i] == -1)
                throw ("Error: unknown value");
            }

            if (support)
              c->Allow_Tuple (t);
            else c->Forbid_Tuple (t);
          }
        }

			pb->Add_Constraint (c,false); 
	}
}


void Load_XCSP3Callbacks::buildConstraintIntension(string id, string expr) 
// creates an intension constraint defined by the expression expr represented by a string
{
  vector<Variable *> scope;
  Expression relation = string_to_expression (expr,pb,scope);
  
  Predicate_Constraint * c = new Predicate_Constraint (scope, relation);
  
  if (scope.size() == 1)
  {
    for (int i = 0; (unsigned) i < scope[0]->Get_Domain()->Get_Initial_Size(); i++)
      if ((scope[0]->Get_Domain()->Is_Present(i)) && (! c->Is_Satisfied(&i)))
        scope[0]->Get_Domain()->Filter_Value(i);
    
    if (scope[0]->Get_Domain()->Get_Size() == 0)
      pb->Set_Conflict (scope[0],0);
    
    delete c;
  }
  else pb->Add_Constraint (c,false);
}


void Load_XCSP3Callbacks::buildConstraintIntension(string id, Tree *tree) 
// creates an intension constraint defined by the expression tree  represented by a tree
{
  buildConstraintIntension(id, tree->get_expr());
}


void Load_XCSP3Callbacks::buildConstraintPrimitive(string id, OrderType op, XVariable *x, int k, XVariable *y) 
// creates an intension constraint defined by the expression x + k op y
{
  string expression;

  switch (op)
  {
		case XCSP3Core::LT: expression = "lt"; break;
		case XCSP3Core::LE: expression = "le"; break;
		case XCSP3Core::GT: expression = "gt"; break;
		case XCSP3Core::GE: expression = "ge"; break;
		case XCSP3Core::EQ: expression = "eq"; break;
		case XCSP3Core::NE: expression = "ne"; break;
		default: throw ("Error: operator not allowed");
  }
  
  expression += "(sub("+x->id+","+y->id+"),"+to_string(-k)+")";

	vector<Variable *> scope;
	Expression relation = string_to_expression (expression,pb,scope);
	
	pb->Add_Constraint (new Predicate_Constraint (scope, relation),false);
}


void Load_XCSP3Callbacks::buildConstraintPrimitive(string id, OrderType op, XVariable *x, int k) 
// creates an intension constraint defined by the expression x op k where op is <= or >=
{
  Variable * var = pb->Get_Variable(x->id);
  
  Domain * d = var->Get_Domain();
  int v;
  
  switch (op)
  {
		case XCSP3Core::LE:     // x <= k
        v = d->Get_Max();
        while ((v != -1) && (d->Get_Real_Value(v) > k))
        {
          d->Filter_Value (v);
          v = d->Previous_Value(v);
        }
        break;
		case XCSP3Core::GE:     // x >= k
        v = d->Get_Min();
        while ((v != -1) && (d->Get_Real_Value(v) < k))
        {
          d->Filter_Value (v);
          v = d->Next_Value(v);
        }
        break;
		default: throw ("Error: operator not allowed");
  }
}

void Load_XCSP3Callbacks::buildConstraintPrimitive(string id, XVariable *x, bool in, int min, int max)
// creates an intension constraint defined by the expression x belongs (resp. does not belong) to [min,max] if in is set to true (resp. false)
{
  Variable * var = pb->Get_Variable(x->id);
  
  Domain * d = var->Get_Domain();
  int v;
  
  if (in)
  {
    // x must belong to [min,max]
    
    v = d->Get_Min();
    while ((v != -1) && (d->Get_Real_Value(v) < min))
    {
      d->Filter_Value (v);
      v = d->Next_Value(v);
    }

    v = d->Get_Max();
    while ((v != -1) && (d->Get_Real_Value(v) > max))
    {
      d->Filter_Value (v);
      v = d->Previous_Value(v);
    }
  }
  else
    {
      // x must not belong to [min,max]
      
      v = d->Get_Min();
      while ((v != -1) && (d->Get_Real_Value(v) < min))
        v = d->Next_Value(v);

      while ((v != -1) && (d->Get_Real_Value(v) <= max))
      {
        d->Filter_Value (v);
        v = d->Next_Value(v);
      }
    }

}
    

void Load_XCSP3Callbacks::buildConstraintAlldifferent(string id, vector<XVariable *> &list)
// creates an all-diff global constraint
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list,scope);
	
	// we create the constraint
	pb->Add_Constraint (new All_Different_Global_Constraint (scope),false);
}


void Load_XCSP3Callbacks::buildConstraintAlldifferentExcept(string id, vector<XVariable *> &list, vector<int> &except)
{
  if (except.size() != 1)
    throw ("Error: a single value is allowed as an exception value for an allDifferent with exception");
  
	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list,scope);
	
	// we create the constraint
	pb->Add_Constraint (new All_Different_Except_Global_Constraint (scope,except[0]),false);
}




void Load_XCSP3Callbacks::buildConstraintAlldifferent(string id, vector<Tree *> &list)
// creates an all-diff global constraint over expressions
{
  vector<Variable *> scope;
  
  // we create an intermediary variable and constraint for each tree
  for(unsigned int i = 0; i < list.size(); i++) 
    Tree_to_Predicate (pb, id, list[i], scope);
    
  // we create the all-diff constraint
  pb->Add_Constraint (new All_Different_Global_Constraint (scope),false);
}

//~ void Load_XCSP3Callbacks::buildConstraintAlldifferentList(string id, vector<vector<XVariable *> > &lists) {
    //~ cout << "\n    allDiff list constraint" << id << endl;
    //~ for(unsigned int i = 0 ; i < (lists.size() < 4 ? lists.size() : 3) ; i++) {
        //~ cout << "        ";
        //~ displayList(lists[i]);

    //~ }
//~ }


void Load_XCSP3Callbacks::buildConstraintAlldifferentMatrix(string id, vector<vector<XVariable *> > &matrix) 
// creates a matrix version of the all-diff global constraint
{
    //~ cout << "\n    allDiff matrix constraint" << id << endl;
    //~ for(unsigned int i = 0 ; i < matrix.size() ; i++) {
        //~ cout << "        ";
        //~ displayList(matrix[i]);
    //~ }

	// we add an all-diff global constraint for each row and each column consider the scope of the constraint
	
	vector<vector<Variable *>> scope_col (matrix[0].size());

  for (vector<vector<XVariable *>>::iterator iter_vec = matrix.begin(); iter_vec != matrix.end(); iter_vec++)
  {
    vector<Variable *> scope_row;
    int col = 0;
    for (vector<XVariable *>::iterator iter = iter_vec->begin(); iter != iter_vec->end(); iter++)
    {
      Variable * x = pb->Get_Variable((*iter)->id); 
   		scope_row.push_back (x);
      scope_col[col].push_back (x);
      col++;
    }

    // we create the constraint corresponding to the current row
    pb->Add_Constraint (new All_Different_Global_Constraint (scope_row),false);
  }
	
  // we create the constraint corresponding to each column
  for (unsigned int i = 0; i < matrix[0].size(); i++)
    pb->Add_Constraint (new All_Different_Global_Constraint (scope_col[i]),false);

}


void Load_XCSP3Callbacks::buildConstraintAllEqual(string id, vector<XVariable *> &list)
// creates an all-equal global constraint whose scope is provided by list
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list,scope);
	
	// we create the constraint
	pb->Add_Constraint (new All_Equal_Global_Constraint(scope),false);
}


void Load_XCSP3Callbacks::buildConstraintAllEqual(string id, vector<Tree *> &list)
// creates an all-equal global constraint over expressions
{
  vector<Variable *> scope;
  
  // we create an intermediary variable and constraint for each tree
  for(unsigned int i = 0; i < list.size(); i++) 
    Tree_to_Predicate (pb, id, list[i], scope);
    
  // we create the all-equal constraint
  pb->Add_Constraint (new All_Equal_Global_Constraint (scope),false);

}


void Load_XCSP3Callbacks::buildConstraintSum(string id, vector<XVariable *> &list, vector<int> &coeffs, XCondition &cond)
// creates a weighted sum global constraint whose scope is provided by list and for which coeffs corresponds to the list of integer coefficients and cond is the considered condition
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list,scope);
		
	if (cond.operandType == XCSP3Core::VARIABLE)
	{
		scope.push_back (pb->Get_Variable(cond.var));
		coeffs.push_back (-1);
		cond.val = 0;
	}
		
	// we create the constraint
	switch (cond.op)
	{
		case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
		case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
		case XCSP3Core::LT: pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
		case XCSP3Core::LE:	pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
		case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
		case XCSP3Core::GE: pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
		case XCSP3Core::IN: pb->Add_Constraint (new In_Weighted_Sum_Global_Constraint (scope,coeffs,cond.min,cond.max),false); break;
	}
}


void Load_XCSP3Callbacks::buildConstraintSum(string id, vector<XVariable *> &list, XCondition &cond)
// creates a sum global constraint whose scope is provided by list and for which cond is the considered condition
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list,scope);

	if (cond.operandType == XCSP3Core::VARIABLE)
	{
		vector<int> coeffs;
		
		for (vector<XVariable*>::iterator iter = list.begin(); iter != list.end(); iter++)
			coeffs.push_back (1);

		scope.push_back (pb->Get_Variable(cond.var));
		coeffs.push_back (-1);
		cond.val = 0;
				
		// we create the constraint
		switch (cond.op)
		{
      case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
      case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
      case XCSP3Core::LT: pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
      case XCSP3Core::LE:	pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
      case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
      case XCSP3Core::GE: pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
      case XCSP3Core::IN: pb->Add_Constraint (new In_Weighted_Sum_Global_Constraint (scope,coeffs,cond.min,cond.max),false); break;
		}
	}
	else
		{
			// we create the constraint
			switch (cond.op)
			{
				case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Sum_Global_Constraint (scope,cond.val),false); break;
				case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Sum_Global_Constraint (scope,cond.val),false); break;
				case XCSP3Core::LT: pb->Add_Constraint (new Less_Sum_Global_Constraint (scope,true,cond.val),false); break;
				case XCSP3Core::LE:	pb->Add_Constraint (new Less_Sum_Global_Constraint (scope,false,cond.val),false); break;
				case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Sum_Global_Constraint (scope,true,cond.val),false); break;
				case XCSP3Core::GE: pb->Add_Constraint (new Greater_Sum_Global_Constraint (scope,false,cond.val),false); break;
				case XCSP3Core::IN: pb->Add_Constraint (new In_Sum_Global_Constraint (scope,cond.min,cond.max),false); break;
			}
		}
}


Variable * Add_Weighted_Variable (CSP * pb, Variable * y, Variable * z)
// creates an auxiliary variable x s.t. x = y * z for the instance pb
{
  vector<Variable *> scope_mul;
  
  // we create new intermediary variable
  string id_var = "prod_"+y->Get_Name()+"_"+z->Get_Name()+"_"+to_string (pb->Get_N());
  
  scope_mul.push_back(pb->Get_Variable (y->Get_Name()));
  scope_mul.push_back(pb->Get_Variable (z->Get_Name()));
  
  Domain * dv = scope_mul[0]->Get_Domain();
  Domain * dc = scope_mul[1]->Get_Domain();
  
  set<long> values;
  
  for (unsigned int j = 0; j < dv->Get_Initial_Size(); j++)
    for (unsigned int k = 0; k < dc->Get_Initial_Size(); k++)
      values.insert(dv->Get_Real_Value(j) * dc->Get_Real_Value(k));
  
  Variable * x = pb->Add_Variable (values,id_var,false,true);
  
  scope_mul.push_back (x);
  
  // we link the corresponding variables by a product constraint

  Expression expr[2];
  expr[0] = Expression (VAR,0);
  expr[1] = Expression (VAR,1);
  
  Expression expr2[2];
  expr2[0] = Expression (MUL,expr,2);
  expr2[1] = Expression (VAR,2);
  
  Expression e = Expression (EQ,expr2,2);
  
  pb->Add_Constraint (new Predicate_Constraint(scope_mul,e),false);
  
  return x;
}


void Load_XCSP3Callbacks::buildConstraintSum(string id, vector<XVariable *> &list, vector<XVariable *> &coeffs, XCondition &cond)
// creates a weighted sum global constraint whose scope is provided by list and for which coeffs corresponds to the list of variable coefficients and cond is the considered condition
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;

  unsigned int arity = list.size();
  
  for (unsigned int i = 0; i < arity; i++)
    scope.push_back (Add_Weighted_Variable(pb, pb->Get_Variable(list[i]->id), pb->Get_Variable(coeffs[i]->id)));
	
	if (cond.operandType == XCSP3Core::VARIABLE)
	{
		vector<int> coeffs;
		
		for (vector<XVariable*>::iterator iter = list.begin(); iter != list.end(); iter++)
			coeffs.push_back (1);

		scope.push_back (pb->Get_Variable(cond.var));
		coeffs.push_back (-1);
		cond.val = 0;
				
		// we create the constraint
		switch (cond.op)
		{
      case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
      case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
      case XCSP3Core::LT: pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
      case XCSP3Core::LE:	pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
      case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
      case XCSP3Core::GE: pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
      case XCSP3Core::IN: pb->Add_Constraint (new In_Weighted_Sum_Global_Constraint (scope,coeffs,cond.min,cond.max),false); break;
		}
	}
	else
		{
			// we create the constraint
			switch (cond.op)
			{
				case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Sum_Global_Constraint (scope,cond.val),false); break;
				case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Sum_Global_Constraint (scope,cond.val),false); break;
				case XCSP3Core::LT: pb->Add_Constraint (new Less_Sum_Global_Constraint (scope,true,cond.val),false); break;
				case XCSP3Core::LE:	pb->Add_Constraint (new Less_Sum_Global_Constraint (scope,false,cond.val),false); break;
				case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Sum_Global_Constraint (scope,true,cond.val),false); break;
				case XCSP3Core::GE: pb->Add_Constraint (new Greater_Sum_Global_Constraint (scope,false,cond.val),false); break;
				case XCSP3Core::IN: pb->Add_Constraint (new In_Sum_Global_Constraint (scope,cond.min,cond.max),false); break;
			}
		}
}


void Load_XCSP3Callbacks::buildConstraintSum(string id, vector<Tree *> &list, vector<int> &coeffs, XCondition &cond) 
// creates a weighted sum global constraint on expressions represented by trees and for which coeffs corresponds to the list of coefficients and cond is the considered condition
{
  vector<Variable *> scope;
  
  // we create an intermediary variable and constraint for each tree
  for(unsigned int i = 0; i < list.size(); i++) 
    Tree_to_Predicate (pb, id, list[i], scope);
    
  // we create the sum constraint
	if (cond.operandType == XCSP3Core::VARIABLE)
	{
		scope.push_back (pb->Get_Variable(cond.var));
		coeffs.push_back (-1);
		cond.val = 0;
  }
		
  switch (cond.op)
  {
    case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
    case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
    case XCSP3Core::LT: pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
    case XCSP3Core::LE:	pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
    case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
    case XCSP3Core::GE: pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
    case XCSP3Core::IN: pb->Add_Constraint (new In_Weighted_Sum_Global_Constraint (scope,coeffs,cond.min,cond.max),false); break;
  }
}

void Load_XCSP3Callbacks::buildConstraintSum(string id, vector<Tree *> &list, XCondition &cond) 
// creates a sum global constraint on expressions represented by trees and for which cond is the considered condition
{  
  vector<Variable *> scope;
  
  // we create an intermediary variable and constraint for each tree
  for(unsigned int i = 0; i < list.size(); i++) 
    Tree_to_Predicate (pb,id, list[i], scope);    
  
	if (cond.operandType == XCSP3Core::VARIABLE)
	{
		vector<int> coeffs;
		
		for (vector<Variable*>::iterator iter = scope.begin(); iter != scope.end(); iter++)
			coeffs.push_back (1);

		scope.push_back (pb->Get_Variable(cond.var));
		coeffs.push_back (-1);
		cond.val = 0;
				
		// we create the constraint
		switch (cond.op)
		{
      case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
      case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Weighted_Sum_Global_Constraint (scope,coeffs,cond.val),false); break;
      case XCSP3Core::LT: pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
      case XCSP3Core::LE:	pb->Add_Constraint (new Less_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
      case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,true,cond.val),false); break;
      case XCSP3Core::GE: pb->Add_Constraint (new Greater_Weighted_Sum_Global_Constraint (scope,coeffs,false,cond.val),false); break;
      case XCSP3Core::IN: pb->Add_Constraint (new In_Weighted_Sum_Global_Constraint (scope,coeffs,cond.min,cond.max),false); break;
		}
	}
	else
		{
			// we create the constraint
			switch (cond.op)
			{
				case XCSP3Core::EQ: pb->Add_Constraint (new Equal_Sum_Global_Constraint (scope,cond.val),false); break;
				case XCSP3Core::NE: pb->Add_Constraint (new Not_Equal_Sum_Global_Constraint (scope,cond.val),false); break;
				case XCSP3Core::LT: pb->Add_Constraint (new Less_Sum_Global_Constraint (scope,true,cond.val),false); break;
				case XCSP3Core::LE:	pb->Add_Constraint (new Less_Sum_Global_Constraint (scope,false,cond.val),false); break;
				case XCSP3Core::GT:	pb->Add_Constraint (new Greater_Sum_Global_Constraint (scope,true,cond.val),false); break;
				case XCSP3Core::GE: pb->Add_Constraint (new Greater_Sum_Global_Constraint (scope,false,cond.val),false); break;
				case XCSP3Core::IN: pb->Add_Constraint (new In_Sum_Global_Constraint (scope,cond.min,cond.max),false); break;
			}
		}
}


void Create_Relationship (CSP * pb, string id_var1, XCondition &xc, string id_var2="")
{
  string expr;
  switch (xc.op)
  {
    case XCSP3Core::LE: expr = "le("; break;
    case XCSP3Core::LT: expr = "lt("; break;
    case XCSP3Core::GE: expr = "ge("; break;
    case XCSP3Core::GT: expr = "gt("; break;
    case XCSP3Core::IN: expr = "in("; break;
    case XCSP3Core::EQ: expr = "eq("; break;
    case XCSP3Core::NE: expr = "ne("; break;
  }
  
  expr += id_var1 + ",";
  
  if (xc.op == XCSP3Core::IN)
  {
    expr += "set(";
    for (int val = xc.min; val < xc.max; val++)
      expr += to_string(val)+ ","; 
    expr += to_string(xc.max) + "))";
  }
  else expr += id_var2 + ")";

  vector<Variable *> scope;
  Expression e = string_to_expression (expr,pb,scope);
  pb->Add_Constraint(new Predicate_Constraint(scope,e),false);
}


void NValues_Constraint (CSP * pb,string id, vector<XVariable *> &list, XCondition &xc, int except_val, bool with_exception)
{
  // we look for the target variable that is the variable which is equal to nValues(...)
  Variable * target_variable;
  
	if (xc.operandType == XCSP3Core::VARIABLE)
    target_variable = pb->Get_Variable(xc.var);
  else
    if (xc.op != XCSP3Core::IN)
    {
      string id_var = id+"_"+to_string (pb->Get_N());
      
      target_variable =  pb->Add_Variable (xc.val,xc.val,id_var,false,true);
    }

  // we then consider the scope of the constraint
	vector<Variable *> scope;

  if (xc.op == XCSP3Core::EQ)
    scope.push_back(target_variable);
  else
    {
      string id_var = "nvalues_"+id+"_"+to_string (pb->Get_N());

      scope.push_back(pb->Add_Variable (0,list.size(),id_var,false,true));
      
      Create_Relationship (pb, id_var, xc, target_variable->Get_Name());
    }

	for (vector<XVariable*>::iterator iter = list.begin(); iter != list.end(); iter++)
		scope.push_back (pb->Get_Variable((*iter)->id));

  if (with_exception)
    throw ("Error: NValues constraint with exception is not supported");
  else pb->Add_Constraint (new NValues_Global_Constraint (scope),false);
}



void Minimum_Maximum_Constraint (CSP * pb,string id, vector<Variable *> &scope, XCondition &xc, bool is_minimum)
{
  // we look for the target variable that is the variable which is equal to min(...)
  Variable * target_variable;
  
	if (xc.operandType == XCSP3Core::VARIABLE)
    target_variable = pb->Get_Variable(xc.var);
  else
    if (xc.op != XCSP3Core::IN)
    {
      string id_var = id+"_"+to_string (pb->Get_N());
      
      target_variable = pb->Add_Variable (xc.val,xc.val,id_var,false,true);
    }

  // we then consider the scope of the constraint
  if (xc.op == XCSP3Core::EQ)
    scope.insert(scope.begin(),target_variable);
  else
    {
      string id_var = id+"_aux_"+to_string (pb->Get_N());

      set<long> values;
      for (vector<Variable*>::iterator iter = scope.begin(); iter != scope.end(); iter++)
      {
        Domain * d = (*iter)->Get_Domain();
        
        for (int i = d->Get_Size()-1; i >= 0; i--)
          values.insert(d->Get_Remaining_Real_Value(i));
      }
      
      scope.insert(scope.begin(),pb->Add_Variable (values,id_var,false,true));
      
      Create_Relationship (pb, id_var, xc, target_variable->Get_Name());
    }

  if (is_minimum)
    pb->Add_Constraint (new Minimum_Global_Constraint (scope),false);
  else pb->Add_Constraint (new Maximum_Global_Constraint (scope),false);
}
    

void Load_XCSP3Callbacks::buildConstraintMinimum(string id, vector<XVariable *> &list, XCondition &xc) 
// creates a minimum global constraint
{
  // we first consider the scope of the constraint
	vector<Variable *> scope;
  
	convert_variable_list (pb,list,scope);
  
  // we create the constraint
  Minimum_Maximum_Constraint (pb,id,scope,xc, true);
}

void Load_XCSP3Callbacks::buildConstraintMinimum(string id, vector<Tree *> &list, XCondition &xc)
// creates a minimum global constraint over expressions
{
  // we create an intermediary variable and constraint for each tree
  vector<Variable *> scope;
  convert_trees (pb,id,list,scope);
  
  // we create the constraint
  Minimum_Maximum_Constraint (pb,id,scope,xc, true);
}


//~ void Load_XCSP3Callbacks::buildConstraintMinimum(string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank,
                                                 //~ XCondition &xc) {
    //~ cout << "\n    arg_minimum  constraint" << endl;
    //~ cout << "        ";displayList(list);
    //~ cout << "        index:" << *index << endl;
    //~ cout << "        Start index : " << startIndex << endl;
    //~ cout << "        condition: " << xc << endl;

//~ }


void Load_XCSP3Callbacks::buildConstraintMaximum(string id, vector<XVariable *> &list, XCondition &xc) 
// creates a maximum global constraint
{
  // we first consider the scope of the constraint
	vector<Variable *> scope;
  
	convert_variable_list (pb,list,scope);
  
  // we create the constraint
  Minimum_Maximum_Constraint (pb,id,scope,xc, false);
}

void Load_XCSP3Callbacks::buildConstraintMaximum(string id, vector<Tree *> &list, XCondition &xc)
// creates a maximum global constraint over expressions
{
  // we create an intermediary variable and constraint for each tree
  vector<Variable *> scope;
  convert_trees (pb,id,list,scope);

  // we create the constraint
  Minimum_Maximum_Constraint (pb,id,scope,xc, false);
}

//~ void Load_XCSP3Callbacks::buildConstraintMaximum(string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank, XCondition &xc) {
    //~ cout << "\n    arg_maximum  constraint" << endl;
    //~ cout << "        ";displayList(list);
    //~ cout << "        index:" << *index << endl;
    //~ cout << "        Start index : " << startIndex << endl;
    //~ cout << "        condition: " << xc << endl;
//~ }


void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable *> &list,  int value) 
// creates an element global constraint whose scope is provided by list with a constant value provided by value
{
	// we first consider the scope of the constraint
	vector<Variable *> scope;
  
	convert_variable_list (pb,list,scope);

  // we create the constraint
  pb->Add_Constraint (new Constant_Element_Global_Constraint (scope,value),false);
}


void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable *> &list, XVariable *value)
// creates an element global constraint whose scope is provided by list with a variable value provided by value
{
  // we first consider the scope of the constraint
	vector<Variable *> scope;
  
	convert_variable_list (pb,list,scope);
  scope.push_back (pb->Get_Variable(value->id));

  // we create the constraint
  pb->Add_Constraint (new Element_Global_Constraint (scope),false);
}


void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, int value) 
// creates a element global constraint whose scope is provided by list with a constant value provided by value and index as a index variable 
{
  if (rank != XCSP3Core::ANY)
    throw ("Error: rank must be equal to any");

	// we first consider the scope of the constraint
	vector<Variable *> scope;
	
	convert_variable_list (pb,list,scope);
  scope.push_back (pb->Get_Variable(index->id));
  
  // we create the constraint
  pb->Add_Constraint (new Constant_Index_Element_Global_Constraint (scope,value),false);
}


void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, XVariable *value) 
// creates an element global constraint whose scope is provided by list with a variable value provided by value and index as a index variable 
{
  // we first consider the scope of the constraint
	vector<Variable *> scope;
  
	convert_variable_list (pb,list,scope);
  scope.push_back (pb->Get_Variable(index->id));
  scope.push_back (pb->Get_Variable(value->id));

  // we create the constraint
  pb->Add_Constraint (new Index_Element_Global_Constraint (scope),false);
}


void Load_XCSP3Callbacks::buildConstraintElement(string, vector<int> &list, int startIndex, XVariable *index, RankType, XVariable *value)
// creates an element global constraint on a list of integers with a variable value provided by value and index as a index variable 
{
  // we first consider the scope of the constraint
	vector<Variable *> scope;
  
  scope.push_back (pb->Get_Variable(index->id));
  scope.push_back (pb->Get_Variable(value->id));

  // we create the constraint 
  Extension_Constraint * c;
  if (use_classical_extension_constraint)
    c = new Binary_Extension_Constraint (scope,false);
  else c = new Positive_Compact_Table_Extension_Constraint (scope);
  
  int t [2];
  
  Domain * di = scope[0]->Get_Domain();
  Domain * dx = scope[1]->Get_Domain();
  
  int i = 0;
  for (vector<int>::iterator iter = list.begin(); iter != list.end(); iter++)
  {
    t[0] = di->Get_Value(i);
    t[1] = dx->Get_Value(*iter);

    if ((t[0] != -1) && (t[1] != -1))
      c->Allow_Tuple (t);
    i++;
  }
  pb->Add_Constraint (c,false);
}

void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable *> &list, XVariable *index, int startIndex, XCondition &xc)
// creates an element global constraint to which a condition is imposed
{
  if (startIndex != 0)
    throw ("Error: The index should start from 0 in the element global constraint");

  // we first consider the scope of the constraint
	vector<Variable *> scope;
  
	convert_variable_list (pb,list,scope);
  scope.push_back (pb->Get_Variable(index->id));
  
  // we create a new variable to represent the value of list[index] (i.e. the value of Element(...))
  int min = scope[0]->Get_Domain()->Get_Real_Min();
  int max = scope[0]->Get_Domain()->Get_Real_Max();
  
  for (unsigned int i = 1; i < list.size(); i++)
  {
    if (min > scope[i]->Get_Domain()->Get_Real_Min())
      min = scope[i]->Get_Domain()->Get_Real_Min();
    if (max < scope[i]->Get_Domain()->Get_Real_Max())
      max = scope[i]->Get_Domain()->Get_Real_Max();
  }
  
  string elt_var = "element_var"+id+to_string(pb->Get_N());
  scope.push_back (pb->Add_Variable(min,max,elt_var,false,true));
  
  // we create a constraint related to the condition xc
  // we look for the target variable that is the variable which is equal to Element(...)
  Variable * target_variable;
  
	if (xc.operandType == XCSP3Core::VARIABLE)
    target_variable = pb->Get_Variable(xc.var);
  else
    if (xc.op != XCSP3Core::IN)
    {
      string id_var = "element_cmp"+id+"_"+to_string (pb->Get_N());
      
      target_variable = pb->Add_Variable (xc.val,xc.val,id_var,false,true);
    }
    else throw ("Error: The operator IN is not take into account");

  // we then consider the scope of the constraint
  Create_Relationship (pb, elt_var, xc, target_variable->Get_Name());


  // we create the constraint
  pb->Add_Constraint (new Index_Element_Global_Constraint (scope),false);
}


void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<vector<XVariable*> > &matrix, int startRowIndex, XVariable *rowIndex, int startColIndex, XVariable* colIndex, XVariable* value) 
// creates an element global constraint whose scope is a matrix 
{
  if ((startRowIndex != 0) || (startColIndex != 0))
    throw ("Error: The index should start from 0 in the element global constraint");

  // we first consider the scope of the constraint by considering a vector instead of a matrix.
	vector<Variable *> scope;

  for (vector<vector<XVariable*>>::iterator iter = matrix.begin(); iter != matrix.end(); iter++)
    convert_variable_list (pb,*iter,scope);
    
  // we create a new variable to represent the index variable
  Variable * index = pb->Add_Variable(0,scope.size()-1,"index_"+id+to_string(pb->Get_N()),false,true);

  scope.push_back (index);
  
  // we link the index variable to the row and column indexes
  string expr = "eq(" + index->Get_Name() + ",add(mul(" + to_string (matrix[0].size()) + "," + rowIndex->id + ")," + colIndex->id + "))";
  
  vector<Variable *> scope_eq;
  Expression e = string_to_expression (expr,pb,scope_eq);
  pb->Add_Constraint(new Predicate_Constraint(scope_eq,e),false);
    
  // we add the value variable to the scope
  scope.push_back (pb->Get_Variable(value->id));
  
  // we create the constraint
  pb->Add_Constraint (new Index_Element_Global_Constraint (scope),false);
}


void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<vector<XVariable*> > &matrix, int startRowIndex, XVariable *rowIndex, int startColIndex, XVariable* colIndex, int value)
{
  if ((startRowIndex != 0) || (startColIndex != 0))
    throw ("Error: The index should start from 0 in the element global constraint");

  // we first consider the scope of the constraint by considering a vector instead of a matrix.
	vector<Variable *> scope;

  for (vector<vector<XVariable*>>::iterator iter = matrix.begin(); iter != matrix.end(); iter++)
    convert_variable_list (pb,*iter,scope);
    
  // we create a new variable to represent the index variable
  Variable * index = pb->Add_Variable(0,scope.size()-1,"index_"+id+to_string(pb->Get_N()),false,true);

  scope.push_back (index);
  
  // we link the index variable to the row and column indexes
  string expr = "eq(" + index->Get_Name() + ",add(mul(" + to_string (matrix[0].size()) + "," + rowIndex->id + ")," + colIndex->id + "))";
  
  vector<Variable *> scope_eq;
  Expression e = string_to_expression (expr,pb,scope_eq);
  pb->Add_Constraint(new Predicate_Constraint(scope_eq,e),false);
    
  // we add the value variable to the scope
  scope.push_back (pb->Add_Variable(value,value,id+"_value"+to_string(pb->Get_N()),false,true));
  
  // we create the constraint
  pb->Add_Constraint (new Index_Element_Global_Constraint (scope),false);  
}


void Load_XCSP3Callbacks::buildConstraintElement(string id, vector<vector<int> > &matrix, int startRowIndex, XVariable *rowIndex, int startColIndex, XVariable* colIndex, XVariable *value)
// creates an element global constraint whose scope is a matrix 
{
  if ((startRowIndex != 0) || (startColIndex != 0))
    throw ("Error: The index should start from 0 in the element global constraint");

  // we first consider the scope of the constraint
	vector<Variable *> scope;
  
  scope.push_back (pb->Get_Variable(rowIndex->id));
  scope.push_back (pb->Get_Variable(colIndex->id));
  scope.push_back (pb->Get_Variable(value->id));

  // we create the constraint 
  Extension_Constraint * c;
  c = new Positive_Compact_Table_Extension_Constraint (scope);
  
  int t [3];
  
  Domain * dri = scope[0]->Get_Domain();
  Domain * dci = scope[1]->Get_Domain();
  Domain * dx = scope[2]->Get_Domain();
  
  for (unsigned int i = 0; i < matrix.size(); i++)
    for (unsigned int j = 0; j < matrix[i].size(); j++)
    {
      t[0] = dri->Get_Value(i);
      t[1] = dci->Get_Value(j);
      t[2] = dx->Get_Value(matrix[i][j]);
      
      if ((t[0] != -1) && (t[1] != -1) && (t[2] != -1))
        c->Allow_Tuple (t);
    }
  pb->Add_Constraint (c,false);
}


void Load_XCSP3Callbacks::buildConstraintInstantiation(string id, vector<XVariable *> &list, vector<int> &values)
// creates an instantiation constraint
{
  int i = 0;
  for (vector<XVariable*>::iterator iter = list.begin(); iter != list.end(); iter++)
  {
    Domain * d = pb->Get_Variable((*iter)->id)->Get_Domain();
    d->Assign (d->Get_Value(values[i]));
    i++;
  }
}



//*********************
// Objective functions
//*********************


void Expression_Objective (COP * pb, string expr, enum Objective obj)
{
  vector<Variable*> scope;
  Expression e = string_to_expression (expr,pb,scope);  
  
  pb->Set_Objective_Function (new Expression_Objective_Function(obj,scope,e));
}


void Load_XCSP3Callbacks::buildObjectiveMinimizeExpression(string expr)
// creates a minimum objective based on an expression
{
  if (dynamic_cast<COP*>(pb) != 0)
    Expression_Objective (dynamic_cast<COP*>(pb),expr,MINIMIZE);
}


void Load_XCSP3Callbacks::buildObjectiveMaximizeExpression(string expr) 
// creates a maximum objective based on an expression
{
  if (dynamic_cast<COP*>(pb) != 0)
    Expression_Objective (dynamic_cast<COP*>(pb),expr,MAXIMIZE);
}


void Load_XCSP3Callbacks::buildObjectiveMinimizeVariable(XVariable *x) 
// creates a minimum objective based on a variable
{
  if (dynamic_cast<COP*>(pb) != 0)
    Expression_Objective (dynamic_cast<COP*>(pb), x->id, MINIMIZE);
}


void Load_XCSP3Callbacks::buildObjectiveMaximizeVariable(XVariable *x) 
// creates a maximum objective based on a variable
{
  if (dynamic_cast<COP*>(pb) != 0)
    Expression_Objective (dynamic_cast<COP*>(pb), x->id, MAXIMIZE);
}


Variable * Add_Weighted_Variable (CSP * pb, Variable * x, int coef)
// adds a variable equal to x * coef to the instance pb and returns it
{      
  Domain * d = x->Get_Domain();
      
  set<long> values;
  for (unsigned int j = 0; j < d->Get_Size();j++)
    values.insert (coef * d->Get_Remaining_Real_Value(j));
  
  pb->Add_Variable (values,x->Get_Name()+ "_coef_"+to_string(pb->Get_N()),false,true);
  Variable * y = pb->Get_Variable(pb->Get_N()-1);

  vector<Variable *> scope;
  scope.push_back (y);
  scope.push_back (x);
  
  pb->Add_Constraint(new Equal_Binary_Constraint(scope,coef,0),false);
  return y;
}


void Minimax_Objective (COP * pb, vector<Variable *> &scope, vector<int> &coefs, bool is_minimum, enum Objective obj)
{
  // we add a variable in order to represent the objective variable and a constraint for expressing the objective
  for (unsigned int i = 0; i < scope.size(); i++)
    if (coefs[i] != 1)
    {
      // we replace the current variable by the weighted variable
      scope[i] = Add_Weighted_Variable (pb, scope[i], coefs[i]);
    }

  // we create the objective function and add it to the instance
  if (is_minimum)
    pb->Set_Objective_Function (new Minimum_Objective_Function(obj,scope));
  else
    pb->Set_Objective_Function (new Maximum_Objective_Function(obj,scope));
}

void Nvalues_Objective (COP * pb, vector<Variable *> &scope, vector<int> &coefs, enum Objective obj)
{
  for (unsigned int i = 0; i < scope.size(); i++)
  {
    if (coefs[i] != 1)
    {
      // we replace the current variable by the weighted variable
      scope[i] = Add_Weighted_Variable (pb, scope[i], coefs[i]);
    }
  }

  // we create the objective function and add it to the instance
  pb->Set_Objective_Function (new Nvalue_Objective_Function(obj,scope));
}


void Product_Objective (COP * pb, vector<Variable *> &scope, vector<int> &coefs, enum Objective obj)
{
  // we add a variable in order to represent the objective variable and a constraint for expressing the product objective
  int coef_product = 1;
  for (vector<int>::iterator iter = coefs.begin(); iter != coefs.end(); iter++)
    coef_product *= *iter;
  
  string expr = "mul(";
  if (coef_product != 1)
    expr += to_string (coef_product);
  for (unsigned int i = 1; i < scope.size(); i++)
    expr += "," + scope[i]->Get_Name();
  expr += ")";
    
  // we create the objective function and add it to the instance
  Expression_Objective (pb,expr, obj);
}


void buildObjective (COP * pb, ExpressionObjective type, vector<Variable *> & scope, vector<int> &coefs, enum Objective obj)
// creates a minimum/maximum objective based on a weighted function 
{
  if (scope.size() > 1)
  {
    switch (type)
    {
      case XCSP3Core::EXPRESSION_O:  
          throw ("Impossible case?");
          break;
      case XCSP3Core::SUM_O: 
          pb->Set_Objective_Function (new Sum_Objective_Function(obj, scope, coefs)); 
          break;
      case XCSP3Core::PRODUCT_O:
          Product_Objective (dynamic_cast<COP*>(pb),scope,coefs, obj); 
          break;
      case XCSP3Core::MINIMUM_O:
          Minimax_Objective (dynamic_cast<COP*>(pb),scope,coefs,true,obj);
          break;
      case XCSP3Core::MAXIMUM_O:  
          Minimax_Objective (dynamic_cast<COP*>(pb),scope,coefs,false,obj);
          break;
      case XCSP3Core::NVALUES_O:  
          Nvalues_Objective (dynamic_cast<COP*>(pb),scope,coefs,obj);
          break;
      case XCSP3Core::LEX_O: 
          throw ("Error: Lex objective not taken into account yet");
          break;
    }
  }
  else
    {
      // the scope contains a single variable (otherwise the instance contains an error)
      if (coefs[0] != 1)
        scope[0] = Add_Weighted_Variable (pb, pb->Get_Variable(scope[0]->Get_Name()), coefs[0]);

      Expression_Objective (pb, scope[0]->Get_Name(), obj);
    }
}


void Load_XCSP3Callbacks::buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs)
// creates a minimum objective based on a weighted function
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we first consider the scope of the objective function
    vector<Variable *> scope;
    
    convert_variable_list (pb,list,scope);
  
    // we build the objective
    buildObjective (dynamic_cast<COP*>(pb), type, scope, coefs, MINIMIZE);
  }
}


void Load_XCSP3Callbacks::buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs)
// creates a maximum objective based on a weighted function
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we first consider the scope of the objective function
    vector<Variable *> scope;
    
    convert_variable_list (pb,list,scope);

    // we build the objective
    buildObjective (dynamic_cast<COP*>(pb), type, scope, coefs, MAXIMIZE);
  }
}


void Load_XCSP3Callbacks::buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list)
// creates a minimum objective based on a function
{
  XCSP3CoreCallbacks::buildObjectiveMinimize(type, list);
}


void Load_XCSP3Callbacks::buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list) 
// creates a maximum objective based on a function
{
  XCSP3CoreCallbacks::buildObjectiveMaximize(type, list);
}


void Load_XCSP3Callbacks::buildObjectiveMinimize(ExpressionObjective type, vector<Tree *> &trees, vector<int> &coefs)
// creates a minimum objective based on a weighted function and expressions
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we create an intermediary variable and constraint for each tree
    vector<Variable *> scope;
    for(unsigned int i = 0; i < trees.size(); i++) 
      Tree_to_Predicate (pb, "obj", trees[i], scope);
    
    // we build the objective
    buildObjective(dynamic_cast<COP*>(pb), type, scope, coefs, MINIMIZE);
  }
}


void Load_XCSP3Callbacks::buildObjectiveMaximize(ExpressionObjective type, vector<Tree *> &trees, vector<int> &coefs)
// creates a maximum objective based on a weighted function and expressions
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we create an intermediary variable and constraint for each tree
    vector<Variable *> scope;
    for(unsigned int i = 0; i < trees.size(); i++) 
      Tree_to_Predicate (pb, "obj", trees[i], scope);
    
    // we build the objective
    buildObjective(dynamic_cast<COP*>(pb),type, scope, coefs, MAXIMIZE);
  }
}


void Load_XCSP3Callbacks::buildObjectiveMinimize(ExpressionObjective type, vector<Tree *> &trees)
// creates a minimum objective based on a weighted function and expressions
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we create an intermediary variable and constraint for each tree
    vector<Variable *> scope;
    for(unsigned int i = 0; i < trees.size(); i++) 
      Tree_to_Predicate (pb, "obj", trees[i], scope);

    // we build the objective
    vector<int> coefs (trees.size(),1);
    buildObjective(dynamic_cast<COP*>(pb),type, scope, coefs, MINIMIZE);
  }
}


void Load_XCSP3Callbacks::buildObjectiveMaximize(ExpressionObjective type, vector<Tree *> &trees)
// creates a maximum objective based on a weighted function and expressions
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we create an intermediary variable and constraint for each tree
    vector<Variable *> scope;
    for(unsigned int i = 0; i < trees.size(); i++) 
      Tree_to_Predicate (pb, "obj", trees[i], scope);

    // we build the objective
    vector<int> coefs (trees.size(),1);
    buildObjective(dynamic_cast<COP*>(pb),type, scope, coefs, MAXIMIZE);
  }
}


void Load_XCSP3Callbacks::buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list, vector<XVariable*> &coefs)
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we first consider the scope of the objective function
    vector<Variable *> scope;
    
    convert_variable_list (pb,list,scope);
    
    for (unsigned int i = 0; i < scope.size(); i++)
    {
      // we replace list[i] by list[i]*coefs[i]
      scope[i] = Add_Weighted_Variable(pb, scope[i], pb->Get_Variable(coefs[i]->id));
    }
  
    // we build the objective
    vector<int> coeffs (scope.size(), 1);
    buildObjective (dynamic_cast<COP*>(pb), type, scope, coeffs, MINIMIZE);
  }
}


void Load_XCSP3Callbacks::buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list, vector<XVariable*> &coefs)
{
  if (dynamic_cast<COP*>(pb) != 0)
  {
    // we first consider the scope of the objective function
    vector<Variable *> scope;
    
    convert_variable_list (pb,list,scope);
    
    for (unsigned int i = 0; i < scope.size(); i++)
    {
      // we replace list[i] by list[i]*coefs[i]
      scope[i] = Add_Weighted_Variable(pb, scope[i], pb->Get_Variable(coefs[i]->id));
    }
  
    // we build the objective
    vector<int> coeffs (scope.size(), 1);
    buildObjective (dynamic_cast<COP*>(pb), type, scope, coeffs, MAXIMIZE);
  }
}
