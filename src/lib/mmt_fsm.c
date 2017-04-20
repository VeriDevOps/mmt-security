/*
 * mmt_fsm.c
 *
 *  Created on: 3 oct. 2016
 *  Created by: Huu Nghia NGUYEN <huunghia.nguyen@montimage.com>
 */

#include "mmt_lib.h"
#include "plugin_header.h"
#include "mmt_fsm.h"
#include "message_t.h"
#include "rule_verif_engine.h"

/**
 * Detailed definition of FSM
 */
struct fsm_struct{
	uint64_t time_min, time_max;
//	uint64_t counter_min, counter_max;

	//id of the FSM
	uint16_t id;

	/** ID of event to be verified */
	uint16_t current_event_id;

   /**  Pointer to the current fsm_state_struct */
   const fsm_state_t *current_state;

   const fsm_state_t *init_state;

   const fsm_state_t *error_state;

   const fsm_state_t *incl_state;

   const fsm_state_t *success_state;

   mmt_array_t *execution_trace;

   //this is for internal usage. It points to _rule_engine_t
   void *user_data;
};


/**
 * Public API
 */
fsm_t *fsm_init(const fsm_state_t *initial_state, const fsm_state_t *error_state, const fsm_state_t *final, const fsm_state_t *incl_state, size_t events_count, size_t message_size ) {
	fsm_t *fsm = mmt_mem_alloc( sizeof( fsm_t ));

	fsm->init_state      = initial_state;
	fsm->current_state   = initial_state;
	fsm->error_state     = error_state;
	fsm->incl_state      = incl_state;
	fsm->success_state   = final;
	fsm->id              = 0;
	fsm->execution_trace = mmt_array_init( events_count + 1 ); //event_id starts from 1, zero is timeout
	fsm->time_max        = fsm->time_min    = 0;
//	fsm->counter_max     = fsm->counter_min = 0;
	fsm->current_event_id= 0;

	return fsm;
}

/**
 * Public API
 */
void fsm_reset( fsm_t *fsm ){
	size_t i;
	__check_null( fsm, );

	//reset the current state to the initial one
	fsm->current_state    = fsm->init_state;
	fsm->current_event_id = 0;

	for( i=0; i< fsm->execution_trace->elements_count; i++ ){
		free_message_t( fsm->execution_trace->data[ i ] );
		fsm->execution_trace->data[ i ] = NULL;
	}
}

static inline fsm_t* _fsm_clone( const fsm_t *fsm ){

	fsm_t *new_fsm = mmt_mem_force_dup( fsm, sizeof( fsm_t) );

	new_fsm->execution_trace  = mmt_array_clone( fsm->execution_trace, mmt_mem_atomic_retain );

	return new_fsm;
}

fsm_t *fsm_clone( const fsm_t *fsm ) {
	__check_null( fsm, NULL );
	return _fsm_clone( (fsm_t *) fsm);
}

static inline enum fsm_handle_event_value _fire_a_tran( fsm_t *fsm, uint16_t transition_index, message_t *message_data ) {
	fsm_t *new_fsm = NULL;
	enum fsm_handle_event_value ret;

	ret = fsm_handle_event( fsm, transition_index, message_data, &new_fsm );

	//Occasionally a new fsm may be created, we do not need it
	if( unlikely( new_fsm != NULL ) )
		fsm_free( new_fsm );

	return ret;
}


