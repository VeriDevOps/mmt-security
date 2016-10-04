/*
 * expression.c
 *
 *  Created on: 21 sept. 2016
 *  Created by: Huu Nghia NGUYEN <huunghia.nguyen@montimage.com>
 */

#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include "base.h"
#include "mmt_utils.h"
#include "expression.h"
#include "mmt_alloc.h"
#include "mmt_log.h"

#define MAX_STRING_SIZE 1000

size_t str_trim( uint8_t *string, size_t size ){
	return size;
}
/**
 * public API
 */
size_t get_variables_inside_expression( const expression_t *expr ){
	size_t size = 0;
	return size;
}

/** Public API */
constant_t *expr_create_a_constant( enum data_type type, size_t data_size, void *data ){
	constant_t *cont = mmt_malloc( sizeof( constant_t ));
	cont->type = type;
	cont->data_size = data_size;
	cont->data = data;
	return cont;
}

/** Public API */
variable_t *expr_create_a_variable( char *proto, char *attr, uint8_t ref_index ){
	variable_t *var = mmt_malloc( sizeof( variable_t ));
	var->proto = proto;
	var->att   = attr;
	var->ref_index = ref_index;
	var->type  = UNKNOWN;
	return var;
}

/** Public API */
operation_t *expr_create_an_operation( char *name, enum operator operator ){
	operation_t *opt = mmt_malloc( sizeof( operation_t ));
	opt->name = name;
	opt->operator = operator;
	opt->params_list = NULL;
	opt->data_type_id = UNKNOWN;
	opt->params_size = 0;
	return opt;
}

/** Public API */
expression_t *expr_create_an_expression( enum expression type, void *data ){
	expression_t *expr = mmt_malloc( sizeof( expression_t));
	expr->type   = type;
	expr->father = NULL;
	switch( type ){
		case VARIABLE:
			expr->variable = data;
			break;
		case CONSTANT:
			expr->constant = data;
			break;
		case OPERATION:
			expr->operation = data;
			break;
	}
	return expr;
}

/** Public API */
void expr_free_a_constant( constant_t *x, enum bool free_data){
	if( free_data == YES )
		mmt_free_and_assign_to_null( x->data );
	mmt_free_and_assign_to_null( x );
}

/** Public API */
void expr_free_a_variable( variable_t *x, enum bool free_data){
	if( free_data == YES ){
		mmt_free_and_assign_to_null( x->proto );
		mmt_free_and_assign_to_null( x->att );
	}
	mmt_free_and_assign_to_null( x );
}

/** Public API */
void expr_free_an_operation( operation_t *x, enum bool free_data){
	link_node_t *ptr, *q;
	//free data and parameters of this operation
	if( free_data == YES){
		mmt_free_and_assign_to_null( x->name );

		ptr = x->params_list;
		while( ptr != NULL ){
			q = ptr;
			ptr = ptr->next;

			//free data of a node of linked-list
			expr_free_an_expression( (expression_t *) q->data, free_data );
			q->data = NULL;
			//free a node of linked-list
			mmt_free_and_assign_to_null( q );
			q = NULL;
		}
	}
	mmt_free_and_assign_to_null( x );
}

/** Public API */
void expr_free_an_expression( expression_t *expr, enum bool free_data){
	switch( expr->type ){
		case VARIABLE:
			expr_free_a_variable( expr->variable, free_data);
			break;
		case CONSTANT:
			expr_free_a_constant( expr->constant, free_data );
			break;
		case OPERATION:
			expr_free_an_operation( expr->operation, free_data );
			break;
	}
	mmt_free_and_assign_to_null( expr );
}

/**
 * Get index of the first character in string such that it is not a space
 * - Return:
 * 	+ 0 if there is no space
 * 	+ str_size if the string contains only space
 * 	+ index where the character is not a space
 */
inline size_t _jump_space( const char *string, size_t str_size ){
	size_t i=0;
	if( (string == NULL) || (str_size == 0) )
			return 0;
	//a pointer created by mmt_malloc always have \0 at the end
	for( i=0; i<str_size; i++ )
		if( ! isspace( string[i] ) )
			return i;
	return 0;
}


