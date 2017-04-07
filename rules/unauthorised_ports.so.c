
 /** 886
  * This file is generated automatically on 2017-04-05 17:08:17
  */
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include "plugin_header.h"
 #include "mmt_fsm.h"
 #include "mmt_lib.h"
 
 /** 893
  * Embedded functions
  */
 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/**
 * Check whether a port is unauthorised
 * - return : 1 if the port is unauthorised
 *            0 if the port is authorised
 * according to: 
 *    https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
 * and
 *    https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.txt
 */
static inline int check_port(int i){
  switch( i ) {
    case 78:
    case 79:
    case 100:
    case 106:
    case 787:
    case 1053:
    case 1491:
    case 2662:
    case 3060:
    case 3131:
    case 3145:
    case 3300:
    case 3301:
    case 4045:
    case 4315:
    case 4443:
    case 4967:
    case 5151:
    case 5152:
    case 5162:
    case 5444:
    case 5555:
    case 5556:
    case 6100:
    case 6200:
    case 6501:
    case 8882:
    case 9001:
    case 6632:
    case 7001:
    case 7002:
    case 7005:
    case 7011:
    case 7012:
    case 7501:
    case 7777:
    case 8001:
    case 16000:
    case 49151:
      return 1;
    default:
      if(i<1023) return 0;
		if(i>49151 && i< 65536) return 0;
		if(i>65535) return 1;
		
		if(i>2193 && i<2197) return 1;
		if(i>4488 && i<4500) return 1;
		if(i>4953 && i<4969) return 1;
		if(i>5569 && i<5573) return 1;
		if(i>5646 && i<5670) return 1;
		if(i>6657 && i<6665) return 1;
		if(i>7491 && i<7500) return 1;
		if(i>7784 && i<7790) return 1;
		if(i>27999 && i<28119) return 1;
		if(i>5554 && i<5558) return 1;
		if(i>5999 && i<6064) return 1;
		if(i>8615 && i<8665) return 1;
		if(i>8801 && i<8804) return 1;
		if(i>8887 && i<8891) return 1;
		if(i>11430 && i<11489) return 1;
		if(i>11623 && i<11720) return 1;
		if(i>27009 && i<27345) return 1;
		if(i>41797 && i<42508) return 1;
		if(i>44444 && i<44544) return 1;
  }
  return 0;
}





 //======================================RULE 3======================================
 #define EVENTS_COUNT_3 2

 #define PROTO_ATTS_COUNT_3 3

 /** 828
  * Proto_atts for rule 3
  */
 
 static proto_attribute_t proto_atts_3[ PROTO_ATTS_COUNT_3 ] = {{.proto = "ip", .proto_id = 178, .att = "dst", .att_id = 13, .data_type = 1, .dpi_type = 8},
 {.proto = "ip", .proto_id = 178, .att = "src", .att_id = 12, .data_type = 1, .dpi_type = 8},
 {.proto = "tcp", .proto_id = 354, .att = "dest_port", .att_id = 2, .data_type = 0, .dpi_type = 2}};
 /** 840
  * Detail of proto_atts for each event
  */
 
 static mmt_array_t proto_atts_events_3[ 3 ] = { {.elements_count = 0, .data = NULL}, 
	 {//event_1
		 .elements_count = 1,
		 .data = (void* []) { &proto_atts_3[ 2 ] }
	 },
	 {//event_2
		 .elements_count = 2,
		 .data = (void* []) { &proto_atts_3[ 0 ] ,  &proto_atts_3[ 1 ] }
	 } 
 };//end proto_atts_events_

 /** 556
  * Structure to represent event data
  */
 typedef struct _msg_struct_3{
	 uint64_t timestamp;//timestamp
	 uint64_t counter;//index of packet
	 const char *ip_dst;
	 const char *ip_src;
	 const double *tcp_dest_port;
 }_msg_t_3;
 /** 593
  * Create an instance of _msg_t_3
  */
 static const void* _allocate_msg_t_3( const message_t *msg  ){
	 static __thread _msg_t_3 m;
	 m.ip_dst = get_element_data_message_t( msg, 178, 13 );
	 m.ip_src = get_element_data_message_t( msg, 178, 12 );
	 m.tcp_dest_port = get_element_data_message_t( msg, 354, 2 );
	 m.timestamp = msg->timestamp; //timestamp
	 m.counter   = msg->counter;   //index of packet
	 return &m; 
 }
 /** 523
  * Public API
  */
 static uint64_t hash_message_3( const void *data ){
	 uint64_t hash = 0;
	 size_t i;	 _msg_t_3 *msg = (_msg_t_3 *) data;
	 //if( msg == NULL ) return hash;

	 if( msg->tcp_dest_port != NULL )
		 hash  |= 2; //event_id = 1
	 if( msg->ip_dst != NULL && msg->ip_src != NULL )
		 hash  |= 4; //event_id = 2
	 return hash;
 }
 /** 95
  * Rule 3, event 1
  * TCP packet with non-authorized port number.
  */
 static inline int g_3_1( const void *event_data, const fsm_t *fsm ){
	 if( unlikely( event_data == NULL )) return 0;
	 const _msg_t_3 *his_data, *ev_data = (_msg_t_3 *) event_data;/* 62 */
	 if( unlikely( ev_data->tcp_dest_port == NULL )) return 0;
	 double tcp_dest_port = *( ev_data->tcp_dest_port );

	 return (check_port(tcp_dest_port) == 1);
 }
 
 /** 95
  * Rule 3, event 2
  * Print out src and dst of IP
  */
 static inline int g_3_2( const void *event_data, const fsm_t *fsm ){
	 if( unlikely( event_data == NULL )) return 0;
	 const _msg_t_3 *his_data, *ev_data = (_msg_t_3 *) event_data;/* 62 */
	 if( unlikely( ev_data->ip_dst == NULL )) return 0;
	 const char *ip_dst =  ev_data->ip_dst ;/* 62 */
	 if( unlikely( ev_data->ip_src == NULL )) return 0;
	 const char *ip_src =  ev_data->ip_src ;

	 return 0 != mmt_mem_cmp(ip_src , ip_dst);
 }
 
 /** 412
  * States of FSM for rule 3
  */
 
 /** 413
  * Predefine list of states: init, fail, pass, ...
  */
 static fsm_state_t s_3_0, s_3_1, s_3_2, s_3_3, s_3_4;
 /** 426
  * Initialize states: init, error, final, ...
  */
 static fsm_state_t
 /** 432
  * initial state
  */
  s_3_0 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,//init or final states
	 .description  = "C4_Analyse_3: Unauthorized port number.",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 1, //FSM_ACTION_CREATE_INSTANCE
	 .data         = NULL,
	 .transitions  = (fsm_transition_t[]){
		 /** 461 TCP packet with non-authorized port number. */
		 /** 463 A real event */
		 { .event_type = 1, .guard = &g_3_1, .action = 1, .target_state = &s_3_4}  //FSM_ACTION_CREATE_INSTANCE
	 },
	 .transitions_count = 1
 },
 /** 432
  * timeout/error state
  */
  s_3_1 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,//init or final states
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = NULL,
	 .transitions_count = 0
 },
 /** 432
  * pass state
  */
  s_3_2 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,//init or final states
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = NULL,
	 .transitions_count = 0
 },
 /** 432
  * inconclusive state
  */
  s_3_3 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,//init or final states
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = NULL,
	 .transitions_count = 0
 },
 /** 432
  * root node
  */
  s_3_4 = {
	 .delay        = {.time_min = 0LL, .time_max = 0LL, .counter_min = 0LL, .counter_max = 0LL},
	 .is_temporary = 1,
	 .description  = "C4_Analyse_3: Unauthorized port number.",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = (fsm_transition_t[]){
		 /** 463 Timeout event will fire this transition */
		 { .event_type = 0, .guard = NULL  , .action = 0, .target_state = &s_3_1}, //FSM_ACTION_DO_NOTHING
		 /** 461 Print out src and dst of IP */
		 /** 463 A real event */
		 { .event_type = 2, .guard = &g_3_2, .action = 2, .target_state = &s_3_2}  //FSM_ACTION_RESET_TIMER
	 },
	 .transitions_count = 2
 };
 /** 490
  * Create a new FSM for this rule
  */
 static void *create_new_fsm_3(){
		 return fsm_init( &s_3_0, &s_3_1, &s_3_2, &s_3_3, EVENTS_COUNT_3, sizeof( _msg_t_3 ) );//init, error, final, inconclusive, events_count
 }//end function

 //======================================GENERAL======================================
 /** 618
  * Information of 1 rules
  * PUBLIC API
  */
 size_t mmt_sec_get_plugin_info( const rule_info_t **rules_arr ){
	  static const rule_info_t rules[] = (rule_info_t[]){
		 {
			 .id               = 3,
			 .type_id          = 2,
			 .type_string      = "evasion",
			 .events_count     = EVENTS_COUNT_3,
			 .description      = "C4_Analyse_3: Unauthorized port number.",
			 .if_satisfied     = NULL,
			 .if_not_satisfied = NULL,
			 .proto_atts_count = PROTO_ATTS_COUNT_3,
			 .proto_atts       = proto_atts_3,
			 .proto_atts_events= proto_atts_events_3,
			 .create_instance  = &create_new_fsm_3,
			 .convert_message  = &_allocate_msg_t_3,
			 .message_size     = sizeof( _msg_t_3 ),
			 .hash_message     = &hash_message_3,
			 .version          = {.created_date=1491404897, .hash = "f9f1d61", .number="1.0.2", .index=1000200, .dpi="1.6.7.0 (1039f64)"},
		 }
	 };
	 *rules_arr = rules;
	 return 1;
 }
 /** 658
  * Moment the rules being encoded
  * PUBLIC API
  */
 const char * __get_generated_date(){ return "2017-04-05 17:08:17, mmt-security version 1.0.2 (f9f1d61 - Apr  5 2017 16:44:04)";};