static inline enum fsm_handle_event_value _update_fsm( fsm_t *fsm, const fsm_state_t *new_state, const fsm_transition_t *tran, message_t *message ){
	void *ptr = NULL;
	uint64_t val;
	int i;
	const fsm_state_t *previous_state;
	enum fsm_handle_event_value ret;

#ifdef DEBUG_MODE
	if( unlikely( fsm->current_event_id == 0 )){
		mmt_halt( "Not possible");
	}
#endif

	//mmt_debug( "fsm_id = %d (%p), ref = %zu, event_id: %d", fsm->id, fsm, mmt_mem_reference_count( event_data), tran->event_type );

	//check if we will override an element of execution trace
	if( unlikely( fsm->execution_trace->data[ fsm->current_event_id   ] != NULL ) ){
		free_message_t( fsm->execution_trace->data[ fsm->current_event_id   ] );
	}

	//store execution log
	fsm->execution_trace->data[ fsm->current_event_id ] = mmt_mem_atomic_retain( message );

//	/* Run exit action
//	 * (even if it returns to itself) */
//		_exec_action( NO, event_data, fsm->current_state, fsm );
//

	// Update the states in FSM
	previous_state     = fsm->current_state;
	fsm->current_state = new_state;

	/* If the target state is a final one, notify user that the machine has stopped */
	if( fsm->current_state->transitions_count == 0 ){
		if (fsm->current_state == fsm->error_state){
			//mmt_debug("FSM_ERROR_STATE_REACHED" );
			return FSM_ERROR_STATE_REACHED;
		}else if (fsm->current_state == fsm->incl_state){
			//mmt_debug("FSM_INCONCLUSIVE_STATE_REACHED" );
			return FSM_INCONCLUSIVE_STATE_REACHED;
		}else if ( fsm->current_state == fsm->success_state ){
			//mmt_debug("FSM_FINAL_STATE_REACHED" );
			return FSM_FINAL_STATE_REACHED;
		}else
			return FSM_ERR_ARG;
	}

	// We reach a state that has delay = 0
	// => we need to continue verifying the next outgoing transitions against the current message
	if( unlikely( fsm->current_state->is_temporary )){
		//for each outgoing transition of the target
		//fire the timeout transition (at index 0) only if other transitions cannot be fired
		for( i=fsm->current_state->transitions_count - 1; i>= 0; i-- ){
			ret = _fire_a_tran( (fsm_t *) fsm, (uint16_t)i, message );

			if( ret != FSM_NO_STATE_CHANGE )
				return ret;
		}

//		mmt_debug( "LOOOP" );
		return FSM_NO_STATE_CHANGE;
	}

	//update deadline
	//outgoing from init state
	if( tran->action == FSM_ACTION_RESET_TIMER || previous_state == fsm->init_state ){
//		fsm->counter_min = new_state->delay.counter_min + message_data->counter;
		fsm->time_min    = new_state->delay.time_min    + message->timestamp;

//		fsm->counter_max = new_state->delay.counter_max + message_data->counter;
		fsm->time_max    = new_state->delay.time_max    + message->timestamp;
	}else{
//		val = new_state->delay.counter_min + message_data->counter;
//		if( val > fsm->counter_min ) fsm->counter_min = val;

		val = new_state->delay.time_min + message->timestamp;
		if( val > fsm->time_min ) fsm->time_min = val;

//		val = new_state->delay.counter_max + message_data->counter;
//		if( val < fsm->counter_max ) fsm->counter_max = val;

		val = new_state->delay.time_max + message->timestamp;
		if( val < fsm->time_max ) fsm->time_max = val;
	}

	/* Call the new _state's entry action if it has any
	 * (even if state returns to itself) */

	//mmt_debug("FSM_STATE_CHANGED" );
	return FSM_STATE_CHANGED;
}


/**
 * Public API
 */
enum fsm_handle_event_value fsm_handle_event( fsm_t *fsm, uint16_t transition_index, message_t *message, fsm_t **new_fsm ) {
	const fsm_transition_t *tran;
	fsm_t *_new_fsm;
	const fsm_state_t *state;
	//uint64_t timer, counter;

#ifdef DEBUG_MODE
	__check_null( fsm, FSM_ERR_ARG );
#endif

#ifdef DEBUG_MODE
	if ( unlikely( !fsm->current_state ))
		mmt_halt( "Not found current state of fsm %d", fsm->id );
	if( transition_index >= fsm->current_state->transitions_count )
		mmt_halt("Transition_index is greater than transitions_count (%d >= %zu)", transition_index,  fsm->current_state->transitions_count );
#endif

