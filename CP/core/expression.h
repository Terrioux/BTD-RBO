/// \file 
/// \brief Definitions related to the Expression class

#ifndef EXPRESSION_H
#define EXPRESSION_H

using namespace std;
#include <string>
#include <vector>
#include <tuple>

typedef enum op_type    /// lists the different possible operators
{
  NEG,    ///< the opposite
  ABS,    ///< the absolute value
  ADD,    ///< the binary addition
  SUM,    ///< the n-ary addition
  SUB,    ///< the subtraction
  MUL,    ///< the binary multiplication
  PROD,   ///< the n-ary multiplication
  DIV,    ///< the integer division
  MOD,    ///< the remainder
  SQR,    ///< the square
  POW,    ///< the power
  MIN,    ///< the minimum value
  MAX,    ///< the maximum value
  EQ,     ///< the equality for two values
  EQU,    ///< the equality for at least two values
  NE,     ///< the difference
  GE,     ///< greater than or equal
  GT,     ///< greater than 
  LE,     ///< less than or equal
  LT,     ///< less than 
  NOT,    ///< the logical not
  AND,    ///< the logical and
  OR,     ///< the logical or
  XOR,    ///< the logical xor
  IFF,    ///< the logical equivalence  
  IF,     ///< the alternative
  DIST,   ///< the distance (i.e. |x-y|)
  IMP,    ///< the logical implication
  SET,    ///< the set definition
  IN,     ///< the set operator in
  NOP     ///< the operator no-op
} operator_type;


typedef enum node_type    /// lists the different types of nodes
{
  OP,     ///< operator
  VAR,    ///< variable
  VAL,    ///< value
  NIL     ///< nil
} node_type;


class Expression        /// This class implements mathematical expressions \ingroup core
{
	protected:
		node_type type;			///< the type of the root node of the expression
		operator_type op;   ///< the operator related to the root node (if the root node is an operator node)
		int varvalarity;		///< the variable related to the root node (if the root node is a var node), the value (if the root node is a val node), the arity (if the root node is an operator node)
		Expression * subexpression;  ///< the subexpression of the current expression
				
	public:
		// constructors and destructor
		Expression (); 																 			///< constructs an empty expression (i.e. whose type is NIL)
		Expression (node_type nt, int var_val);				 			///< constructs an expression corresponding to a variable or to a value depending on nt
		Expression (operator_type oper, Expression * sub, int arity=0); 	///< constructs an expression corresponding to the operator oper whose operands are given by sub whose size is arity
		Expression (operator_type oper, vector<Expression> & sub); 	///< constructs an expression corresponding to the operator oper whose operands are given by sub
		Expression (Expression & e);								  		  ///< constructs an expression by copying the expression e
		Expression (const Expression & e);					  		  ///< constructs an expression by copying the expression e
		~Expression ();																 			///< destructor
		
		// operator
		Expression & operator= (const Expression & e);			///< operator =
		bool operator== (const Expression & e);							///< operator ==
		bool operator!= (const Expression & e);							///< operator !=
		
		// basic functions
		int Evaluate (int t []);					///< returns the evaluation of the expression such that the value t[i] is the effective value of the variable i
		void Change_Variable (vector<int> & effective_parameters, vector<node_type> & effective_parameter_type);	///< modifies the expression by providing effective value or changing the variable name for the variables of the current expression
		string Get_String ();							///< returns the string corresponding to the expression in the functional format
		node_type Get_Type ();						///< returns the type of the root node of the expression
		operator_type Get_Operator ();		///< returns the operator of the root node of the expression if the root node is an operator node
		Expression & Get_Subexpression (int i);		///< returns the i th subexpression of the root node of the expression if the root node is an operator node
		int Get_Arity();									///< returns the arity of the operators
		int Get_Variable ();							///< returns the variable number of the root node of the expression if the root node is a variable node
		int Get_Value ();									///< returns the value of the root node of the expression if the root node is a value node
    void Get_Postfix_Expression (vector<tuple<node_type,operator_type,int>> & postfix_expression);    ///< computes the equivalent postfix expression of the current expression
};


// definition of inline function

inline bool Expression::operator!= (const Expression & e)
// operator !=
/// \param[in] e the expression we want to compare
{
	return ! (*this == e);
}


