#ifndef __ATASK_H__
#define __ATASK_H__

#define ATASK_RESERVED_NBR  	16
#define ATASK_NOT_DEFINED	255

#include "Arduino.h"

typedef void (*task_cb)(void);

typedef struct 
{
    char      name[16];
    uint32_t  interval;
    uint32_t  next_run;
    uint16_t  state;
    uint16_t  prev_state;
	uint16_t  cntr;
	uint16_t  run_flag;
    task_cb   cb;
} atask_st;


void atask_initialize(void);

void atask_set_task(uint8_t task_indx, atask_st *task_p);

uint8_t atask_add_new(atask_st *task_p);

atask_st *atask_get_task(uint8_t task_indx);

void atask_delay(uint8_t tindx, uint32_t delay_ms);

void atask_run(void);

void atask_print_status(bool force);

void atask_clear_cntr(uint8_t tindx);

uint16_t atask_get_cntr(uint8_t tindx);

#endif