	//	mmt_debug( "Verify transition: %d of fsm %p", transition_index, fsm );

	//do not use the message/event if it comes early than time_min
	if( unlikely( message->timestamp < fsm->time_min ))
		return FSM_NO_STATE_CHANGE;

	//event_type = FSM_EVENT_TYPE_TIMEOUT when this function is called by #_fire_a_tran
	//e.g., when no real-transition can be fired
	//in such a case, event_id will be the last transition that can not be fired
	if( fsm->current_state->transitions[ transition_index ].event_type != FSM_EVENT_TYPE_TIMEOUT )
		fsm->current_event_id = fsm->current_state->transitions[ transition_index ].event_type;

	//check if timeout or not (even we are checking a real event)
	//check only for the state other than init_state
	if( !fsm->current_state->is_temporary
			&& fsm->current_state != fsm->init_state
			&& message->timestamp > fsm->time_max  ){
		tran = &fsm->current_state->transitions[ 0 ];//timeout transition must be the first in the array
		if( likely( tran->event_type == FSM_EVENT_TYPE_TIMEOUT ))
			//fire timeout transition
			return _update_fsm( fsm, tran->target_state, tran, message );
	}

	tran = &fsm->current_state->transitions[ transition_index ];// _get_transition(fsm, state, event);

	//if we intend to check TIMEOUT but transition is not timeout => stop checking
	if( unlikely( tran->event_type == FSM_EVENT_TYPE_TIMEOUT ))
		return FSM_NO_STATE_CHANGE;

	//must not be null
//	if( tran == NULL ) return FSM_NO_STATE_CHANGE;

	/* If transition is guarded, ensure that the condition is held: */
	if (tran->guard != NULL && tran->guard( message, fsm )  == NO )
		return FSM_NO_STATE_CHANGE;

	/* A transition must have a next _state defined
	 * If the user has not defined the next _state, go to error _state: */
	//mmt_assert( tran->target_state != NULL, "Error: Target state cannot be NULL" );

//	mmt_debug( "Exit action: %d", fsm->current_state->exit_action );
	//Create a new instance, then update its data
	if ( tran->action == FSM_ACTION_CREATE_INSTANCE && tran->target_state->transitions_count > 0 ){
		//mmt_debug( " new FSM");
		_new_fsm = _fsm_clone( fsm );
		*new_fsm = (fsm_t *)_new_fsm;
		return _update_fsm( _new_fsm, tran->target_state, tran, message );
	}

	//add event to execution trace
	return _update_fsm( fsm, tran->target_state, tran, message );
}

bool fsm_is_verifying_single_packet( const fsm_t *fsm ){

#ifdef DEBUG_MODE
	__check_null( fsm, FSM_ERR_ARG );
#endif
	const fsm_transition_t *tran;

	//has 3 events (one for timeout and 2 real events)
	if( fsm->execution_trace->elements_count != 3 )
		return false;

	//init_state has only one out-going transition
	if( fsm->init_state->transitions_count != 1 )
		 return false;

	tran = &fsm->init_state->transitions[0];

	//zero-delay for target state of init one
	if( ! tran->target_state->is_temporary )
		return false;

	//the state has 2 transitions: 0-timeout 1-real-transition
	if( tran->target_state->transitions_count != 2 )
		return false;

	tran = &tran->target_state->transitions[ 1 ];

	//target of real-transition is one of final-states
	if( tran->target_state != fsm->error_state && tran->target_state != fsm->success_state && tran->target_state != fsm->incl_state )
		return false;

	return true;
}

