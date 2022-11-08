/*
 * task.h
 *
 *  Created on: Sep 12, 2022
 *      Author: maibui
 */

#ifndef TASK_H_
#define TASK_H_

#include <stdio.h>

//#include "mod_fatfs_chan.h"
#include "mod_utilities.h"
#include "sbe.h"

#define TASK_1 1
#define ACQ_TASK 1
#define LOGGING_TASK 1
#define STREAMING_TASK 1

#define INIT_RATE 2 // 10Hz
#define IDLE_RATE 5 // 10Hz
#define ACQ_RATE 10 // 10Hz
#define STREAMING_RATE 10 // 10Hz
#define LOGGING_RATE 10 // 10Hz

#define  APP_EXAMPLE_TASK_PRIO            21u  /*   Task Priority.                 */
#define  APP_EXAMPLE_TASK_STK_SIZE       256u  /*   Stack size in CPU_STK.         */

#define  ACQ_TASK_PRIO                    10u  /*   Task Priority.                 */
#define  ACQ_TASK_STK_SIZE              256u  /*   Stack size in CPU_STK.         */
#define  LOGGING_TASK_PRIO                23u  /*   Task Priority.                 */
#define  LOGGING_TASK_STK_SIZE           256u  /*   Stack size in CPU_STK.         */
#define  STREAMING_TASK_PRIO              21u  /*   Task Priority.                 */
#define  STREAMING_TASK_STK_SIZE         256u  /*   Stack size in CPU_STK.         */
// maibui 4 Oct 2022
#define  SBE_TASK_PRIO              21u  /*   Task Priority.                 */
#define  SBE_TASK_STK_SIZE         256u  /*   Stack size in CPU_STK.         */

#define  ACQ_TASK_TICKS                    2  /*   Task tick.                 */
#define  LOGGING_TASK_TICKS                4  /*   Task tick.                 */
#define  STREAMING_TASK_TICKS              3  /*   Task tic.                 */
#define  INIT_TICKS                        1  /*   Task Priority.                 */
#define  LOGGING_TICKS                     3  /*   Task Priority.                 */
#define  STREAMING_TICKS                   5  /*   Task Priority.                 */

enum task_state {init=0, idle, acq, logging};
enum streaming_task_state {streaming_init=0, streaming_idle, streaming};
enum sbe_task_state {sbe_init=0, sbe_idle, sbe_logging, sbe_acq};
//enum storing_state {idle, storing};

#define SBE_DATA_LENGTH 22
#define LW_CIR_BUFF_SIZE 10

// structure for task - maibui 15 Sept 2022
typedef struct main_acq_struct
{
    bool acp_task_done;

    enum task_state acq_curr_state ;
}main_acq_struct, *main_acq_struct_ptr;


// structure for task - maibui 15 Sept 2022
typedef struct main_logging_struct
{
    bool logging_task_done;

    enum task_state logging_curr_state ;
    char data_file_name[25];
    FIL fh;
}main_logging_struct, *main_logging_struct_ptr;

//enum streaming_state {idle, acq};

// structure for task - maibui 15 Sept 2022
typedef struct main_streaming_struct
{
    bool streaming_task_done;

    enum streaming_task_state streaming_curr_state ;
}main_streaming_struct, *main_streaming_struct_ptr;

// move sbe49 variables to own file***************************
// sbe49 circular buffer/data struct - ebohannon 8 Oct 2022
typedef struct {
  uint64_t time_stamp_ms;
  char sbe_data[24]; //speed and directions
} sbe_data_t;

enum sbe_acq_state {find_sync,acq_string,checking,sending};
typedef struct
{
    int read_req_bytes;
    enum sbe_acq_state acq_state;
}sbe_runtime_t;

typedef struct main_sbe_struct
{
    bool sbe_task_done;

    enum sbe_task_state sbe_curr_state ;
    int read_req_bytes;
    sbe_data_t sbe_struct;
    sbe_runtime_t sbe_acq_data;
}main_sbe_struct, *main_sbe_struct_ptr;

// for saving in the sd card
typedef struct {
  int timestamp; //time in milliseconds from timer initialization
  char ctd_data[SBE_DATA_LENGTH]; //to store CTD data, initially from Seabird 49
  float rpm; //speed and direction from hall effect sensors
  volatile uint32_t adc_data; //analog data - current/voltage of Livewire
} livewire_sample_t, *livewire_sample_t_ptr;

// for streaming out
typedef struct {
  int timestamp; //time in milliseconds from timer initialization
  uint32_t temp;
  uint32_t con;
  uint32_t press;
  uint32_t tpc;
  float rpm; //speed and direction from hall effect sensors
  volatile uint32_t adc_data; //analog data - current/voltage of Livewire
} livewire_sample_decode_t, *livewire_sample_decode_t_ptr;


typedef struct
{
  livewire_sample_t livewire_cir_buff[LW_CIR_BUFF_SIZE];
  livewire_sample_decode_t livewire_decode_cir_buff[LW_CIR_BUFF_SIZE];
  uint64_t lw_cir_buf_write_cnt;
  uint64_t lw_cir_buf_write_indx;
  uint64_t lw_cir_buf_stream_read_cnt;
  uint64_t lw_cir_buf_stream_read_indx;
  uint64_t lw_cir_buf_log_read_cnt;
  uint64_t lw_cir_buf_log_read_indx;
}main_lw_struct, *main_lw_struct_ptr;


extern sbe_data_t sbe_history_ptr[];
extern volatile uint32_t sbe_history_write_index;
extern volatile uint32_t sbe_history_write_count;
extern volatile uint32_t sbe_history_read_index;
extern volatile uint32_t sbe_history_read_count;
#define SBE_BUFFER_SIZE 24
#define SBE_CIR_BUFF_SIZE 10
#define SBE_ENTRY_MAX_SIZE 125
//extern main_lw_struct lw_data;
//extern char sbe_str[125];

//*************************************************************

bool tasks_init();
bool acq_task_init(main_acq_struct* acq_struct_ptr);

/* Example Task Data:      */
void  task_1 (void  *p_arg);
void  task_1_create (void);
//void  acq_task_create (void);
void  acq_task_create (main_acq_struct *);
void  logging_task_create (main_logging_struct *);
void  streaming_task_create (main_streaming_struct *);
void  sbe_task_create (main_sbe_struct *); //maibui - 4 Oct 2022

void  task_1 (void  *p_arg);
void  acq_task (void  *p_arg);
void  logging_task (void  *p_arg);
void  streaming_task (void  *p_arg);
void  sbe_task (void  *p_arg); //maibui - 4 Oct 2022
int   sbe_acq_f(void  *p_arg);



void initADC (void);
void sbe_hex_to_binary_f(char *raw_sbe_data, int indx, main_lw_struct* main_lw_frame);  //ebohannon - 14 Oct 2022
#endif /* TASK_H_ */