size_t _parse_a_name( char **name, const char *string, size_t str_size ){
	size_t i = 0, index = 0;
	const char *temp;
	*name = NULL;
	if( string == NULL || str_size == 0 )
		return 0;

	index = _jump_space( string, str_size );

	//all of string are spaces
	if( index == str_size ) return index;

	temp = string + index;

	//first character must be an alphabet
	if( ! (isalpha(*temp ) || *temp == '_') )
		return index;

	//the next characters can be alphabet or digit
	while( (isalnum( *(temp+i) ) || *(temp +i) == '_' ) && index < str_size ){
		i++;
		index ++;
	}

	//duplicate the name
	*name = mmt_mem_dup( temp, i );
	return index;
}

/**
 * Parse a string that is put inside by ' ' or " "
 */
size_t _parse_a_string( char **name, const char *string, size_t str_size ){
	size_t i = 0, index = 0;
	const char *temp;
	*name = NULL;

	if( string == NULL || str_size == 0 )
		return 0;

	index = _jump_space( string, str_size );

	//all of string are spaces
	if( index == str_size ) return index;

	temp = string + index;

	//first character must be the open bracket: " or '
	if( *temp  != '"' && *temp != '\'')
		return index;

	i = 1;
	index ++;
	//find a close bracket
	while( *(temp+i) != *temp && index < str_size ){
		i++;
		index ++;
	}

	//must be found the closer
	mmt_assert( index < str_size, "Error 3.a: String is in incorrect format: %s", temp );

	//duplicate the name
	*name = mmt_mem_dup( temp + 1, i - 1 );

	//jump over the closer
	return index + 1;
}

/**
 * Parse a number: integer or float
 */
enum bool _parse_a_number( double **num, const char *string, size_t str_size ){
	size_t i = 0, index = 0;
	enum bool has_dot = NO;
	const char *temp;
	char *str;
	*num = NULL;
	if( string == NULL || str_size == 0 )
		return 0;

	index = _jump_space( string, str_size );

	//all of string are spaces
	if( index == str_size ) return index;

	temp = string + index;
	//first character must be an number
	if( ! isdigit(*temp ))
		return index;

	//the next characters can be a dot or a digit
	while( (isdigit(*(temp+i)) || *(temp+i) == '.' ) && index < str_size ){
		if( *(temp+i) == '.' ){
			//already saw a dot then we break at the second dot
			if( has_dot == YES )
				break;
			else
				has_dot = YES;
		}

		i++;
		index ++;
	}

	//duplicate the name
	str  = mmt_mem_dup( temp, i );
	*num  = mmt_malloc( sizeof( double ));
	**num = atof( str );
	mmt_free_and_assign_to_null( str );

	return index;
}

/**
 * get number of digits of an integer
 */
size_t _num_digits( int n ){
	if (n < 0) return _num_digits ((n == INT_MIN) ? INT_MAX : -n);
	if (n < 10) return 1;
	return 1 + _num_digits (n / 10);
}

/**
 * A constant is either a number or a string
 */
size_t _parse_constant( constant_t **expr, const char *string, size_t str_size ){
	size_t i = 0, index;
	char *name = NULL;
	double *number = NULL;
	constant_t *cont;
	*expr = NULL;
	if( string == NULL  || str_size == 0 )
		return 0;

	index = _parse_a_number( &number, string, str_size );
	//found a number
	if( number != NULL ){
		*expr = expr_create_a_constant(NUMERIC, sizeof( double), number);
		return index;
	}

	//not found any number => find a string
	index = _parse_a_string( &name, string, str_size );
	if( name != NULL ){
		*expr = expr_create_a_constant(STRING, mmt_mem_size( name ), name);
		return index;
	}
	return index;
}

/**
 * Parse a variable that is in format: proto.att[.index], e.g., TCP.SRC or TCP.SRC.1
 */
