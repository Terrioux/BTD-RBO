/// \file 
/// \brief Sources related to the CSP class

#include "csp.h"
#include "domain.h"
#include "binary_extension_constraint.h"
#include "negative_nary_extension_constraint.h"
#include "positive_nary_extension_constraint.h"
#include "positive_minimal_frequency_difference_binary_constraint.h"
#include "negative_minimal_frequency_difference_binary_constraint.h"
#include "minimal_frequency_difference_binary_constraint.h"
#include "predicate_constraint.h"
#include "universal_constraint.h"
#include "nogood_constraint.h"
#include "nogood_base_global_constraint.h"
#include "unary_nogood_base_global_constraint.h"
#include "compact_table_extension_constraint.h"
#include "expression.h"
#include <typeinfo>
#include <stack>


//-----------------------------
// constructors and destructor
//-----------------------------


CSP::CSP (Event_Manager * em, string pb_name)
// constructs an empty CSP whose name is pb_name and for which the events are managed by em
/// \param[in] em the event manager
/// \param[in] pb_name  the name of the CSP instance
{
	h = new Multi_Hypergraph;
	mandatory_h = 0;
	name = pb_name;
  unary_nogood_base = new Unary_Nogood_Base_Global_Constraint();
  mandatory_variable_number = 0;
  nogood_base = 0;
  event_manager = em;
  last_conflict_variable = 0;
  last_conflict_constraint = 0;
  requires_constraint_restoration = false;
}


CSP::~CSP ()
// destructor
{
  delete unary_nogood_base;
	for (Constraint * c : constraints)
		delete c;
	
	for (Variable * x : variables)
		delete x;
	
	delete h;
	delete mandatory_h;
  
  delete event_manager;
}


//-----------------
// basic functions
//-----------------


Variable * CSP::Add_Variable (set<long> & values, string var_name, bool is_initial, bool is_auxiliary)
// adds a new variable v to the CSP whose domain is defined by the set values and whose name is var_name, is_initial is set to true if the variable appears initially in the problem, is_auxiliary is set to true if the variable is auxiliary and returns it
/// \param[in] values the set of values which defines the domain of the new variable
/// \param[in] var_name the name of the new variable
/// \param[in] is_auxiliary true if the variable is auxiliary, false otherwise
{
  Variable * x = new Variable (values,Get_N(),var_name,event_manager, is_initial, is_auxiliary);
  variables.push_back(x);
  if (! is_auxiliary)
    mandatory_variable_number++;
	h->Add_Vertex();
  
  
  return x;
}


Variable * CSP::Add_Variable (long a, long b, string var_name, bool is_initial, bool is_auxiliary)
// adds a new variable v to the CSP whose domain is defined by the values of [a,b] and whose name is var_name, is_initial is set to true if the variable appears initially in the problem, is_auxiliary is set to true if the variable is auxiliary and returns it
/// \param[in] a the lower bound of the integer domain of the new variable
/// \param[in] b the upper bound of the integer domain of the new variable
/// \param[in] var_name the name of the new variable
/// \param[in] is_auxiliary true if the variable is auxiliary, false otherwise
{
  assert (a <= b);

  Variable * x = new Variable (a,b,Get_N(),var_name,event_manager, is_initial, is_auxiliary);
  variables.push_back(x);
  if (! is_auxiliary)
    mandatory_variable_number++;
	h->Add_Vertex();
  
  return x;
}


Variable * CSP::Get_Variable (string label)
// returns a pointer on the variable whose name is label
/// \param[in] label the name of the wanted variable
{
	for (Variable * x : variables)
    if (x->Get_Name().compare (label) == 0)
      return x;

  return 0;
}


