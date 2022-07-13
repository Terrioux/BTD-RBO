/// \file 
/// \brief Definitions a collection of functions for managing expressions
/// \ingroup tools

#ifndef EXPRESSION_TOOLS_H
#define EXPRESSION_TOOLS_H

#include "expression.h"
#include "csp.h"

unsigned long compute_allowed_tuple_number (vector<Variable *> & var, int arity, Expression & e);		///< returns the number of tuples of the Cartesian product of the domains of the variables in var which are allowed by the expression e
unsigned long compute_sampled_allowed_tuple_number (vector<Variable *> & var, int arity, Expression & e, int sample_size);	///< returns the number of tuples among a sample of sample_size tuples from the Cartesian product of the domains of the variables in var which are allowed by the expression e

Expression  string_to_expression (string expression, CSP * pb, vector<Variable *> & scope); ///< converts a string to an expression

#endif
