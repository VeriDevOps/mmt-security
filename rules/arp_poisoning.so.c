
 /** 886
  * This file is generated automatically on 2017-04-05 17:07:59
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
 

 //======================================RULE 1======================================
 #define EVENTS_COUNT_1 3

 #define PROTO_ATTS_COUNT_1 4

 /** 828
  * Proto_atts for rule 1
  */
 
 static proto_attribute_t proto_atts_1[ PROTO_ATTS_COUNT_1 ] = {{.proto = "arp", .proto_id = 30, .att = "ar_op", .att_id = 5, .data_type = 0, .dpi_type = 2},
 {.proto = "arp", .proto_id = 30, .att = "ar_sha", .att_id = 6, .data_type = 1, .dpi_type = 6},
 {.proto = "arp", .proto_id = 30, .att = "ar_sip", .att_id = 7, .data_type = 1, .dpi_type = 8},
 {.proto = "arp", .proto_id = 30, .att = "ar_tip", .att_id = 9, .data_type = 1, .dpi_type = 8}};
 /** 840
  * Detail of proto_atts for each event
  */
 
 static mmt_array_t proto_atts_events_1[ 4 ] = { {.elements_count = 0, .data = NULL}, 
	 {//event_1
		 .elements_count = 2,
		 .data = (void* []) { &proto_atts_1[ 0 ] ,  &proto_atts_1[ 3 ] }
	 },
	 {//event_2
		 .elements_count = 3,
		 .data = (void* []) { &proto_atts_1[ 0 ] ,  &proto_atts_1[ 1 ] ,  &proto_atts_1[ 2 ] }
	 },
	 {//event_3
		 .elements_count = 3,
		 .data = (void* []) { &proto_atts_1[ 0 ] ,  &proto_atts_1[ 1 ] ,  &proto_atts_1[ 2 ] }
	 } 
 };//end proto_atts_events_

 /** 556
  * Structure to represent event data
  */
 typedef struct _msg_struct_1{
	 uint64_t timestamp;//timestamp
	 uint64_t counter;//index of packet
	 const double *arp_ar_op;
	 const char *arp_ar_sha;
	 const char *arp_ar_sip;
	 const char *arp_ar_tip;
 }_msg_t_1;
 /** 593
  * Create an instance of _msg_t_1
  */
 static const void* _allocate_msg_t_1( const message_t *msg  ){
	 static __thread _msg_t_1 m;
	 m.arp_ar_op = get_element_data_message_t( msg, 30, 5 );
	 m.arp_ar_sha = get_element_data_message_t( msg, 30, 6 );
	 m.arp_ar_sip = get_element_data_message_t( msg, 30, 7 );
	 m.arp_ar_tip = get_element_data_message_t( msg, 30, 9 );
	 m.timestamp = msg->timestamp; //timestamp
	 m.counter   = msg->counter;   //index of packet
	 return &m; 
 }
 /** 523
  * Public API
  */
 static uint64_t hash_message_1( const void *data ){
	 uint64_t hash = 0;
	 size_t i;	 _msg_t_1 *msg = (_msg_t_1 *) data;
	 //if( msg == NULL ) return hash;

	 if( msg->arp_ar_op != NULL && msg->arp_ar_tip != NULL )
		 hash  |= 2; //event_id = 1
	 if( msg->arp_ar_op != NULL && msg->arp_ar_sha != NULL && msg->arp_ar_sip != NULL )
		 hash  |= 4; //event_id = 2
	 if( msg->arp_ar_op != NULL && msg->arp_ar_sha != NULL && msg->arp_ar_sip != NULL )
		 hash  |= 8; //event_id = 3
	 return hash;
 }
 /** 95
  * Rule 1, event 1
  * An arp who was requested
  */
 static inline int g_1_1( const void *event_data, const fsm_t *fsm ){
	 if( unlikely( event_data == NULL )) return 0;
	 const _msg_t_1 *his_data, *ev_data = (_msg_t_1 *) event_data;/* 62 */
	 if( unlikely( ev_data->arp_ar_op == NULL )) return 0;
	 double arp_ar_op = *( ev_data->arp_ar_op );

	 return (arp_ar_op == 1);
 }
 
 /** 95
  * Rule 1, event 2
  * An arp reply with MAC address
  */
 static inline int g_1_2( const void *event_data, const fsm_t *fsm ){
	 if( unlikely( event_data == NULL )) return 0;
	 const _msg_t_1 *his_data, *ev_data = (_msg_t_1 *) event_data;/* 62 */
	 if( unlikely( ev_data->arp_ar_op == NULL )) return 0;
	 double arp_ar_op = *( ev_data->arp_ar_op );/* 62 */
	 if( unlikely( ev_data->arp_ar_sip == NULL )) return 0;
	 const char *arp_ar_sip =  ev_data->arp_ar_sip ;
	 his_data = (_msg_t_1 *)fsm_get_history( fsm, 1);
	 if( unlikely( his_data == NULL )) return 0;/* 62 */
	 if( unlikely( his_data->arp_ar_tip == NULL )) return 0;
	 const char *arp_ar_tip_1 =  his_data->arp_ar_tip ;

	 return ((arp_ar_op == 2) && 0 == mmt_mem_cmp(arp_ar_sip , arp_ar_tip_1));
 }
 
 /** 95
  * Rule 1, event 3
  * An arp reply but with different MAC address
  */
 static inline int g_1_3( const void *event_data, const fsm_t *fsm ){
	 if( unlikely( event_data == NULL )) return 0;
	 const _msg_t_1 *his_data, *ev_data = (_msg_t_1 *) event_data;/* 62 */
	 if( unlikely( ev_data->arp_ar_op == NULL )) return 0;
	 double arp_ar_op = *( ev_data->arp_ar_op );
	 his_data = (_msg_t_1 *)fsm_get_history( fsm, 2);
	 if( unlikely( his_data == NULL )) return 0;/* 62 */
	 if( unlikely( his_data->arp_ar_sha == NULL )) return 0;
	 const char *arp_ar_sha_2 =  his_data->arp_ar_sha ;/* 62 */
	 if( unlikely( ev_data->arp_ar_sha == NULL )) return 0;
	 const char *arp_ar_sha =  ev_data->arp_ar_sha ;/* 62 */
	 if( unlikely( ev_data->arp_ar_sip == NULL )) return 0;
	 const char *arp_ar_sip =  ev_data->arp_ar_sip ;/* 62 */
	 if( unlikely( his_data->arp_ar_tip == NULL )) return 0;
	 const char *arp_ar_tip_1 =  his_data->arp_ar_tip ;

	 return (((arp_ar_op == 2) && 0 == mmt_mem_cmp(arp_ar_sip , arp_ar_tip_1)) && 0 != mmt_mem_cmp(arp_ar_sha , arp_ar_sha_2));
 }
 
 /** 412
  * States of FSM for rule 1
  */
 
 /** 413
  * Predefine list of states: init, fail, pass, ...
  */
 static fsm_state_t s_1_0, s_1_1, s_1_2, s_1_3, s_1_4, s_1_5;
 /** 426
  * Initialize states: init, error, final, ...
  */
 static fsm_state_t
 /** 432
  * initial state
  */
  s_1_0 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,//init or final states
	 .description  = "IPv4 address conflict detection (RFC5227). Possible arp poisoning.",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 1, //FSM_ACTION_CREATE_INSTANCE
	 .data         = NULL,
	 .transitions  = (fsm_transition_t[]){
		 /** 461 An arp who was requested */
		 /** 463 A real event */
		 { .event_type = 1, .guard = &g_1_1, .action = 1, .target_state = &s_1_4}  //FSM_ACTION_CREATE_INSTANCE
	 },
	 .transitions_count = 1
 },
 /** 432
  * timeout/error state
  */
  s_1_1 = {
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
  s_1_2 = {
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
  s_1_3 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,//init or final states
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = NULL,
	 .transitions_count = 0
 }, s_1_4 = {
	 .delay        = {.time_min = 1LL, .time_max = 300000000LL, .counter_min = 0LL, .counter_max = 0LL},
	 .is_temporary = 0,
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = (fsm_transition_t[]){
		 /** 463 Timeout event will fire this transition */
		 { .event_type = 0, .guard = NULL  , .action = 0, .target_state = &s_1_3}, //FSM_ACTION_DO_NOTHING
		 /** 461 An arp reply with MAC address */
		 /** 463 A real event */
		 { .event_type = 2, .guard = &g_1_2, .action = 2, .target_state = &s_1_5}  //FSM_ACTION_RESET_TIMER
	 },
	 .transitions_count = 2
 },
 /** 432
  * root node
  */
  s_1_5 = {
	 .delay        = {.time_min = 1LL, .time_max = 300000000LL, .counter_min = 0LL, .counter_max = 0LL},
	 .is_temporary = 0,
	 .description  = "IPv4 address conflict detection (RFC5227). Possible arp poisoning.",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = (fsm_transition_t[]){
		 /** 463 Timeout event will fire this transition */
		 { .event_type = 0, .guard = NULL  , .action = 0, .target_state = &s_1_1}, //FSM_ACTION_DO_NOTHING
		 /** 461 An arp reply but with different MAC address */
		 /** 463 A real event */
		 { .event_type = 3, .guard = &g_1_3, .action = 2, .target_state = &s_1_2}  //FSM_ACTION_RESET_TIMER
	 },
	 .transitions_count = 2
 };
 /** 490
  * Create a new FSM for this rule
  */
 static void *create_new_fsm_1(){
		 return fsm_init( &s_1_0, &s_1_1, &s_1_2, &s_1_3, EVENTS_COUNT_1, sizeof( _msg_t_1 ) );//init, error, final, inconclusive, events_count
 }//end function

 //======================================RULE 2======================================
 #define EVENTS_COUNT_2 2

 #define PROTO_ATTS_COUNT_2 3

 /** 828
  * Proto_atts for rule 2
  */
 
 static proto_attribute_t proto_atts_2[ PROTO_ATTS_COUNT_2 ] = {{.proto = "arp", .proto_id = 30, .att = "ar_op", .att_id = 5, .data_type = 0, .dpi_type = 2},
 {.proto = "arp", .proto_id = 30, .att = "ar_sha", .att_id = 6, .data_type = 1, .dpi_type = 6},
 {.proto = "arp", .proto_id = 30, .att = "ar_sip", .att_id = 7, .data_type = 1, .dpi_type = 8}};
 /** 840
  * Detail of proto_atts for each event
  */
 
 static mmt_array_t proto_atts_events_2[ 3 ] = { {.elements_count = 0, .data = NULL}, 
	 {//event_1
		 .elements_count = 3,
		 .data = (void* []) { &proto_atts_2[ 0 ] ,  &proto_atts_2[ 1 ] ,  &proto_atts_2[ 2 ] }
	 },
	 {//event_2
		 .elements_count = 3,
		 .data = (void* []) { &proto_atts_2[ 0 ] ,  &proto_atts_2[ 1 ] ,  &proto_atts_2[ 2 ] }
	 } 
 };//end proto_atts_events_

 /** 556
  * Structure to represent event data
  */
 typedef struct _msg_struct_2{
	 uint64_t timestamp;//timestamp
	 uint64_t counter;//index of packet
	 const double *arp_ar_op;
	 const char *arp_ar_sha;
	 const char *arp_ar_sip;
 }_msg_t_2;
 /** 593
  * Create an instance of _msg_t_2
  */
 static const void* _allocate_msg_t_2( const message_t *msg  ){
	 static __thread _msg_t_2 m;
	 m.arp_ar_op = get_element_data_message_t( msg, 30, 5 );
	 m.arp_ar_sha = get_element_data_message_t( msg, 30, 6 );
	 m.arp_ar_sip = get_element_data_message_t( msg, 30, 7 );
	 m.timestamp = msg->timestamp; //timestamp
	 m.counter   = msg->counter;   //index of packet
	 return &m; 
 }
 /** 523
  * Public API
  */
 static uint64_t hash_message_2( const void *data ){
	 uint64_t hash = 0;
	 size_t i;	 _msg_t_2 *msg = (_msg_t_2 *) data;
	 //if( msg == NULL ) return hash;

	 if( msg->arp_ar_op != NULL && msg->arp_ar_sha != NULL && msg->arp_ar_sip != NULL )
		 hash  |= 2; //event_id = 1
	 if( msg->arp_ar_op != NULL && msg->arp_ar_sha != NULL && msg->arp_ar_sip != NULL )
		 hash  |= 4; //event_id = 2
	 return hash;
 }
 /** 95
  * Rule 2, event 1
  * An arp reply with MAC address
  */
 static inline int g_2_1( const void *event_data, const fsm_t *fsm ){
	 if( unlikely( event_data == NULL )) return 0;
	 const _msg_t_2 *his_data, *ev_data = (_msg_t_2 *) event_data;/* 62 */
	 if( unlikely( ev_data->arp_ar_op == NULL )) return 0;
	 double arp_ar_op = *( ev_data->arp_ar_op );

	 return (arp_ar_op == 2);
 }
 
 /** 95
  * Rule 2, event 2
  * An arp reply but with different MAC address
  */
 static inline int g_2_2( const void *event_data, const fsm_t *fsm ){
	 if( unlikely( event_data == NULL )) return 0;
	 const _msg_t_2 *his_data, *ev_data = (_msg_t_2 *) event_data;/* 62 */
	 if( unlikely( ev_data->arp_ar_op == NULL )) return 0;
	 double arp_ar_op = *( ev_data->arp_ar_op );
	 his_data = (_msg_t_2 *)fsm_get_history( fsm, 1);
	 if( unlikely( his_data == NULL )) return 0;/* 62 */
	 if( unlikely( his_data->arp_ar_sha == NULL )) return 0;
	 const char *arp_ar_sha_1 =  his_data->arp_ar_sha ;/* 62 */
	 if( unlikely( ev_data->arp_ar_sha == NULL )) return 0;
	 const char *arp_ar_sha =  ev_data->arp_ar_sha ;/* 62 */
	 if( unlikely( his_data->arp_ar_sip == NULL )) return 0;
	 const char *arp_ar_sip_1 =  his_data->arp_ar_sip ;/* 62 */
	 if( unlikely( ev_data->arp_ar_sip == NULL )) return 0;
	 const char *arp_ar_sip =  ev_data->arp_ar_sip ;

	 return (((arp_ar_op == 2) && 0 == mmt_mem_cmp(arp_ar_sip , arp_ar_sip_1)) && 0 != mmt_mem_cmp(arp_ar_sha , arp_ar_sha_1));
 }
 
 /** 412
  * States of FSM for rule 2
  */
 
 /** 413
  * Predefine list of states: init, fail, pass, ...
  */
 static fsm_state_t s_2_0, s_2_1, s_2_2, s_2_3, s_2_4;
 /** 426
  * Initialize states: init, error, final, ...
  */
 static fsm_state_t
 /** 432
  * initial state
  */
  s_2_0 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,//init or final states
	 .description  = "IPv4 address conflict detection (RFC5227). Possible arp poisoning.",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 1, //FSM_ACTION_CREATE_INSTANCE
	 .data         = NULL,
	 .transitions  = (fsm_transition_t[]){
		 /** 461 An arp reply with MAC address */
		 /** 463 A real event */
		 { .event_type = 1, .guard = &g_2_1, .action = 1, .target_state = &s_2_4}  //FSM_ACTION_CREATE_INSTANCE
	 },
	 .transitions_count = 1
 },
 /** 432
  * timeout/error state
  */
  s_2_1 = {
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
  s_2_2 = {
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
  s_2_3 = {
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
  s_2_4 = {
	 .delay        = {.time_min = 1LL, .time_max = 300000000LL, .counter_min = 0LL, .counter_max = 0LL},
	 .is_temporary = 0,
	 .description  = "IPv4 address conflict detection (RFC5227). Possible arp poisoning.",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .data         = NULL,
	 .transitions  = (fsm_transition_t[]){
		 /** 463 Timeout event will fire this transition */
		 { .event_type = 0, .guard = NULL  , .action = 0, .target_state = &s_2_1}, //FSM_ACTION_DO_NOTHING
		 /** 461 An arp reply but with different MAC address */
		 /** 463 A real event */
		 { .event_type = 2, .guard = &g_2_2, .action = 2, .target_state = &s_2_2}  //FSM_ACTION_RESET_TIMER
	 },
	 .transitions_count = 2
 };
 /** 490
  * Create a new FSM for this rule
  */
 static void *create_new_fsm_2(){
		 return fsm_init( &s_2_0, &s_2_1, &s_2_2, &s_2_3, EVENTS_COUNT_2, sizeof( _msg_t_2 ) );//init, error, final, inconclusive, events_count
 }//end function

 //======================================GENERAL======================================
 /** 618
  * Information of 2 rules
  * PUBLIC API
  */
 size_t mmt_sec_get_plugin_info( const rule_info_t **rules_arr ){
	  static const rule_info_t rules[] = (rule_info_t[]){
		 {
			 .id               = 1,
			 .type_id          = 0,
			 .type_string      = "attack",
			 .events_count     = EVENTS_COUNT_1,
			 .description      = "IPv4 address conflict detection (RFC5227). Possible arp poisoning.",
			 .if_satisfied     = NULL,
			 .if_not_satisfied = NULL,
			 .proto_atts_count = PROTO_ATTS_COUNT_1,
			 .proto_atts       = proto_atts_1,
			 .proto_atts_events= proto_atts_events_1,
			 .create_instance  = &create_new_fsm_1,
			 .convert_message  = &_allocate_msg_t_1,
			 .message_size     = sizeof( _msg_t_1 ),
			 .hash_message     = &hash_message_1,
			 .version          = {.created_date=1491404879, .hash = "f9f1d61", .number="1.0.2", .index=1000200, .dpi="1.6.7.0 (1039f64)"},
		 },
		 {
			 .id               = 2,
			 .type_id          = 0,
			 .type_string      = "attack",
			 .events_count     = EVENTS_COUNT_2,
			 .description      = "IPv4 address conflict detection (RFC5227). Possible arp poisoning.",
			 .if_satisfied     = NULL,
			 .if_not_satisfied = NULL,
			 .proto_atts_count = PROTO_ATTS_COUNT_2,
			 .proto_atts       = proto_atts_2,
			 .proto_atts_events= proto_atts_events_2,
			 .create_instance  = &create_new_fsm_2,
			 .convert_message  = &_allocate_msg_t_2,
			 .message_size     = sizeof( _msg_t_2 ),
			 .hash_message     = &hash_message_2,
			 .version          = {.created_date=1491404879, .hash = "f9f1d61", .number="1.0.2", .index=1000200, .dpi="1.6.7.0 (1039f64)"},
		 }
	 };
	 *rules_arr = rules;
	 return 2;
 }
 /** 658
  * Moment the rules being encoded
  * PUBLIC API
  */
 const char * __get_generated_date(){ return "2017-04-05 17:07:59, mmt-security version 1.0.2 (f9f1d61 - Apr  5 2017 16:44:04)";};