size_t _parse_variable( variable_t **expr, const char *string, size_t str_size ){
	size_t index, i = 0, j, k;
	char *str_1 = NULL, *str_2 = NULL;
	char const *temp;
	double *num = NULL;
	variable_t *var;
	enum bool has_reference = NO;

	*expr = NULL;

	index = _jump_space( string, str_size );
	if( (string == NULL) || (str_size == 0) || index == str_size )
		return index;

	temp = string + index;
	index = _parse_a_name( &str_1, temp, str_size );
	if( str_1 != NULL ){
		//must have a dot
		if( string[ index ] == '.' ){
			index ++; //jump over this dot
			temp = string + index ;
			index += _parse_a_name( &str_2, temp,  str_size - index );
			if( str_2 != NULL ){
				var = mmt_malloc( sizeof( variable_t ));
				*expr = var;
				var->proto = str_1;
				var->att   = str_2;

				//has index ?
				if( string[ index ] == '.' ){
					index ++; //jump over the second dot
					temp = string + index; //2 dots
					index += _parse_a_number( &num, temp, str_size - index );
					var->ref_index = (uint8_t ) (*num);
					mmt_free_and_assign_to_null( num );
				}else
					var->ref_index = UNKNOWN;
				var->type = UNKNOWN;
			}
			else
				mmt_free_and_assign_to_null( str_1 );
		}
	}

	return index;
}

inline char _get_the_next_char( const char *string ){
	while( isspace( *string )) string ++;
	return *string;
}

enum bool _parse_a_boolean_expression( enum bool is_first_time, expression_t *expr, const char *string ){
	//parse expression and create sub-tree, expr->operator = top operator
	//((ARP.OPCODE == 2)&&(ARP.SRC_PROTO == ARP.SRC_PROTO.1))
	//OR, AND, NEQ, EQ, GT, GTE, LT, LTE, THEN, COMPUTE, XC, XCE, XD, XDE, XE, ADD, SUB, MUL, DIV
	size_t index;

	const char *temp = string;
	const char *temp2 = string;

	expression_t *new_expr;
	operation_t *new_op;
	variable_t *new_var;
	constant_t *new_cont;
	char *new_string;

	//jump
	while ( isspace(*temp))temp++;

