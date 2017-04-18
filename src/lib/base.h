/*
 * base.h
 *
 *  Created on: 21 sept. 2016
 *  Created by: Huu Nghia NGUYEN <huunghia.nguyen@montimage.com>
 *
 *  Basic definitions/data struct
 */

#ifndef SRC_LIB_BASE_H_
#define SRC_LIB_BASE_H_

#include <stdlib.h>
#include <stdint.h>
//for uint64_t PRIu64
#include <inttypes.h>
#include <stdbool.h>

#define UNKNOWN -1
#define NO false
#define YES true

//thread local storage
#define __thread_scope __thread

//macroe
#define __check_null( x, y ) if( unlikely( x == NULL )) return y; else

//branch prediction
#ifndef likely
	#define likely(x)   __builtin_expect(!!(x),1)
#endif
#ifndef unlikely
	#define unlikely(x) __builtin_expect(!!(x),0)
#endif


/* a=target variable, i=bit number to act upon 0-n  (n == sizeof(a))*/
//set bit i-th to 1
#define BIT_SET(number,i)   ((number) |= (1ULL<<(i)))
//set bit i-th to 0
#define BIT_CLEAR(number,i) ((number) &= ~(1ULL<<(i)))
//flip bit i-th
#define BIT_FLIP(number,i)  ((number) ^= (1ULL<<(i)))
//check bit i-th
#define BIT_CHECK(number,i) ((number) &  (1ULL<<(i)))

#endif /* SRC_LIB_BASE_H_ */