int CSP::Add_Constraint (Constraint * c, bool duplicate)
// adds a new constraint c to the CSP (a new memory space is allocated if duplicate is set to true) and returns the number of the constraint
/// \param[in] c the constraint we want to add
/// \param[in] duplicate boolean which specifies if the constraint must be duplicated or not
{
	unsigned int a = c->Get_Arity();
	set <unsigned int> elements = c->Get_Scope();
	unsigned int i;
	bool addition = false;
	bool not_found = true;
	
	// we look for a constraint with the same scope as c and whose type is compatible with c's one in order to merge the two constraints
	i = 0;
  if (typeid(*c) == typeid(Predicate_Constraint))
  {
    while ((i < Get_M()) && (not_found))
    {
      if ((a == constraints[i]->Get_Arity()) && (typeid(*c) == typeid(*constraints[i])))
        not_found = constraints[i]->Get_Scope() != elements;
      i++;
    }
  }
  else
    if ((typeid(*c) == typeid (Positive_Minimal_Frequency_Difference_Binary_Constraint)) || (typeid(*c) == typeid (Negative_Minimal_Frequency_Difference_Binary_Constraint)))
    {
      while ((i < Get_M()) && (not_found))
      {
        if (a == constraints[i]->Get_Arity())
          not_found = constraints[i]->Get_Scope() != elements;
        i++;
      }
    }

	if (not_found)  addition = true;
	else
		{
			i--;
			if (typeid(*constraints[i]) == typeid(Predicate_Constraint))
			{
				addition = false;
				dynamic_cast<Predicate_Constraint*>(constraints[i])->Merge (dynamic_cast<Predicate_Constraint*>(c));
        delete c;		
			}
			else
        if ((dynamic_cast<Positive_Minimal_Frequency_Difference_Binary_Constraint*>(c) != 0) && (dynamic_cast<Negative_Minimal_Frequency_Difference_Binary_Constraint*>(constraints[i]) != 0))
        {
          Constraint * c2;
          int cst1 = dynamic_cast<Positive_Minimal_Frequency_Difference_Binary_Constraint*>(c)->Get_Constant();
          int cst2 = dynamic_cast<Negative_Minimal_Frequency_Difference_Binary_Constraint*>(constraints[i])->Get_Constant();
      
          vector<Variable *> scope (c->Get_Arity());
          
          for (unsigned int j = 0; j < c->Get_Arity(); j++)
            scope[j] = c->Get_Scope_Variable(j);
          
          c2 = new Minimal_Frequency_Difference_Binary_Constraint (scope,cst1,cst2);

          delete constraints[i];
          
          if (! duplicate)
            delete c;
            
          constraints[i] = c2;
        }
        else
          if ((dynamic_cast<Negative_Minimal_Frequency_Difference_Binary_Constraint*>(c) != 0) && (dynamic_cast<Positive_Minimal_Frequency_Difference_Binary_Constraint*>(constraints[i]) != 0))
          {
            Constraint * c2;
            int cst1 = dynamic_cast<Positive_Minimal_Frequency_Difference_Binary_Constraint*>(constraints[i])->Get_Constant();
            int cst2 = dynamic_cast<Negative_Minimal_Frequency_Difference_Binary_Constraint*>(c)->Get_Constant();
            
            vector<Variable *> scope (c->Get_Arity());
            
            for (unsigned int j = 0; j < c->Get_Arity(); j++)
              scope[j] = c->Get_Scope_Variable(j);
            
            c2 = new Minimal_Frequency_Difference_Binary_Constraint (scope,cst1,cst2);

            delete constraints[i];
            
            if (! duplicate)
              delete c;
              
            constraints[i] = c2;								
          }
          else addition = true;
		}
	
	if (addition)
	{
		if (duplicate)	constraints.push_back(c->Duplicate());
		else constraints.push_back(c);
		constraints[Get_M()-1]->Set_Num (Get_M()-1);

		if (duplicate)
		{
			vector<Variable *> new_scope (a);
			for (unsigned i = 0; i < a; i++)
				new_scope[i] =  Get_Variable (constraints[Get_M()-1]->Get_Scope_Variable (i)->Get_Num());
			
			constraints[Get_M()-1]->Change_Scope (new_scope);
		}
	
		h->Graphical_Structure::Add_Edge(elements);
		
    if ((nogood_base == 0) && (dynamic_cast<Nogood_Base_Global_Constraint*>(c) != 0))
      nogood_base = dynamic_cast<Nogood_Base_Global_Constraint*>(constraints[Get_M()-1]);
		
    if (dynamic_cast<Compact_Table_Extension_Constraint*> (c) != 0)
      requires_constraint_restoration = true;
      
		return Get_M()-1;
	}
	else return i;
}


