/*
 * gen_fsm_header.h
 *
 *  Created on: 7 oct. 2016
 *  Created by: Huu Nghia NGUYEN <huunghia.nguyen@montimage.com>
 *
 *  This is header of a plugin generated by main_gen_plugin.
 *  A plugin contains the encoding of one or many rules.
 */

#ifndef SRC_LIB_PLUGIN_HEADER_H_
#define SRC_LIB_PLUGIN_HEADER_H_
#include <stdint.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>

#include "message_t.h"
#include "mmt_array_t.h"

typedef struct proto_attribute_struct{
	/**
	 * Protocol name
	 */
	const char *proto;
	/**
	 * Attribute name
	 */
	const char *att;
	/**
	 * Protocol ID
	 */
	uint32_t proto_id;
	/**
	 * Attribute ID
	 */
	uint32_t att_id;
	/**
	 * Data type defined by MMT-Security such as MMT_SEC_MSG_DATA_TYPE_NUMERIC, MMT_SEC_MSG_DATA_TYPE_STRING
	 */
	int data_type;
	/**
	 * data type defined by MMT-DPI such as MMT_U8_DATA, MMT_DATA_IP_ADDR, ...
	 */
	int dpi_type;
}proto_attribute_t;

/**
 * Information of the moment the rules was created
 */
typedef struct rule_version_info_struct{
	/**
	 * Moment C code of the rule was generated
	 */
	time_t created_date;
	/**
	 * A hash string represent GIT version of source code from that the rule_compile was created
	 */
	const char *hash;
	/**
	 * Version number in string format, e.g., 1.2.18
	 */
	const char *number;
	/**
	 * A number represent the number string above, e.g., 56
	 * A recent version must has an index that is greater than the one of older version
	 */
	uint32_t index;
	/**
	 * Version string of MMT-DPI
	 */
	const char *dpi;
}rule_version_info_t;

struct rule_info_struct;
/**
 * A function to be called when a rule is validated
 */
typedef void (*mmt_rule_satisfied_callback)(
		const struct rule_info_struct *rule,		//rule being validated
		int verdict,		             //DETECTED, NOT_RESPECTED
		uint64_t timestamp,  			 //moment (by time) the rule is validated
		uint64_t counter,					 //moment (by order of packet) the rule is validated
		const mmt_array_t * const trace//historic of messages that validates the rule
);

/**
 * Information of a rule in generated lib
 */
typedef struct rule_info_struct{
	uint32_t id;
	/** rule type */
	int type_id;
	/** rule type that is either "attack", "security", "test", "evasion"*/
	const char *type_string;

	/**
	 * Description of the rule
	 */
	const char *description;
	/** a command to be executed when a rule is satisfied */
	mmt_rule_satisfied_callback if_satisfied;
	/** a command to be executed when a rule is not satisfied */
	const char *if_not_satisfied;

	/** Size of #proto_atts */
	uint8_t proto_atts_count;

	/** number of events existing in the rules  */
	uint8_t events_count;

	/** Array of unique protocols and their attributes being used in this rule */
	const proto_attribute_t *proto_atts;
	/**
	 * Each array represents detail protocols and their attributes of each event.
	 * There are #events_count elements. Each element is an mmt_array_t
	 *  that represents unique proto.att of an event. Specifically,
	 *   - array.elements_count = number of proto.atts in the event
	 *   - array.data = array of pointers. Each pointer has type of proto_attribute_t
	 */
	const mmt_array_t *proto_atts_events;

	/**
	 * Each array represents the protocols and attributes will be excluded from its event mask.
	 * This means that MMT-Security will not check the presents of these proto.atts before verifying
	 * an event. (Such a checking is done normally to ensure all proto.atts are present)
	 */
	const mmt_array_t *excluded_filter;

	/** Create a FSM instance */
	void* (* create_instance )();

	/**
	 * Set a unique number to represent a proto.att.
	 * This function must be called firstly, inside #mmt_sec_init, before #create_instance.
	 * In any processing of this rule, e.g., verification of a transition guard, the rule will
	 * not use proto.att or proto_id and att_id to request its data
	 * but it will use this unique number.
	 *
	 * Note: This function is not thread-safe
	 */
	void (* hash_message )( const char *proto, const char *att, uint16_t index );

	/**
	 * Information at the moment the rule was created
	 */
	const rule_version_info_t *version;
}rule_info_t;

/**
 * Get information of rules in a generated plugin
 * - Output
 * 	+ rules_arr
 * - Return
 * 	+ number of rules
 */
size_t mmt_sec_get_plugin_info( const rule_info_t **plugins_arr );


#endif /* SRC_LIB_PLUGIN_HEADER_H_ */
