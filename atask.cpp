#include "Arduino.h"
#include "atask.h"

typedef struct 
{
    uint8_t  run_indx;
	uint8_t  head_indx;
} atask_ctrl_st;

atask_st *task[ATASK_RESERVED_NBR]; 
atask_ctrl_st atask_ctrl;

void dummy_task(void)
{

};

atask_st undef_task = {"Undefined      ", 0, 0, 0, 255, 0, 0, dummy_task };

void atask_initialize(void)
{
    for (uint8_t i = 0; i < ATASK_RESERVED_NBR; i++)
    {
        task[i] = &undef_task;
    }
    //atask_ctrl.active_tasks = active_tasks;
	atask_ctrl.head_indx = 0;
	atask_ctrl.run_indx = 0;
}

void atask_start(uint8_t tindx)
{
	task[atask_ctrl.run_indx]->run_flag = 0x01;
}
void atask_stop(uint8_t tindx)
{
	task[atask_ctrl.run_indx]->run_flag = 0x00;
}

uint8_t atask_add_new(atask_st *task_p) 
{
	uint8_t new_indx = ATASK_NOT_DEFINED;
	
	if (atask_ctrl.head_indx < ATASK_RESERVED_NBR)
	{	
        task[atask_ctrl.head_indx] = task_p;  
		new_indx = atask_ctrl.head_indx++;
	}
	return new_indx;
}


atask_st *atask_get_task(uint8_t task_indx) 
{
    if ( task_indx < atask_ctrl.head_indx )
        return task[task_indx];  
	else return &undef_task;
}

void atask_delay(uint8_t tindx, uint32_t delay_ms)
{
  task[tindx]->interval = millis() + delay_ms;
}

void atask_run(void)
{
	//Serial.print(atask_ctrl.run_indx);
    uint32_t now_ms = millis();
    if (atask_ctrl.run_indx < atask_ctrl.head_indx)
    {
		if(task[atask_ctrl.run_indx]->run_flag != 0x00)
		{
			//Serial.print("R");
			if (now_ms >= task[atask_ctrl.run_indx]->next_run )
			{
				//Serial.print(atask_ctrl.run_indx);
				task[atask_ctrl.run_indx]->next_run = now_ms + task[atask_ctrl.run_indx]->interval;
				(*task[atask_ctrl.run_indx]->cb)();
				task[atask_ctrl.run_indx]->cntr++;
			}
		}
		task[atask_ctrl.run_indx]->prev_state = task[atask_ctrl.run_indx]->state;
		atask_ctrl.run_indx++;

    } 
	else
	{
		atask_ctrl.run_indx = 0;
	}
}

void atask_print_status(bool force)
{   char buffer[64];
    // bool do_print = false;
    Serial.print("Now = ");
	Serial.println(millis());
    for (uint8_t i = 0; i < atask_ctrl.head_indx; i++)
    {
        if ((task[i]->prev_state != task[i]->state) || force)
        {
            sprintf(buffer,"%s: %d -> %d next %d %d  %d\n",
              task[i]->name, 
              task[i]->prev_state, 
              task[i]->state, 
              task[i]->next_run,
			  task[i]->cntr,
			  task[i]->run_flag);
			Serial.print(buffer);  
        }
    }
}

void atask_clear_cntr(uint8_t tindx)
{
	task[tindx]->cntr = 0;
}

uint16_t atask_get_cntr(uint8_t tindx)
{
	return task[tindx]->cntr;
}