	if (*temp == '(') {
		temp2 = temp + 1;
		if ( is_first_time == YES) {
			_parse_a_boolean_expression(NO, expr, temp2);
		} else {
			if( expr->type == OPERATION && expr->operation->operator == FUNCTION ){
				_parse_a_boolean_expression(NO, expr, temp2);
			}else{

				//create new_expr
				//we have not known yet the operator of new_op
				//it will be determined after
				new_op = expr_create_an_operation(NULL, UNKNOWN );
				new_expr = expr_create_an_expression( OPERATION, new_op );
				new_expr->father = expr;
				//append new_expr to expr->params_list
				expr->operation->params_list = append_node_to_link_list( expr->operation->params_list, new_expr );
				expr->operation->params_size ++;

				_parse_a_boolean_expression(NO, new_expr, temp2);
			}
		}
	} else if (*temp == ')') {
		mmt_assert( expr->father != NULL || _get_the_next_char(temp + 1) == '\0', "Error 37d: Unexpected: %s", temp + 1 );
		_parse_a_boolean_expression(NO, expr->father, temp + 1);
	} else if (*temp == '\0') {
		//do nothing
	}  else if (*temp == '\'') {
		//a 'string'
		index = _parse_constant( &new_cont, temp, MAX_STRING_SIZE );

		new_expr = expr_create_an_expression( CONSTANT, new_cont );
		new_expr->father = expr;

		//append new_expr to expr->params_list
		expr->operation->params_list = append_node_to_link_list( expr->operation->params_list, new_expr );
		expr->operation->params_size ++;
		_parse_a_boolean_expression(NO, expr, temp + index);
	} else if (isalpha(*temp) || *temp == '_') {
		//a variable: PROTO.FIELD.EVENT
		index = _parse_variable( &new_var, temp, MAX_STRING_SIZE );
		mmt_assert( new_var != NULL, "Error 37c: Illegal variable name: %s", temp );
		new_expr = expr_create_an_expression( VARIABLE, new_var );
		new_expr->father = expr;
		//append new_expr to expr->params_list
		expr->operation->params_list = append_node_to_link_list( expr->operation->params_list, new_expr );
		expr->operation->params_size ++;
		_parse_a_boolean_expression(NO, expr, temp + index);
	} else if (*temp == '#') {
		//an embedded function: #func(param_1, param_2)
		temp ++;
		index = _parse_a_name( &new_string, temp, MAX_STRING_SIZE );

		new_op = expr_create_an_operation( new_string, FUNCTION );
		new_expr = expr_create_an_expression( OPERATION, new_op );
		new_expr->father = expr;

		//append new_expr to expr->params_list
		expr->operation->params_list = append_node_to_link_list( expr->operation->params_list, new_expr );
		expr->operation->params_size ++;
		//parse the parameters of this function
		_parse_a_boolean_expression(NO, new_expr, temp + index );
	} else if (isdigit(*temp)) {
		//a number 1.0
		index = _parse_constant( &new_cont, temp, MAX_STRING_SIZE );
		//create a new expression
		new_expr = expr_create_an_expression( CONSTANT, new_cont );
		new_expr->father = expr;
		//append new_expr to expr->params_list
		expr->operation->params_list = append_node_to_link_list( expr->operation->params_list, new_expr );
		expr->operation->params_size ++;
		_parse_a_boolean_expression(NO, expr, temp + index);
	} else if (*temp == '&' && *(temp + 1) == '&') {
		// &&
		temp2 = temp + 2;
		expr->type = OPERATION;
		expr->operation->operator = AND;
		expr->operation->name = mmt_mem_dup( "&&", 2);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '|' && *(temp + 1) == '|') {
		// &&
		temp2 = temp + 2;
		expr->type = OPERATION;
		expr->operation->operator = OR;
		expr->operation->name = mmt_mem_dup( "||", 2);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '=' && *(temp + 1) == '=') {
		// ==
		temp2 = temp + 2;
		// set value
		expr->type = OPERATION;
		expr->operation->operator = EQ;
		expr->operation->name = mmt_mem_dup( "==", 2);
		//continue
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '!' && *(temp + 1) == '=') {
		// !=
		temp2 = temp + 2;
		expr->type = OPERATION;
		expr->operation->operator = NEQ;
		expr->operation->name = mmt_mem_dup( "!=", 2);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '>' && *(temp + 1) != '=') {
		// >
		temp2 = temp + 1;
		expr->type = OPERATION;
		expr->operation->operator = GT;
		expr->operation->name = mmt_mem_dup( ">", 1);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '>' && *(temp + 1) == '=') {
		// >=
		temp2 = temp + 2;
		expr->type = OPERATION;
		expr->operation->operator = GTE;
		expr->operation->name = mmt_mem_dup( ">=", 2);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '<' && *(temp + 1) != '=') {
		// <
		temp2 = temp + 1;
		expr->type = OPERATION;
		expr->operation->operator = LT;
		expr->operation->name = mmt_mem_dup( "<", 1);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '<' && *(temp + 1) == '=') {
		// <=
		temp2 = temp + 2;
		expr->type = OPERATION;
		expr->operation->operator = LTE;
		expr->operation->name = mmt_mem_dup( "<=", 2);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '+') {
		// +
		temp2 = temp + 1;
		expr->type = OPERATION;
		expr->operation->operator = ADD;
		expr->operation->name = mmt_mem_dup( "+", 1);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '-') {
		// -
		temp2 = temp + 1;
		expr->type = OPERATION;
		expr->operation->operator = SUB;
		expr->operation->name = mmt_mem_dup( "-", 1);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '*') {
		// *
		temp2 = temp + 1;
		expr->type = OPERATION;
		expr->operation->operator = MUL;
		expr->operation->name = mmt_mem_dup( "*", 1);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if (*temp == '/') {
		// '/'
		temp2 = temp + 1;
		expr->type = OPERATION;
		expr->operation->operator = DIV;
		expr->operation->name = mmt_mem_dup( "/", 1);
		_parse_a_boolean_expression(NO, expr, temp2);
	} else if( *temp == ',' && expr->type == OPERATION && expr->operation->operator == FUNCTION ){
		temp ++;
		while( isspace( *temp)) temp ++;
		//waiting for another parameter
		mmt_assert( *temp != ')', "Error 37b: Illegal delimiter of function %s: ,%c", expr->operation->name, *temp );
		//*tmp != ')' ==> continue parsing the next parameter of function
		_parse_a_boolean_expression(NO, expr, temp);
	}else{
		(void)fprintf(stderr, "Error 37: Illegal character found in boolean expression: %c%c.\n", *temp, *(temp + 1));
		exit(-1);
	}
	return 0;
}

/**
 * public API
 */
int parse_expression( expression_t **expr, const char *string, size_t str_size ){
	expression_t *ret = NULL;
	operation_t *new_op;
	*expr = NULL;
	if( string == NULL )
		return 0;

	//we have not known yet the operator of new_op
	//it will be determined after
	new_op = expr_create_an_operation(NULL, UNKNOWN );
	ret    = expr_create_an_expression( OPERATION, new_op );
	*expr  = ret;
	return _parse_a_boolean_expression( YES, ret, string );
}


/**
 * public API
 */
size_t expr_stringify_constant( char **string, const constant_t *expr){
	char buff[10];
	size_t size;
	double d;

	if( expr == NULL ){
		string = NULL;
		return 0;
	}

	if( expr->type == NUMERIC ){
		d = *(double *)expr->data;
		//integer
		if(  d- (int)d == 0 )
			sprintf(buff, "%d", (int) d);
		else
			sprintf(buff, "%.2f", d);
		size = strlen( buff );
		*string = mmt_mem_dup( buff, size );
		return size;
	}else{
		*string = mmt_mem_dup( expr->data, expr->data_size );
		return expr->data_size;
	}
}

/**
 * public API
 */
size_t expr_stringify_variable( char **string, const variable_t *var){
	size_t size = 0;
	*string = NULL;

	if( var == NULL ){
		return 0;
	}

	size = mmt_mem_size( var->proto ) + mmt_mem_size( var->att ) + 1;//1 for .


	if( var->ref_index != (uint8_t)UNKNOWN ){
		size += 1 + _num_digits( var->ref_index ); //1 byte for .
		*string = mmt_malloc( size ); //mmt_malloc allocates size+1
		snprintf(*string, size+1, "%s_%s_%d", var->proto, var->att, var->ref_index); //+1 for null character
	}else{
		*string = mmt_malloc( size ); //mmt_malloc allocates size+1
		snprintf(*string, size+1, "%s_%s", var->proto, var->att ); //+1 for null character
	}

	return size;
}

/**
 * Public API
 */
size_t expr_stringify_operation( char **string, const operation_t *opt ){
	char *tmp, *root, *str;
	const char *delim;
	link_node_t *ptr;
	size_t size, delim_size = 0, new_size = 0;

	root = mmt_malloc( 10000 );
	*string = root;

	if( opt->operator == FUNCTION ){
		sprintf( root, "%s(", opt->name );
		new_size = mmt_mem_size( opt->name ) + 1; //+1 for '('
	}else{
		sprintf( root, "(" );
		new_size = 1;
	}

	//delimiter
	if( opt->operator == FUNCTION ){
		delim = ",";
		delim_size = 1;
	}else{
		delim = opt->name;
		delim_size = mmt_mem_size( delim );
	}

	//parameters
	ptr = opt->params_list;
	while( ptr != NULL ){
		size = expr_stringify_expression( &tmp, (expression_t *) ptr->data );
		//the last parameter ==> no need delimiter but a close-bracket
		if( ptr->next == NULL ){
			size += 1; ////close bracket
			snprintf(root + new_size, size+1, "%s)", tmp);
		}else{
			size += delim_size + 2; //+2 for spaces around the delimiter
			snprintf(root + new_size, size+1, "%s %s ", tmp, delim );
		}
		new_size += size;
		//tmp was created in expr_stringify_expression( &tmp ...
		mmt_free_and_assign_to_null( tmp );
		ptr = ptr->next;
	};


	//clone string
	*string = mmt_mem_dup( root, new_size );
	mmt_free_and_assign_to_null( root );

	return new_size;
}
/**
 * public API
 */
size_t expr_stringify_expression( char **string, const expression_t *expr){
	//nothing to do
	if( expr == NULL ){
		string = NULL;
		return 0;
	}
	switch( expr->type ){
	case CONSTANT:
		return expr_stringify_constant( string, expr->constant );
	case VARIABLE:
		return expr_stringify_variable( string, expr->variable );
	case OPERATION:
		return expr_stringify_operation( string, expr->operation );
	default:
		mmt_debug( "Undefined" );
		return 0;
	}
}

/**
 * Compare 2 variables by its "proto" and "att"
 */
int _compare_variable_name( const void *v1, const void *v2){
	variable_t *x = (variable_t *)v1, *y = (variable_t *)v2;
	int d1, d2;
	mmt_assert( v1 != NULL && v2 != NULL, "Error: Variables are NULL" );
	d1 = strcmp( x->proto, y->proto );
	d2 = strcmp( x->att,   y->att );
	if( d1 == 0 && d2 == 0 )
		return 0;
	else if( d1 != 0 )
		return d1;
	else
		return d2;
}

int _compare_variable_name_and_index( const void *v1, const void *v2){
	variable_t *x = (variable_t *)v1, *y = (variable_t *)v2;
	int d1, d2, d3;
	mmt_assert( v1 != NULL && v2 != NULL, "Error: Variables are NULL" );
	d1 = strcmp( x->proto, y->proto );
	d2 = strcmp( x->att,   y->att );
	d3 = x->ref_index - y->ref_index;
	if( d1 == 0 && d2 == 0 && d3 == 0 )
		return 0;
	else if( d1 != 0 )
		return d1;
	else if( d2 != 0 )
		return d2;
	else
		return d3;
}

size_t _get_unique_variables_of_expression( const expression_t *expr, mmt_map_t *map ){
	size_t var_count = 0;
	void *ptr;
	link_node_t *p;

	if( expr == NULL ) return 0;

	switch( expr->type ){
	case VARIABLE:
		ptr = mmt_map_set_data( map, expr->variable, expr->variable, NO );
		if( ptr == NULL )
			var_count ++;
		break;
	case CONSTANT:
		break;
	case OPERATION:
		p = expr->operation->params_list;
		//get variables in parameters of the operation
		while( p != NULL ){
			var_count += _get_unique_variables_of_expression( (expression_t *) p->data, map );
			p = p->next;
		}
		break;
	}
	return var_count;
}

/**
 * Public API
 */
size_t get_unique_variables_of_expression( const expression_t *expr, mmt_map_t **variables_map, enum bool has_index ){
	size_t var_count = 0;
	mmt_map_t *map;
	void *ptr;
	*variables_map = NULL;
	if( expr == NULL ) return 0;
	if( has_index == YES )
		map = mmt_map_init( _compare_variable_name_and_index );
	else
		map = mmt_map_init( _compare_variable_name );

	var_count = _get_unique_variables_of_expression( expr, map );

	//free the map being allocated
	if( var_count == 0 )
		mmt_map_free( map, NO );
	else
		*variables_map = map;

	return var_count;
}
/**
 * public API
 */
constant_t *evaluate_expression( const expression_t *expr, const constant_t **constants, size_t const_size ){
	constant_t *ret = (constant_t *) mmt_malloc( sizeof( constant_t ));
	return ret;
}
