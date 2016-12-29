/*
 * lock_free_spsc_ring.c
 *
 *  Created on: 31 mars 2016
 *      Author: nhnghia
 *
 * An implementation of Lamport queue without lock
 * based on https://github.com/blytkerchan/Lamport
 *
 * Modified on 24 nov. 2016
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include "mmt_lib.h"
#include "lock_free_spsc_ring.h"

void ring_free( lock_free_spsc_ring_t *q ){
	if( q == NULL ) return;
	if( q->_data ) mmt_mem_free( q->_data );
	#ifdef SPIN_LOCK
		pthread_spin_destroy( &(q->spin_lock) );
	#endif

	mmt_mem_free( q );
}

lock_free_spsc_ring_t* ring_init( uint32_t size ){
	lock_free_spsc_ring_t *q = mmt_mem_alloc( sizeof( lock_free_spsc_ring_t ));
	q->_data = mmt_mem_alloc( sizeof( void *) * size );
	q->_size = size;
	q->_head = q->_tail = 0;
	q->_cached_head = q->_cached_tail = 0;

	return q;
}