//special rule that verifies on one packet
enum fsm_handle_event_value fsm_handle_single_packet( fsm_t *fsm, message_t *message ){
	const fsm_transition_t *tran;
	fsm_t *_new_fsm;
	const fsm_state_t *state;
	//uint64_t timer, counter;

#ifdef DEBUG_MODE
	__check_null( fsm, FSM_ERR_ARG );
	if ( unlikely( !fsm->current_state ))
			mmt_halt( "Not found current state of fsm %d", fsm->id );
#endif

	//first event
	tran = &fsm->init_state->transitions[0];
	if( tran->guard && tran->guard( message, fsm)  == NO )
		return FSM_NO_STATE_CHANGE;

	//==> first event is satisfied

	//store execution log
	//increase its references to 2 as another is used for
	//  the second event fsm->execution_trace->data[ 2 ]
	fsm->execution_trace->data[ 1 ] = mmt_mem_atomic_retains( message, 2 );

	/**
	 * We check now the second event
	 * first transition is timeout
	 * second transition is real one
	 */
	tran = &fsm->init_state->transitions[0].target_state->transitions[1];

	//the second event is not satisfied => use timeout
	if( tran->guard && tran->guard( message, fsm)  == NO )
		state = fsm->init_state->transitions[0].target_state->transitions[0].target_state;
   else
      state = tran->target_state;

	fsm->execution_trace->data[ 2 ] = message;

	if (state == fsm->error_state){
		//mmt_debug("FSM_ERROR_STATE_REACHED" );
		return FSM_ERROR_STATE_REACHED;
	}else if (state == fsm->incl_state){
		//mmt_debug("FSM_INCONCLUSIVE_STATE_REACHED" );
		return FSM_INCONCLUSIVE_STATE_REACHED;
	}else if (state == fsm->success_state ){
		//mmt_debug("FSM_FINAL_STATE_REACHED" );
		return FSM_FINAL_STATE_REACHED;
	}else
		return FSM_ERR_ARG;
}

/**
 * Public API
 */
const fsm_state_t *fsm_get_current_state( const fsm_t *fsm) {
	__check_null( fsm, NULL );

	return fsm->current_state;
}


/**
 * Public API
 */
bool fsm_is_stopped( const fsm_t *fsm) {
	__check_null( fsm, YES );
	return (fsm->current_state->transitions_count == 0);
}

/**
 * Public API
 */
void fsm_free( fsm_t *fsm ){
	__check_null( fsm, );

	mmt_array_free( fsm->execution_trace, (void *)free_message_t );
	mmt_mem_force_free( fsm );
}


/**
 * Public API
 */
const mmt_array_t* fsm_get_execution_trace( const fsm_t *fsm ){
#ifdef DEBUG_MODE
	__check_null( fsm, NULL );
#endif
	return( fsm->execution_trace );
}


/**
 * Public API
 */
const message_t *fsm_get_history( const fsm_t *fsm, uint32_t event_id ){
#ifdef DEBUG_MODE
	__check_null( fsm, NULL );
#endif
	if( unlikely( event_id >= fsm->execution_trace->elements_count )){
		mmt_halt("Access outside of array");
	}

	return fsm->execution_trace->data[ event_id ];
}

/**
 * Public API
 */
uint16_t fsm_get_id( const fsm_t *fsm ){
#ifdef DEBUG_MODE
	__check_null( fsm, -1 );
#endif
	return fsm->id;
}

/**
 * Public API
 */
void fsm_set_id( fsm_t *fsm, uint16_t id ){
#ifdef DEBUG_MODE
	__check_null( fsm,  );
#endif
	fsm->id = id;
}



/**
 * Public API
 */
void * fsm_get_user_data( const fsm_t *fsm ){
#ifdef DEBUG_MODE
	__check_null( fsm, NULL );
#endif
	return fsm->user_data;
}

/**
 * Public API
 */
void fsm_set_user_data( fsm_t *fsm, void *data){
#ifdef DEBUG_MODE
	__check_null( fsm,  );
#endif
	fsm->user_data = data;
}
