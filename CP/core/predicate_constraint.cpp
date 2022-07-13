#include "predicate_constraint.h"
#include <stack>


//-----------------------------
// constructors and destructor
//-----------------------------


Predicate_Constraint::Predicate_Constraint (vector<Variable *> & var, Expression & e): Constraint (var), t (var.size()), relation (e)
// constructs a new constraint with number n which involves the variable in var and whose relation is given by the expression e
{	
  relation.Get_Postfix_Expression (postfix_relation);
  values.resize (postfix_relation.size());
}


Predicate_Constraint::Predicate_Constraint (Predicate_Constraint & c): Constraint (c), t (c.scope.size()), relation (c.relation)
// constructs a new constraint by copying the constraint c
{
  relation.Get_Postfix_Expression (postfix_relation);
  values.resize (postfix_relation.size());
}


Predicate_Constraint::~Predicate_Constraint ()
// destructor
{
}


//----------
// operator
//----------


Predicate_Constraint & Predicate_Constraint::operator= (Predicate_Constraint & c)
// operator =
{
	if (this != &c)
	{
		num = c.num;
		arity = c.arity;
		scope = c.scope;
		scope_var = c.scope_var;
		relation = c.relation;
		unassigned_variable_number = c.unassigned_variable_number;
		position = c.position;
	}
	return *this;
}


//-----------------
// basic functions
//-----------------


bool is_present (Expression & e, Expression & e2)
{
	operator_type op = e.Get_Operator();

	if ((e.Get_Type () == OP) && (op == AND))
	{
		if ((e2 == e.Get_Subexpression(0)) || (e2 == e.Get_Subexpression(1)))
			return true;
		else
			{
				int i = 0;
				int arity = e.Get_Arity ();
				while ((i < arity) && (! is_present (e.Get_Subexpression(i),e2)))
					i++;
				return i < arity;
			}
	}
	else return e == e2;
}


