#include "expression.h"
#include <sstream>


//-----------------------------
// constructors and destructor
//-----------------------------


Expression::Expression ()
// constructs an empty expression (i.e. whose type is NIL)
{
	type = NIL;
	subexpression = 0;
}


Expression::Expression (node_type nt, int var_val)
// constructs an expression corresponding to a variable or to a value depending on nt
/// \param[in] nt the type of the node we want to create
/// \param[in] var_val the value or the variable we want to add
{
	if ((nt == VAL) || (nt == VAR))
	{
		type = nt;
		varvalarity = var_val;
		subexpression = 0;
	}
	else throw ("Incorrect use of the construction Expression (node_type,int)");
}


Expression::Expression (operator_type oper, Expression * sub, int arity)
// constructs an expression corresponding to the operator oper whose operands are given by sub whose size is arity
/// \param[in] oper the operator we want to add
/// \param[in] sub the subexpressions related to the operator
/// \param[in] arity the arity of the operator
{
	type = OP;
	op = oper;

	switch (op)
	{
		case NEG : varvalarity = 1; break;
		case ABS : varvalarity = 1; break;
		case SQR : varvalarity = 1; break;
		case NOT : varvalarity = 1; break;
		case IF  : varvalarity = 3; break;
		case SUM : varvalarity = arity; break;
		case PROD: varvalarity = arity; break;
		case MIN : varvalarity = arity; break;
		case MAX : varvalarity = arity; break;
		case EQU : varvalarity = arity; break;
		case AND : varvalarity = arity; break;
		case OR  : varvalarity = arity; break;
		case XOR : varvalarity = arity; break;
		case IFF : varvalarity = arity; break;
		case SET : varvalarity = arity; break;
		case INI : varvalarity = 3; break;
		default  : varvalarity = 2;
	}
	
	subexpression = new Expression [varvalarity];
	
	for (int i = 0; i < varvalarity; i++)
		subexpression[i] = sub[i];
}


Expression::Expression (operator_type oper, vector<Expression> & sub)
// constructs an expression corresponding to the operator oper whose operands are given by sub whose size is arity
/// \param[in] oper the operator we want to add
/// \param[in] sub the subexpressions related to the operator
{
	type = OP;
	op = oper;
	
	subexpression = new Expression [sub.size()];
	varvalarity = sub.size();
	
	for (unsigned int i = 0; i < sub.size(); i++)
		subexpression[i] = sub[i];
}


Expression::Expression (Expression & e)
// constructs an expression by copying the expression e
/// \param[in] e the expression we want to copy
{
	if (e.type == OP)
	{
		type = OP;
		op = e.op;

		varvalarity = e.varvalarity;
		subexpression = new Expression [varvalarity];

		for (int i = 0; i < varvalarity; i++)
			subexpression[i] = e.subexpression[i];		
	}
	else
		{
			type = e.type;
			varvalarity = e.varvalarity;
			subexpression = 0;
		}
}


Expression::Expression (const Expression & e)
// constructs an expression by copying the expression e
/// \param[in] e the expression we want to copy
{
	if (e.type == OP)
	{
		type = OP;
		op = e.op;

		varvalarity = e.varvalarity;
		subexpression = new Expression [varvalarity];

		for (int i = 0; i < varvalarity; i++)
			subexpression[i] = e.subexpression[i];		
	}
	else
		{
			type = e.type;
			varvalarity = e.varvalarity;
			subexpression = 0;
		}
}


Expression::~Expression ()
// destructor
{
	delete [] subexpression;
}


//----------
// operator
//----------


Expression & Expression::operator= (const Expression & e)		
// operator =
/// \param[in] e the expression we want to assign
{
	if (this != &e)
	{
		delete [] subexpression;
		
		if (e.type == OP)
		{
			type = OP;
			op = e.op;

			varvalarity = e.varvalarity;
			subexpression = new Expression [varvalarity];

			for (int i = 0; i < varvalarity; i++)
				subexpression[i] = e.subexpression[i];		
		}
		else
			{
				type = e.type;
				varvalarity = e.varvalarity;
				subexpression = 0;
			}
	}
	return *this;
}


bool Expression::operator== (const Expression & e)
// operator ==
/// \param[in] e the expression we want to compare
{
	if (this == &e)  return true;
	else
		{			
			if (type == OP)
			{
				if (type == e.type) 
				{
					if ((op == e.op) && (varvalarity == e.varvalarity))
					{
						int i;
						if ((op == MAX) || (op == MIN) || (op == ADD) || (op == SUM) || (op == MUL) || (op == PROD) || (op == AND) || (op == OR) || (op == EQ) || (op == EQU) || (op == NE) || (op == IFF) || (op == XOR) || (op == DIST) || (op == SET))
						{
							// the operator is commutative
							int j;
							i = 0; 
							do
							{
								j = 0;
								while ((j < varvalarity) && (subexpression[i] != e.subexpression[j]))
									j++;
									
								if (j < varvalarity)
									i++;
							}
							while ((i < varvalarity) && (j < varvalarity));
						}
						else
							for (i = 0; (i < varvalarity) && (subexpression[i] == e.subexpression[i]); i++);
						
						return (i == varvalarity);	
					}
					else return false;
				}
				else return false;
			}
			else return (type == e.type) && (varvalarity == e.varvalarity);
		}
}