inline int Expression::Evaluate (int t [])
// returns the evaluation of the expression such that the value t[i] is the effective value of the variable i
/// \param[in] t the value of each variable in the expression
{
	switch (type)
	{
		case VAL : return varvalarity;
		case VAR : return t[varvalarity];
		case OP:
				int n,m;
				n = subexpression[0].Evaluate(t);
				switch (op)
				{
 				  case NEG : return -n;
					case ABS : 
							if (n > 0)  return n;
							else return -n;
					case ADD : return n + subexpression[1].Evaluate (t);
					case SUM :
							for (int i = 1; i < varvalarity; i++)
								n += subexpression[i].Evaluate (t);
							return n;
					case SUB : return n-subexpression[1].Evaluate (t);
					case MUL : return n*subexpression[1].Evaluate (t);
					case PROD : 
							for (int i = 1; i < varvalarity; i++)
								n *= subexpression[i].Evaluate (t);
							return n;
					case DIST : n = n - subexpression[1].Evaluate (t);
											if (n > 0)  return n;
											else return -n;
					case DIV : return n / subexpression[1].Evaluate (t);
					case MOD : return n % subexpression[1].Evaluate (t);
					case EQ  : 
							if (n == subexpression[1].Evaluate (t)) return 1;
							else return 0;
					case EQU :
							m = 1;
							while ((m < varvalarity) && (n == subexpression[m].Evaluate (t)))
								m++;
							return m == varvalarity;
					case NE  :
							if (n != subexpression[1].Evaluate (t)) return 1;
							else return 0;
					case GE  :
							if (n >= subexpression[1].Evaluate (t)) return 1;
							else return 0;
					case GT  : 
							if (n > subexpression[1].Evaluate (t)) return 1;
							else return 0;
					case LE  :
							if (n <= subexpression[1].Evaluate (t)) return 1;
							else return 0;
					case LT  :
							if (n < subexpression[1].Evaluate (t)) return 1;
							else return 0;
					case NOT :
							if (n == 0) return 1;
							else return 0;
					case AND :
							for (int i = 1; (i < varvalarity) && (n != 0); i++)
								n = subexpression[i].Evaluate (t);
							return (n != 0);
					case OR  :
							for (int i = 1; (i < varvalarity) && (n == 0); i++)
								n = subexpression[i].Evaluate (t);
							return (n != 0);
					case SQR : 
							return n*n;
					case POW : 
							int p;
							m = subexpression[1].Evaluate (t);
							p = 1;
							for (int i = 0; i < m; i++)
								p *= n;
							return p;
					case MIN : 
							for (int i = 1; i < varvalarity; i++)
							{
								m = subexpression[i].Evaluate (t);
								if (m < n)
									n = m;
							}
							return n;
					case MAX :
							for (int i = 1; i < varvalarity; i++)
							{
								m = subexpression[i].Evaluate (t);
								if (m > n)
									n = m;
							}
							return n;
					case XOR : 
							if (n != 0)			// n is used as a counter which represents the number of true term
								n = 1;
							for (int i = 1; i < varvalarity; i++)
								if (subexpression[i].Evaluate (t) != 0)
									n++;
							return n % 2 == 1;
					case IFF : 
							m = 1;
							if (n == 0)
							{
								while ((m < varvalarity) && (subexpression[m].Evaluate (t) == 0))
									m++;
							}
							else
								{
									while ((m < varvalarity) && (subexpression[m].Evaluate (t) != 0))
										m++;
								}
							return (m == varvalarity);
					case IF  :
							if (n != 0)  return subexpression[1].Evaluate (t);
							else return subexpression[2].Evaluate (t);
					case IMP :
							if (n == 0)  return 1;
							else return subexpression[1].Evaluate (t);
					case SET : 
							return varvalarity;
					case IN  :
							m = subexpression[1].Evaluate (t);
							for (int i = 0; i < m; i++)
								if (n == subexpression[1].subexpression[i].Evaluate (t))
									return 1;
							return 0;
          case NOP: return 0;
				}
				break;
		case NIL : return 0;				
	}
	return 0;
}


inline node_type Expression::Get_Type ()
// returns the type of the root node of the expression
{
	return type;
}


inline operator_type Expression::Get_Operator ()
// returns the operator of the root node of the expression if the root node is an operator node
{
	if (type == OP)
		return op;
	else throw ("Error: illicit call to Get_Operator");
}


inline Expression & Expression::Get_Subexpression (int i)
// returns the i th subexpression of the root node of the expression if the root node is an operator node (i starts from 0)
/// \param[in] i the number of the subexpression we want to get
{
	if (type == OP)
	{
		if ((i >= 0) && (i < varvalarity))
			return subexpression[i];
		else throw ("Error: illicit call to Get_Subexpression");
	}
	else throw ("Error: illicit call to Get_Subexpression");
}


inline int Expression::Get_Arity ()
// returns the arity of the operator
{
	if (type == OP)
		return varvalarity;
	else throw ("Error: illicit call to Get_Arity");
}


inline int Expression::Get_Variable ()
// returns the variable number of the root node of the expression if the root node is a variable node
{
	if (type == VAR)
		return varvalarity;
	else throw ("Error: illicit call to Get_Variable");
}


inline int Expression::Get_Value ()
// returns the value of the root node of the expression if the root node is a value node
{
	if (type == VAL)
		return varvalarity;
	else throw ("Error: illicit call to Get_Value");
}


#endif
