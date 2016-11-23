/*
 * mmt_sec_config.h
 *
 *  Created on: 23 nov. 2016
 *      Author: la_vinh
 */

#ifndef MMT_SEC_CONFIG_H_
#define MMT_SEC_CONFIG_H_
#include <stdio.h>
#include <inttypes.h>

typedef struct mmt_sec_config_struct {
	uint8_t nb_thr_sec; //number of mmt_sec_handler threads
	uint32_t portno; //port number of the server
	uint32_t threshold_size; //THRESHOLD_SIZE
	uint8_t threshold_time; //THRESHOLD_TS
}mmt_sec_config_struct_t;

mmt_sec_config_struct_t get_mmt_sec_config(char *filename);

#endif /* MMT_SEC_CONFIG_H_ */