//-----------------
// basic functions
//-----------------


void Expression::Change_Variable (vector<int> & effective_parameters, vector<node_type> & effective_parameter_type)
// modifies the expression by providing effective value or changing the variable name for the variables of the current expression
/// \param[in] effective_parameters the new effective values of the variables
/// \param[in] effective_parameter_type the type of the new effective value of the variables
{
	if (type == VAR)
	{
		type = effective_parameter_type[varvalarity];
		varvalarity = effective_parameters[varvalarity];
	}
	else
		if (type == OP)
		{
			for (int i = 0; i < varvalarity; i++)
				subexpression[i].Change_Variable(effective_parameters,effective_parameter_type);
		}
}


string Expression::Get_String ()
// returns the string corresponding to the expression in the functional format
{
	stringstream ss;
	string res;
	switch (type)
	{
		case VAL :
				ss << varvalarity;
				res =  ss.str();
				break;
		case VAR :
				ss << "X" << varvalarity;
				res =  ss.str();
				break;
		case OP:
				switch (op)
				{
 				  case NEG : res = "neg("; break;
					case ABS : res = "abs("; break;
					case ADD : res = "add("; break;
					case SUM : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "add(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "add("; 
                     break;
					case SUB : res = "sub("; break;
					case MUL : res = "mul("; break;
					case PROD: if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "mul(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "mul("; 
                     break;
					case DIV : res = "div("; break;
					case MOD : res = "mod("; break;
					case SQR : res = "sqr("; break;
					case POW : res = "pow("; break;
					case MIN : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "min(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "min(";
                     break;
					case MAX : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "max(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "max(";
                     break;
					case EQ  : res = "eq("; break;
					case EQU : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "eq(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "eq(";
                     break;
					case NE  : res = "ne("; break;
					case GE  : res = "ge("; break;
					case GT  : res = "gt("; break;
					case LE  : res = "le("; break;
					case LT  : res = "lt("; break;
					case NOT : res = "not("; break;
					case AND : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "and(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "and(";
                     break;
					case OR  : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "or(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "or(";
                     break;
					case XOR : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "xor(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "xor(";
                     break;
					case IFF : if (varvalarity > 2)
                     {
               				for (int i = 0; i < varvalarity-1; i++)
                      {
                        res += "iff(";
                        res += subexpression[i].Get_String();
                        res += ",";
                      }
                      res += subexpression[varvalarity-1].Get_String();
                      for (int i = 0; i < varvalarity-1; i++)
                        res += ")";
                      
                      return res;
                     }
                     else res = "iff(";
                     break;
					case IF  : res = "if("; break;
					case DIST: res = "dist("; break;
					case IMP : res = "imp("; break;
					case SET : res = "set("; break;
					case IN  : res = "in("; break;
					case INI : res = "ini("; break;
          case NOP : break;
				}
				
				for (int i = 0; i < varvalarity; i++)
				{
					res += subexpression[i].Get_String();
					if (i != varvalarity-1)
						res += ",";
				}
				res += ")";
				break;
		case NIL : 
				res =  "NIL";
				break;
	}
	return res;
}


void Expression::Get_Postfix_Expression (vector<tuple<node_type,operator_type,int>> & postfix_expression)
// computes the equivalent postfix expression of the current expression
{
	switch (type)
	{
		case VAL: postfix_expression.push_back (make_tuple(VAL,NOP,varvalarity));
              break;
		case VAR: postfix_expression.push_back (make_tuple(VAR,NOP,varvalarity));
              break;
		case OP : for (int i = varvalarity-1; i >= 0 ; i--)
                subexpression[i].Get_Postfix_Expression(postfix_expression);
              postfix_expression.push_back (make_tuple(OP,op,varvalarity));
              break;
		case NIL: break;
	}  
}


int Expression::Get_Minimal_Value (int min [], int max [])
// returns the minimal value of the expression w.r.t. the minimal and maximal value of the related variables
{
  int n,m;
	switch (type)
	{
		case VAL : return varvalarity;
		case VAR : return min[varvalarity];
		case OP:
				switch (op)
				{
 				  case NEG : return -subexpression[0].Get_Maximal_Value(min,max);
					case ABS : 
              return 0;
					case ADD : return subexpression[0].Get_Minimal_Value(min,max) + subexpression[1].Get_Minimal_Value(min,max);
					case SUM :
              n = 0;
							for (int i = 1; i < varvalarity; i++)
								n += subexpression[i].Get_Minimal_Value(min,max);
							return n;
					case SUB : return subexpression[0].Get_Minimal_Value(min,max) - subexpression[1].Get_Maximal_Value(min,max);
					case MUL :
					case PROD : 
              {
                bool all_positive = true;
                int prod_pos = 1;
                int prod_mix = 1;
                for(int i = 0 ; i < varvalarity ; i++)
                {
                  int vmin = subexpression[i].Get_Minimal_Value(min,max);
                  int vmax = subexpression[i].Get_Maximal_Value(min,max);
                  
                  if (vmin < 0)
                    all_positive = false;
                  
                  prod_pos *= vmin;
                  
                  if (abs(vmin) < abs(vmax))
                    prod_mix *= vmax;
                  else prod_mix *= vmin;
                }
                
                if (all_positive)
                  return prod_pos;
                else return - abs(prod_mix);
              }
					case DIST : 
              return 0;
					case DIV : return subexpression[0].Get_Minimal_Value(min,max) / subexpression[1].Get_Maximal_Value(min,max);
					case MOD : return 0;
					case SQR : 
              n = subexpression[0].Get_Minimal_Value(min,max);
							return n*n;
					case POW : 
							int p;
              n = subexpression[0].Get_Minimal_Value(min,max);
							m = subexpression[1].Get_Minimal_Value(min,max);
							p = 1;
							for (int i = 0; i < m; i++)
								p *= n;
							return p;
					case MIN : 
              n = subexpression[0].Get_Minimal_Value(min,max);
							for (int i = 1; i < varvalarity; i++)
							{
								m = subexpression[i].Get_Minimal_Value(min,max);
								if (m < n)
									n = m;
							}
							return n;
					case MAX :
							n = subexpression[0].Get_Minimal_Value(min,max);
              for (int i = 1; i < varvalarity; i++)
							{
								m = subexpression[i].Get_Minimal_Value(min,max);
								if (m > n)
									n = m;
							}
							return n;
					case SET : 
							return varvalarity;
					default: 
							return 0;
				}
				break;
		case NIL : return 0;				
	}
	return 0;
}


int Expression::Get_Maximal_Value (int min [], int max [])
// returns the maximal value of the expression w.r.t. the minimal and maximal value of the related variables
{
  int n,m;
	switch (type)
	{
		case VAL : return varvalarity;
		case VAR : return max[varvalarity];
		case OP:
				switch (op)
				{
 				  case NEG : return -subexpression[0].Get_Minimal_Value(min,max);
					case ABS :
              {
                int val_min = abs(subexpression[0].Get_Minimal_Value(min,max));
                int val_max = abs(subexpression[0].Get_Maximal_Value(min,max));
              
                if (val_min < val_max)
                  return val_max;
                else return val_min;
              }
					case ADD : return subexpression[0].Get_Maximal_Value(min,max) + subexpression[1].Get_Maximal_Value(min,max);
					case SUM :
              n = 0;
							for (int i = 1; i < varvalarity; i++)
								n += subexpression[i].Get_Maximal_Value(min,max);
							return n;
					case SUB : return subexpression[0].Get_Maximal_Value(min,max) - subexpression[1].Get_Minimal_Value(min,max);
					case MUL :
					case PROD : 
              {
                bool all_positive = true;
                int prod_pos = 1;
                int prod_mix = 1;
                for(int i = 0 ; i < varvalarity ; i++)
                {
                  int vmin = subexpression[i].Get_Minimal_Value(min,max);
                  int vmax = subexpression[i].Get_Maximal_Value(min,max);
                  
                  if (vmin < 0)
                    all_positive = false;
                  
                  prod_pos *= vmax;
                  
                  if (abs(vmin) < abs(vmax))
                    prod_mix *= vmax;
                  else prod_mix *= vmin;                  
                }
                
                if (all_positive)
                  return prod_pos;
                else return abs(prod_mix);
              }
					case DIST : 
              n = subexpression[0].Get_Maximal_Value(min,max) - subexpression[1].Get_Minimal_Value(min,max);
              m = subexpression[0].Get_Minimal_Value(min,max) - subexpression[1].Get_Maximal_Value(min,max);
              if (abs(n) < abs(m))
                return abs(m);
              else
                return abs(n);
					case DIV : return subexpression[0].Get_Minimal_Value(min,max) / subexpression[1].Get_Maximal_Value(min,max);
					case MOD : return subexpression[1].Get_Maximal_Value(min,max);
					case SQR : 
              n = subexpression[0].Get_Maximal_Value(min,max);
							return n*n;
					case POW : 
							int p;
              n = subexpression[0].Get_Maximal_Value(min,max);
							m = subexpression[1].Get_Maximal_Value(min,max);
							p = 1;
							for (int i = 0; i < m; i++)
								p *= n;
							return p;
					case MIN : 
              n = subexpression[0].Get_Maximal_Value(min,max);
							for (int i = 1; i < varvalarity; i++)
							{
								m = subexpression[i].Get_Minimal_Value(min,max);
								if (m < n)
									n = m;
							}
							return n;
					case MAX :
							n = subexpression[0].Get_Maximal_Value(min,max);
              for (int i = 1; i < varvalarity; i++)
							{
								m = subexpression[i].Get_Maximal_Value(min,max);
								if (m > n)
									n = m;
							}
							return n;
					case SET : 
							return varvalarity;
					default: 
							return 1;
				}
				break;
		case NIL : return 0;				
	}
	return 0;
}