int Predicate_Constraint::Evaluate (int t [])
// returns the evaluation of the expression such that the value t[i] is the effective value of the variable i
/// \param[in] t the value of each variable in the expression
{
  int last_value_index = -1;
  for (vector<tuple<node_type,operator_type,int>>::iterator iter = postfix_relation.begin(); iter != postfix_relation.end(); iter++)
  {
    switch (get<0>(*iter))
    {
      case VAL : last_value_index++;
                 values[last_value_index] = get<2>(*iter);
                 break;
      case VAR : last_value_index++;
                 values[last_value_index] = t[get<2>(*iter)];
                 break;
      case OP:
          switch (get<1>(*iter))
          {
            case NEG : 
                values[last_value_index] = - values[last_value_index]; 
                break;
            case ABS : 
                if (values[last_value_index] < 0)
                  values[last_value_index] = - values[last_value_index]; 
                break;
            case ADD : 
                last_value_index--;
                values[last_value_index] += values[last_value_index+1];
                break;
            case SUM :
                {
                  int varvalarity = get<2>(*iter);
                  int sum = 0;
                  for (int i = 0; i < varvalarity; i++)
                    sum += values[last_value_index-i];
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = sum;
                }
                break;
            case SUB : 
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] - values[last_value_index];
                break;
            case MUL :
                last_value_index--;
                values[last_value_index] *= values[last_value_index+1];
                break;
            case PROD : 
                {
                  int prod = 1;
                  int varvalarity = get<2>(*iter);
                  for (int i = 0; i < varvalarity; i++)
                    prod *= values[last_value_index-i];
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = prod;
                }
                break;
            case DIST : 
                last_value_index--;
                values[last_value_index] -= values[last_value_index+1];
                if (values[last_value_index] < 0) 
                  values[last_value_index] = - values[last_value_index];
                break;
            case DIV :
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] / values[last_value_index];
                break;
            case MOD :
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] % values[last_value_index];
                break;
            case EQ  : 
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] == values[last_value_index];
                break;
            case EQU :
                {
                  int n = 1;
                  int val = values[last_value_index];
                  int varvalarity = get<2>(*iter);
                  while ((n < varvalarity) && (values[last_value_index-n] == val))
                    n++;
                  
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = n == varvalarity;
                }
                break;
            case NE  :
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] != values[last_value_index];
                break;
            case GE  :
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] >= values[last_value_index];
                break;
            case GT  : 
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] > values[last_value_index];
                break;
            case LE  :
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] <= values[last_value_index];
                break;
            case LT  :
                last_value_index--;
                values[last_value_index] = values[last_value_index+1] < values[last_value_index];
                break;
            case NOT :
                if (values[last_value_index] == 0)
                  values[last_value_index] = 1;
                else values[last_value_index] = 0;
                break;
            case AND :
                {
                  int i = 0; 
                  int varvalarity = get<2>(*iter);

                  while ((i < varvalarity) && (values[last_value_index-i] != 0))
                    i++;
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = i == varvalarity;
                }
                break;
            case OR  :
                {
                  int i = 0; 
                  int varvalarity = get<2>(*iter);

                  while ((i < varvalarity) && (values[last_value_index-i] == 0))
                    i++;
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = i < varvalarity;
                }
                break;
            case SQR : 
                values[last_value_index] *= values[last_value_index];
                break;
            case POW :
                {
                  int p;
                  p = 1;
                  for (int i = 0; i < values[last_value_index-1]; i++)
                    p *= values[last_value_index];
                  last_value_index--; 
                  values[last_value_index] = p;
                }
                break;
            case MIN : 
                {
                  int min = values[last_value_index];
                  int varvalarity = get<2>(*iter);
                  for (int i = 1; i < varvalarity; i++)
                    if (values[last_value_index-i] < min)
                      min = values[last_value_index-i];
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = min;
                }
                break;
            case MAX :
                {
                  int max = values[last_value_index];
                  int varvalarity = get<2>(*iter);
                  for (int i = 1; i < varvalarity; i++)
                    if (values[last_value_index-i] > max)
                      max = values[last_value_index-i];
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = max;
                }
                break;
            case XOR : 
                {
                  int n = 0;  // n is used as a counter which represents the number of true term
                  int varvalarity = get<2>(*iter);
                  for (int i = 0; i < varvalarity; i++)
                    if (values[last_value_index-i] != 0)
                      n++;
                  last_value_index -= varvalarity-1;
                  values[last_value_index] = n%2 == 1;
                }
                break;
            case IFF : 
                {
                  int n = 1;
                  int varvalarity = get<2>(*iter);
                  
                  if (values[last_value_index] == 0)
                  {                    
                    while ((n < varvalarity) && (values[last_value_index-n] == 0))
                      n++;
                  }
                  else
                    {                    
                      while ((n < varvalarity) && (values[last_value_index-n] != 0))
                        n++;
                    }

                  last_value_index -= varvalarity-1;
                  values[last_value_index] = n == varvalarity;
                }
                break;
            case IF  :
                if (values[last_value_index] != 0)
                  values[last_value_index-2] = values[last_value_index-1];
                last_value_index -= 2;
                break;
            case IMP :
                if (values[last_value_index] == 0)
                  values[last_value_index-1] = 1;
                last_value_index--;
                break;
            case SET :
                last_value_index++;
                values[last_value_index] = get<2>(*iter);
                break;
            case IN  :
                {
                  int i = 0;
                  while ((i < values[last_value_index-1]) && (values[last_value_index] != values[last_value_index-i-2]))
                    i++;
                  values[last_value_index - values[last_value_index-1]-1] = i < values[last_value_index-1];
                  last_value_index -= values[last_value_index-1]+1;
                }
                break;
            case NOP: 
                break;
          }
          break;
      case NIL : break;				
    }
  }
	return values[0];
}



void Predicate_Constraint::Merge (Predicate_Constraint * c)
// merges the current constraint with the constraint c
{
	arity = scope.size();	
	vector<int> effective_parameters (arity);
	vector<node_type> effective_parameter_type (arity);
	
	for (unsigned int i = 0; i < arity; i++)
	{
		effective_parameters[i] = Get_Position (c->scope_var[i]->Get_Num());
		effective_parameter_type [i] = VAR;
	}
	
	Expression subexpr[2];
	subexpr[0] = relation;
	subexpr[1] = c->relation;
	subexpr[1].Change_Variable (effective_parameters,effective_parameter_type);
	
	if (! is_present (subexpr[0],subexpr[1]))
  {
		relation = Expression (AND,subexpr,2);
    postfix_relation.clear();
    relation.Get_Postfix_Expression (postfix_relation);
    values.resize (postfix_relation.size());
  }
}


bool Predicate_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	int real_t [arity];
	
	for (unsigned int i = 0; i < arity; i++)
		real_t[i] = scope_var[i]->Get_Domain()->Get_Real_Value (t[i]);

	return relation.Evaluate(real_t) != 0;
}