void CSP::Replace_Constraint (unsigned int num, Constraint * c)
// replaces the constraint whose number is num by the constraint c (we assume that the two constraints have the same scope)
/// param[in] num the number of the constraint we want to replace
/// param[in] c the constraint used for the replacement
{
	if (num < Get_M())
	{
    bool update_structure = true;
    
    if ((c->Get_Arity() == constraints[num]->Get_Arity()) && (h->Get_Edge(num)->Get_Arity() > 0))
    {
      set<unsigned int>::iterator iter = constraints[num]->Begin();
      set<unsigned int>::iterator iter_c = c->Begin();
      while ((iter != constraints[num]->End()) && (iter_c != c->End()) && (*iter == *iter_c))
      {
        iter++;
        iter_c++;
      }
      
      update_structure = (iter != constraints[num]->End()) || (iter_c != c->End());
    }

    bool update_nogood_base = (nogood_base != 0) && (nogood_base->Get_Num() == num);
      
		delete constraints [num];
		constraints [num] = c;
		c->Set_Num (num);
    
    if (update_nogood_base)
      nogood_base = dynamic_cast<Nogood_Base_Global_Constraint*>(c);
    
    if (update_structure)
    {
      h->Reset_Edge (num);
      
      for (set<unsigned int>::iterator iter = c->Begin(); iter != c->End(); iter++)
        h->Add_Vertex_In_Edge (*iter,num);
    }

    if (dynamic_cast<Compact_Table_Extension_Constraint*> (c) != 0)
      requires_constraint_restoration = true;
	}
}


Multi_Hypergraph * CSP::Get_Mandatory_Structure()
// returns a pointer on the constraint hypergraph representing the structure of the CSP restricted to mandatory variables
{
  if (mandatory_variable_number == Get_N())
    return h;
  else
    if (mandatory_h == 0)
    {
      mandatory_h = new Multi_Hypergraph();
      
      // we add a vertice per variable (including auxiliary variables)
      for (unsigned int i = 0; i < Get_N(); i++)
        mandatory_h->Add_Vertex ();
      
      // we keep each hyperedge of h which only involves mandatory variables
      for (map<int,Edge*>::iterator iter = h->Begin_Edge_List (); iter != h->End_Edge_List (); iter++)
      {
        vertex scope [(*iter).second->Get_Arity()];
        unsigned int arity = 0;
        
        for (set<vertex>::iterator iter_v = (*iter).second->Begin(); (iter_v != (*iter).second->End()) && (! variables[*iter_v]->Is_Auxiliary ()); iter_v++)
        {
          scope[arity] = *iter_v;
          arity++;
        }
        
        if (arity == (unsigned) (*iter).second->Get_Arity())
          mandatory_h->Add_Edge_End(scope,arity);
      }
      
      // we add some edges to take into account the relationship between mandatory variables via auxiliary variables
      vector<bool> marked (Get_N());
      for (unsigned int i = 0; i < Get_N(); i++)
        marked[i] = ! Get_Variable(i)->Is_Auxiliary();
        
      for (unsigned int i = 0; i < Get_N(); i++)
        if (! marked[i])
        {
          set<unsigned int> scope;
          
          stack<unsigned int> S;
          S.push(i);
          marked[i] = true;
          
          while (! S.empty())
          {
            unsigned x = S.top();
            S.pop();
            
            for (list<Edge *>::iterator iter = h->Begin_Edge_List(x) ; iter != h->End_Edge_List(x); iter++)
            {
              for (vertex v : (*iter)->Get_Scope())
              {
                if (Get_Variable(v)->Is_Auxiliary())
                {
                  if (! marked[v])
                  {
                    S.push(v);
                    marked[v] = true;
                  }
                }
                else scope.insert(v);
              }
            }
          }
          
          if (scope.size() > 1)
          {
            unsigned int arity = 0;
            unsigned int scope_edge [scope.size()];
            for (unsigned int v : scope)
            {
              scope_edge[arity] = v;
              arity++;
            }
            mandatory_h->Add_Edge_End(scope_edge,arity);
          }
        }
        
    }
  return mandatory_h;
}