bool Predicate_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
  
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	int i = 0;
	int v;
	
  int * ts;
  bool has_no_support;
  bool record_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
	bool exist_more_tuples;
	
	do
	{
		v = dx->Get_Remaining_Value (i);
		
		ts = ls->Get_Support (num,x,v);
		
		if (ts == 0) 
		{
			t.Assign (x,v,dx->Get_Real_Value(v));
			t.Complete (scope_var,x);
			has_no_support = true;
			record_support = true;
			exist_more_tuples = true;
		}
		else
			{
				for (unsigned int j = 0; j < arity; j++)
					t.Assign (j,ts[j],scope_var[j]->Get_Domain()->Get_Real_Value(ts[j]));

				if (t.Is_Valid (scope_var))
				{
					has_no_support = false;
					record_support = false;
					exist_more_tuples = true;
				}
				else 
					{
						has_no_support = true;
						record_support = true;
						
						if (ls->Continue_From_Scratch())
						{
							t.Assign (x,v,dx->Get_Real_Value(v));
							t.Complete (scope_var,x);
							exist_more_tuples = true;							
						}
						else exist_more_tuples = t.Next_Compatible_Valid (scope_var,x);
					}
			}
				
		while ((has_no_support) && (exist_more_tuples))
		{
      if (Evaluate(t.Get_Real_Values()) != 0)
				has_no_support = false;
			else exist_more_tuples = t.Next_Valid (scope_var,x);
		}
		
		if (has_no_support)
		{
			ds->Add_Element (scope_var [x]);
			dx->Filter_Value (v);
			dx_size--;
			
			modif = true;
		}
		else 
			{
				if (record_support)
				{
					ds->Add_Support (num,x,v,arity,ts);
					ls->Change_Support (num,x,v,t.Get_Values(),arity);
				}
				i++;
			}
  }
  while (i < dx_size);

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
  
  return modif;
}


void Predicate_Constraint::Get_Allowed_Tuple_List (list<int *> & list)
// constructs the list of tuples allowed by the constraint
{
	list.clear();
	
	Real_Tuple t (scope.size());
	t.Complete (scope_var);
	int * t2;
	bool again;
	
	do
	{
		if (relation.Evaluate (t.Get_Real_Values()) != 0)
		{
			t2 = new int [arity];
			for (unsigned int i = 0; i < arity; i++)
				t2[i] = t[i];
			
			list.push_back (t2);
		}
		again = t.Next_Valid (scope_var);	
	}
	while (again);
}


string Predicate_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<intension> ";

  stack<pair<string,Expression>> S;
  S.push (make_pair("",relation));
  
  do
  {
    pair<string,Expression> top = S.top();
    S.pop();
    
    if (top.first == "")
    {
      switch (top.second.Get_Type())
      {
        case VAL :
            expr += to_string(top.second.Get_Value());
            break;
        case VAR :
            expr += scope_var[top.second.Get_Variable()]->Get_Name();
            break;
        case OP:
            switch (top.second.Get_Operator())
            {
              case NEG : expr += "neg("; break;
              case ABS : expr += "abs("; break;
              case ADD : expr += "add("; break;
              case SUM : expr += "add("; break;
              case SUB : expr += "sub("; break;
              case MUL : expr += "mul("; break;
              case PROD: expr += "mul("; break;
              case DIV : expr += "div("; break;
              case MOD : expr += "mod("; break;
              case SQR : expr += "sqr("; break;
              case POW : expr += "pow("; break;
              case MIN : expr += "min("; break;
              case MAX : expr += "max("; break;
              case EQ  : expr += "eq("; break;
              case EQU : expr += "eq("; break;
              case NE  : expr += "ne("; break;
              case GE  : expr += "ge("; break;
              case GT  : expr += "gt("; break;
              case LE  : expr += "le("; break;
              case LT  : expr += "lt("; break;
              case NOT : expr += "not("; break;
              case AND : expr += "and("; break;
              case OR  : expr += "or("; break;
              case XOR : expr += "xor("; break;
              case IFF : expr += "iff("; break;
              case IF  : expr += "if("; break;
              case DIST: expr += "dist("; break;
              case IMP : expr += "imp("; break;
              case SET : expr += "set("; break;
              case IN  : expr += "in("; break;
              case NOP : break;
            }
            
            S.push (make_pair(")",Expression()));
            for (int i = top.second.Get_Arity()-1; i >= 0; i--)
            {
              if (i < top.second.Get_Arity()-1)
                S.push (make_pair(",",Expression()));
              S.push (make_pair("",top.second.Get_Subexpression(i)));
            }
            break;
        case NIL : 
            break;
      }
    }
    else expr += top.first;
  }
  while (! S.empty());

  
  expr += "</intension>";
  return expr;
}
