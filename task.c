/*
 * task.c
 *
 *  Created on: Sep 12, 2022
 *      Author: maibui
 */

#include <string.h>
#include <stdio.h>
#include "cli.h"
#include "sl_cli.h"
#include "sl_cli_instances.h"
#include "sl_cli_arguments.h"
#include "sl_cli_handles.h"
#include "sl_simple_led.h"
#include "sl_simple_led_instances.h"
#include "sl_iostream.h"
#include "sl_iostream_init_instances.h"
#include "sl_iostream_handles.h"
#include "os.h"
#include "ff.h"
#include "sdio.h"
#include "diskio.h"

#include "hall_effects.h"
#include "math.h"

#include "task.h"
//#include "sbe.h"
// maibui - 12 Oct 2022: adding adc
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_adc.h"

// for adc - maibui - 12 Oct 2022
#define adcFreq   16000000
volatile uint32_t adc_sample;
volatile uint32_t millivolts;
static char sbe_str[125] = "\0";
static main_lw_struct lw_data;


/* Example Task Defines:            */

                                                        /* Example Task Data:      */
OS_TCB   App_ExampleTaskTCB;                            /*   Task Control Block.   */
CPU_STK  App_ExampleTaskStk[APP_EXAMPLE_TASK_STK_SIZE]; /*   Stack.                */

OS_TCB   App_ExampleTaskTCB;                            /*   Task Control Block.   */
CPU_STK  App_ExampleTaskStk[APP_EXAMPLE_TASK_STK_SIZE]; /*   Stack.                */

OS_TCB   Acq_TaskTCB;                            /*   Task Control Block.   */
CPU_STK  Acq_TaskStk[APP_EXAMPLE_TASK_STK_SIZE]; /*   Stack.                */

OS_TCB   Logging_TaskTCB;                            /*   Task Control Block.   */
CPU_STK  Logging_TaskStk[APP_EXAMPLE_TASK_STK_SIZE]; /*   Stack.                */

OS_TCB   Streaming_TaskTCB;                            /*   Task Control Block.   */
CPU_STK  Streaming_TaskStk[APP_EXAMPLE_TASK_STK_SIZE]; /*   Stack.                */

OS_TCB   Sbe_TaskTCB;                            /*   Task Control Block.   */
CPU_STK  Sbe_TaskStk[SBE_TASK_STK_SIZE]; /*   Stack.                */


                                                      /* Example Task Code:      */
//void  task_1 (void  *p_arg);
void  task_1_create (void)
{
    RTOS_ERR     err;

    OSTaskCreate(&App_ExampleTaskTCB,                /* Pointer to the task's TCB.  */
                 "Example Task.",                    /* Name to help debugging.     */
                 &task_1,                   /* Pointer to the task's code. */
                  DEF_NULL,                          /* Pointer to task's argument. */
                  APP_EXAMPLE_TASK_PRIO,             /* Task's priority.            */
                 &App_ExampleTaskStk[0],             /* Pointer to base of stack.   */
                 (APP_EXAMPLE_TASK_STK_SIZE / 10u),  /* Stack limit, from base.     */
                  APP_EXAMPLE_TASK_STK_SIZE,         /* Stack size, in CPU_STK.     */
                  10u,                               /* Messages in task queue.     */
                  0u,                                /* Round-Robin time quanta.    */
                  DEF_NULL,                          /* External TCB data.          */
                  OS_OPT_TASK_STK_CHK,               /* Task options.               */
                 &err);
    if (err.Code != RTOS_ERR_NONE) {
        /* Handle error on task creation. */
    }
}

void  task_1 (void  *p_arg)
{
  static int i=0;
  OS_RATE_HZ os_tick_rate;
  RTOS_ERR p_err;

  int run_count = 0;

    /* Use argument. */
   (void)&p_arg;
   os_tick_rate = OSTimeTickRateHzGet (&p_err);


    while (DEF_TRUE) {
        printf("example task activation count %d \r\n",i++);
        OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
        /* All tasks should be written as infinite loops. */
        run_count++;
        if (run_count == 10) break;
    }
}


void initADC (void)
{
  // Enable ADC0 clock
  CMU_ClockEnable(cmuClock_ADC0, true);

  // Declare init structs
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

  // Modify init structs and initialize
  init.prescale = ADC_PrescaleCalc(adcFreq, 0); // Init to max ADC clock for Series 1

  initSingle.diff       = false;        // single ended
  initSingle.reference  = adcRef2V5;    // internal 2.5V reference
  initSingle.resolution = adcRes12Bit;  // 12-bit resolution
  initSingle.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirement

  // Select ADC input. See README for corresponding EXP header pin.
//  initSingle.posSel = adcPosSelAPORT4XCH11;
  initSingle.posSel = adcPosSelAPORT4XCH13; // change to PE13 - 14Oct2022
  init.timebase = ADC_TimebaseCalc(0);

  ADC_Init(ADC0, &init);
  ADC_InitSingle(ADC0, &initSingle);
}


//typedef enum {idle = 1,acq}acq_state;
//typedef enum {reading = 1,storing}storing_state;
//typedef enum {reading = 1,printout}streaming_state;


bool acq_task_init(main_acq_struct* acq_struct_ptr)
{
  bool status = 0;

  acq_struct_ptr->acp_task_done = 0;

  return status;
}
//void  acq_task_create (void)

void  acq_task_create (main_acq_struct* acq_struct_ptr)
{
    RTOS_ERR     err;

    OSTaskCreate(&Acq_TaskTCB,                // Pointer to the task's TCB.  */
                 "Acquistion Task.",                    /* Name to help debugging.     */
                 &acq_task,                   /* Pointer to the task's code. */
 //                DEF_NULL,                          /* Pointer to task's argument. */
                  acq_struct_ptr,                          /* Pointer to task's argument. */
                  ACQ_TASK_PRIO,             /* Task's priority.            */
                 &Acq_TaskStk[0],             /* Pointer to base of stack.   */
                 (ACQ_TASK_STK_SIZE / 10u),  /* Stack limit, from base.     */
                  ACQ_TASK_STK_SIZE,         /* Stack size, in CPU_STK.     */
                  10u,                               /* Messages in task queue.     */
                  0u,                                /* Round-Robin time quanta.    */
                  DEF_NULL,                          /* External TCB data.          */
                  OS_OPT_TASK_STK_CHK,               /* Task options.               */
                 &err);
    if (err.Code != RTOS_ERR_NONE) {
        printf("Failed to create acq_task\r\n");
        /* Handle error on task creation. */
    }
}


void  acq_task (void  *p_arg)
{
  static int i=0;
  OS_RATE_HZ os_tick_rate;
  RTOS_ERR p_err;
  main_acq_struct* acq_ptr;

  static int acq_run_count = 0;

  /* Use argument. */
  acq_ptr = p_arg;
  acq_ptr->acq_curr_state = 1;

   os_tick_rate = OSTimeTickRateHzGet (&p_err);

//   while(!acq_ptr->acp_task_done)
   while(DEF_TRUE)
   {
       // begining run: turn on "led"
//       printf("ount %d \r\n",run_count);
 //     OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
       /* All tasks should be written as infinite loops. */
//       run_count++;
//      if (run_count == 20) break;
   switch(acq_ptr->acq_curr_state)
     {
       case init:
         // do all initial need to get done before acquire data
         // example: reset run counter ...
 //          printf("acq_task is in \"init\" state, count %d\r\n",acq_run_count);
 //          OSTimeDly (ACQ_TASK_PRIO_TICKS, OS_OPT_TIME_PERIODIC,&p_err);
           OSTimeDly (INIT_TICKS, OS_OPT_TIME_PERIODIC,&p_err);
           // All tasks should be written as infinite loops.
           acq_run_count++;
           if (acq_run_count >= 5)
             {
               acq_ptr->acq_curr_state = idle;
               acq_run_count = 0;
               break;
             }
         break;
       case idle:
//         printf("acq_task is in \"idle\" state, with run_count %d\r\n",acq_run_count);
         // waiting for data is ready ... (until the data_ready_flag is on) ==> change to "acq" state
         OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
 //        OSTimeDly (IDLE_TICKS, OS_OPT_TIME_PERIODIC,&p_err);
         // All tasks should be written as infinite loops.
         acq_run_count++;
         if (acq_run_count >= 5)
           {
             acq_ptr->acq_curr_state = acq;
             acq_run_count = 0;
             break;
           }
        break;
       case acq:
 //        printf("acq_task is in \"acquisition\" state, count %d\r\n",acq_run_count);
         OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
 //        OSTimeDly (ACQ_TICKS, OS_OPT_TIME_PERIODIC,&p_err);
         // All tasks should be written as infinite loops.
         // acquire ctd data here ...
         //acquire_ctd_data();



         acq_run_count++;
         if (acq_run_count >= 5)
           {
             acq_ptr->acq_curr_state = init;
             acq_run_count = 0;
             break;
           }
         break;
       default:
         break;
     }
   // the end of one round: turn off "led"  => measure how long it takes for one round

   i++;
   if(i == 30) break;
   }
   printf("Inside acquisition task\r\n");
}
void  logging_task_create (main_logging_struct* logging_struct_ptr)
{
    RTOS_ERR     err;

    OSTaskCreate(&Logging_TaskTCB,                /* Pointer to the task's TCB.  */
                 "Logging Task.",                    /* Name to help debugging.     */
                 &logging_task,                   /* Pointer to the task's code. */
                 logging_struct_ptr,                          /* Pointer to task's argument. */
                  LOGGING_TASK_PRIO,             /* Task's priority.            */
                 &Logging_TaskStk[0],             /* Pointer to base of stack.   */
                 (LOGGING_TASK_STK_SIZE / 10u),  /* Stack limit, from base.     */
                 LOGGING_TASK_STK_SIZE,         /* Stack size, in CPU_STK.     */
                  10u,                               /* Messages in task queue.     */
                  0u,                                /* Round-Robin time quanta.    */
                  DEF_NULL,                          /* External TCB data.          */
                  OS_OPT_TASK_STK_CHK,               /* Task options.               */
                 &err);
    if (err.Code != RTOS_ERR_NONE) {
        /* Handle error on task creation. */
    }
}

void  logging_task (void  *p_arg)
{
  static int i=0;
  OS_RATE_HZ os_tick_rate;
  RTOS_ERR p_err;
//
//  char data_str[48] = "$LWG_123456789\r\n";
//  char temp_str[48] = "$LWG_123456789sbedata_halleffectdata\r\n";

  int retval = 0;
  int val = 0;
  FIL fh;
  char file_name[] = "sbe_sim_sample.txt";
//  char sbe_str[125];

  int num_write;
  unsigned int nwritten = 0;
  uint64_t write_check = 0;
  uint64_t prev_write_cnt = 0;
  int log_state = 0;
  int len = 0;
  static int logging_run_count = 0;
  os_tick_rate = OSTimeTickRateHzGet (&p_err);
//
  main_logging_struct* logging_ptr;

    /* Use argument. */
//
   logging_ptr = p_arg;
   logging_ptr->logging_curr_state = 1;
   log_state = 1;
   strcpy(logging_ptr->data_file_name,"test_file3"); // for now.
//
   retval = f_open(&fh, logging_ptr->data_file_name, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
   if (retval != FR_OK)
   {
       printf("Failed to open %s, error %u\n", logging_ptr->data_file_name, retval);
       return;
   }
   while(DEF_TRUE)
//   while(!storing_ptr->storing_task_done)
   {
//       switch(logging_ptr->logging_curr_state)
       switch(log_state)
     {
//       case init:
       case 1:
//           printf("logging_task is in \"init\" state, count %d\r\n",logging_run_count);
//           OSTimeDly (os_tick_rate/INIT_RATE, OS_OPT_TIME_PERIODIC,&p_err);
           OSTimeDly (os_tick_rate/INIT_RATE, OS_OPT_TIME_PERIODIC,&p_err);
           // All tasks should be written as infinite loops.
           logging_run_count++;
           if (logging_run_count >= 5)
             {
               logging_ptr->logging_curr_state = idle;
               logging_run_count = 0;
               log_state = 2;
               break;
             }
         break;
//       case idle:
       case 2:
//         printf("logging_task is in \"idle\" state, with run_count %d\r\n",logging_run_count);
         OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
         // All tasks should be written as infinite loops.
         logging_run_count++;
         if (logging_run_count >= 5)
           {
             logging_ptr->logging_curr_state = logging;
             logging_run_count = 0;
             log_state = 3;
             break;
           }
        break;
//       case logging:
       case 3:
 //        printf("logging_task is in \"logging\" state, count %d\r\n",logging_run_count);
         OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
         // All tasks should be written as infinite loops.
         // save data into the file
//         f_lseek(&fh,SEEK_END);
//         sprintf(data_str,"%s_%d",temp_str,logging_run_count);
//         num_write = strlen(data_str);
//         retval = f_write(&fh, data_str,num_write,&nwritten);
//  //       val = fs_write_tst_f(logging_ptr->data_file_name,data_str);
// //        val = mod_write_sd_f(&logging_ptr->fh, logging_ptr->data_file_name, data_str);
//         if (val!=FR_OK)
//           {
//             printf("Failed to write data into the sd card\n");
//           }
         //vvv ebohannon OCt 20 2022 vvv
         //data file should still be open, so want to check lw_data for new entries to log
         //let's first try and grab one entry from the buffer to save
         write_check = lw_data.lw_cir_buf_write_cnt;
         if (write_check > prev_write_cnt){
             printf("cool\n"); //this print works
//             len = strlen(sbe_str);
//             printf("%s\n",sbe_str);
//             sl_iostream_write(sl_iostream_vcom_handle, sbe_str, len);

//             printf("send out: %s\n",sbe_str);
//             len = strlen(sbe_str);
//
//             retval = f_open(&fh, file_name, FA_WRITE|FA_OPEN_APPEND);
//             if (retval == FR_OK)
//                   {
//             //          retval = f_write(&fh, "0123456789abcdef",10,&nwritten);
//                       retval = f_write(&fh, data_entry, num_write, &nwritten);}
//             f_close(&fh);
         }
         prev_write_cnt = lw_data.lw_cir_buf_write_cnt;

         logging_run_count++;
         if (logging_run_count >= 5)
           {
             logging_ptr->logging_curr_state = init;
             logging_run_count = 0;
             log_state = 1;
//             f_close(&fh);
             break;
           }
         break;
       default:
         break;
     }
        i++;
/*   if(i == 30)
     {        f_close(&logging_ptr->fh);
               break;
     }
*/
   }

   printf("Inside logging task\r\n");
}

void  streaming_task_create (main_streaming_struct* streaming_struct_ptr)
{
    RTOS_ERR     err;

    OSTaskCreate(&Streaming_TaskTCB,                /* Pointer to the task's TCB.  */
                 "Streaming Task.",                    /* Name to help debugging.     */
                 &streaming_task,                   /* Pointer to the task's code. */
                 streaming_struct_ptr,                          /* Pointer to task's argument. */
                  STREAMING_TASK_PRIO,             /* Task's priority.            */
                 &Streaming_TaskStk[0],             /* Pointer to base of stack.   */
                 (STREAMING_TASK_STK_SIZE / 10u),  /* Stack limit, from base.     */
                  STREAMING_TASK_STK_SIZE,         /* Stack size, in CPU_STK.     */
                  10u,                               /* Messages in task queue.     */
                  0u,                                /* Round-Robin time quanta.    */
                  DEF_NULL,                          /* External TCB data.          */
                  OS_OPT_TASK_STK_CHK,               /* Task options.               */
                 &err);
    if (err.Code != RTOS_ERR_NONE) {
        /* Handle error on task creation. */
    }
}
void  streaming_task (void  *p_arg)
{
  static int i=0;
  OS_RATE_HZ os_tick_rate;
  RTOS_ERR p_err;

  static int streaming_run_count = 0;

  os_tick_rate = OSTimeTickRateHzGet (&p_err);

   main_streaming_struct* streaming_ptr;

     /* Use argument. */
    streaming_ptr = p_arg;
    streaming_ptr->streaming_curr_state = 1;
  /*   while (DEF_TRUE) {
       printf("******* Streaming task activation count %d \r\n",i++);
        OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
        // All tasks should be written as infinite loops.
        streaming_run_count++;
        if (streaming_run_count == 10) break;
    }*/


   while(DEF_TRUE)
//    while(!streaming_ptr->streaming_task_done)
    {
        switch(streaming_ptr->streaming_curr_state)
      {
        case streaming_init:
//            printf("streaming_task is in \"init\" state, count %d\r\n",streaming_run_count);
            OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
            // All tasks should be written as infinite loops.
            streaming_run_count++;
            if (streaming_run_count >= 5)
              {
                streaming_ptr->streaming_curr_state = streaming_idle;
                streaming_run_count = 0;
                break;
              }
          break;
        case streaming_idle:
 //         printf("streaming_task is in \"idle\" state, with run_count %d\r\n",streaming_run_count);
          OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
          // All tasks should be written as infinite loops.
          streaming_run_count++;
          if (streaming_run_count >= 5)
            {
              streaming_ptr->streaming_curr_state = streaming;
              streaming_run_count = 0;
              break;
            }
         break;
        case streaming:
//          printf("streaming_task is in \"streaming\" state, count %d\r\n",streaming_run_count);
          OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
          // All tasks should be written as infinite loops.
          streaming_run_count++;
          if (streaming_run_count >= 5)
            {
              streaming_ptr->streaming_curr_state = streaming_init;
              streaming_run_count = 0;
              break;
            }
          break;
        default:
          break;
      }
        i++;
    if(i == 30) break;

    }

   printf("Inside streaming task\r\n");
}

//maibui - 4 Oct 2022
void  sbe_task_create (main_sbe_struct* sbe_struct_ptr)
{
    RTOS_ERR     err;

    OSTaskCreate(&App_ExampleTaskTCB,                // Pointer to the task's TCB.
                 "SBE Task.",                    // Name to help debugging.
                 &sbe_acq_f,                   // Pointer to the task's code.
//                 &sbe_task,                   // Pointer to the task's code.
                  DEF_NULL,                          // Pointer to task's argument.
                  SBE_TASK_PRIO,             // Task's priority.
                 &Sbe_TaskStk[0],             // Pointer to base of stack.
                 (SBE_TASK_STK_SIZE / 10u),  // Stack limit, from base.
                  SBE_TASK_STK_SIZE,         // Stack size, in CPU_STK.
                  10u,                               // Messages in task queue.
                  0u,                                // Round-Robin time quanta.
                  DEF_NULL,                          //External TCB data.
                  OS_OPT_TASK_STK_CHK,               // Task options.
                 &err);
    if (err.Code != RTOS_ERR_NONE) {
        // Handle error on task creation.
    }
}

/*
 * Function Name: void sbe_task (void *p_arg)
 * Description: task to log
 * Input: *p_arg takes any pointer
 * Output:
 * Author:
 * Date:
 */
void  sbe_task (void  *p_arg)
{
  static int i=0;
  OS_RATE_HZ os_tick_rate;
  RTOS_ERR p_err;

  FRESULT retval = 0;
  main_sbe_struct* sbe_ptr;

  char sbe_str[] = "123456789873t494";

    //Use argument.
  // (void)&p_arg;
   sbe_ptr = p_arg;
   sbe_ptr->sbe_curr_state = 3; // sbe_logging

 //  os_tick_rate = OSTimeTickRateHzGet (&p_err);
   FIL fh;
   char file_name[] = "sbe_sim_sample.txt";
   char data_str[48] = "\0";

   int num_write;
   unsigned int nwritten = 0;

   static int sbe_run_count = 0;

   sl_status_t status;
   uint64_t sbe_time_ms;
   char sbe_input[SBE_BUFFER_SIZE];
   size_t buffer_length = SBE_BUFFER_SIZE;
   size_t bytes_read = SBE_BUFFER_SIZE;
   sbe_data_t sbe_history_ptr[SBE_CIR_BUFF_SIZE];
   volatile uint32_t sbe_history_write_index;
   volatile uint32_t sbe_history_write_count;

   volatile uint32_t sbe_history_read_index;
   volatile uint32_t sbe_history_read_count;

   int sbe_char_index = 0;

   sbe_history_write_index = 0;
   sbe_history_write_count = 0;

   sbe_history_read_index = 0;
   sbe_history_read_count = 0;

   /*
   //create test file to log sbe49 data
   f_mount();
   retval = f_open(&fh, file_name, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
   if (retval != FR_OK)
   {
       printf("Failed to open %s, error %u\n", file_name, retval);
       return;
   }
   //close file so it can be opened in append mode in the loop
   f_close(&fh);
*/
   while(DEF_TRUE)
     {
//       printf("sbe: %s\n",sbe_str);
       /*      f_lseek(&fh,SEEK_END);
        sprintf(data_str,"%s_%d",sbe_str,i);
        num_write = strlen(data_str);
        retval = f_write(&fh, data_str,num_write,&nwritten);
*/

       status = sl_iostream_read(sl_iostream_sbe49_handle,&sbe_input,buffer_length,&bytes_read);
       if(status != SL_STATUS_OK)
       {
                return;
       }
printf("Sbe data: %s\n",sbe_input);
#ifdef DB
         switch(sbe_ptr->sbe_curr_state)
       {
           case sbe_logging:
                 // save sbe data into the file in sd card
               printf("sbe: %s\n",sbe_str);
               retval = f_open(&fh, file_name, FA_WRITE|FA_OPEN_APPEND);
               if (retval == FR_OK){
                   while (sbe_history_read_count <= sbe_history_write_count) {
                       sbe_char_index = 0;
                       //grab data entry to be saved
                       while (sbe_char_index < SBE_BUFFER_SIZE){
                          sbe_history_ptr[sbe_history_write_index].sbe_data[sbe_char_index] = sbe_input[sbe_char_index];
                          sbe_char_index++;
                                  }
                       //write 1-line data entry to file
                       retval = f_write(&fh, sbe_input, num_write, &nwritten);
                  //increment read counters
                       sbe_history_read_count++;
                                  }
               }
               //retrieve next datum from sbe circular buffer to store
             f_close(&fh);
             break;
//           default:
//             break;

//         */
//         /*
         case sbe_init:
 //            printf("streaming_task is in \"init\" state, count %d\r\n",streaming_run_count);

             OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
             // All tasks should be written as infinite loops.
             sbe_run_count++;
             if (sbe_run_count >= 5)
               {
                 sbe_ptr->sbe_curr_state = sbe_idle;
                 sbe_run_count = 0;
                 break;
               }
           break;
         case sbe_idle:
  //         printf("streaming_task is in \"idle\" state, with run_count %d\r\n",streaming_run_count);
           OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
           // All tasks should be written as infinite loops.
           sbe_run_count++;
           if (sbe_run_count >= 5)
             {
               sbe_ptr->sbe_curr_state = sbe_acq;
               sbe_run_count = 0;
               break;
             }
          break;
         case sbe_acq:
           //store sbe data to circular buffer
 //          printf("streaming_task is in \"streaming\" state, count %d\r\n",streaming_run_count);
           OSTimeDly (os_tick_rate/10, OS_OPT_TIME_PERIODIC,&p_err);
           // All tasks should be written as infinite loops.

           //Grab sbe49 data
           //sl_status_t sl_iostream_read(sl_iostream_t *stream,void *buffer,size_t buffer_length,size_t *bytes_read)
           status =
                     sl_iostream_read(sl_iostream_sbe49_handle,&sbe_input,buffer_length,&bytes_read);
           if(status != SL_STATUS_OK){
                    return;
                 }
           //want to check veracity of data
//           if ((strcmp(&sbe_input[strlen(sbe_input)-1], "\n") == 0) && (strcmp(&sbe_input[strlen(sbe_input)-2], "\r") == 0)){
//               printf("sbe49 data entry invalid");
//               return;
           }
           //save confirmed valid data to sbe circular buffer

           printf("sbe stream: %s\n",sbe_input);
           /*
           while (sbe_char_index < SBE_BUFFER_SIZE){
               sbe_history_ptr[sbe_history_write_index].sbe_data[sbe_char_index] = sbe_input[sbe_char_index];
               sbe_char_index++;
           } // end of while (sbe_char_index < SBE_BUFFER_SIZE)


           //Grab time stamp
           status =
                     sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(),&sbe_time_ms);
                 if(status != SL_STATUS_OK){
                     return;
                 }
           sbe_history_ptr[sbe_history_write_index].time_stamp_ms = sbe_time_ms;
           sbe_history_write_index++;
           sbe_history_write_index %= SBE_CIR_BUFF_SIZE;
           sbe_history_write_count++;

           sbe_run_count++;

           printf("sbe stream: %s\n",sbe_input); */
/*
           //testing sbe_logging later
           if (sbe_history_write_count >= 30) {
               sbe_ptr->sbe_curr_state = sbe_logging;
               break;
           }

           if (sbe_run_count >= 5)
             {
               sbe_ptr->sbe_curr_state = sbe_init;
               sbe_run_count = 0;
               break;
             }
             */
           break; //break out of case acq
//         default:
//           break;
//           */
#endif
        sbe_run_count++;
        if (sbe_run_count >= 30)
          {
             sbe_run_count = 0;
 //          f_close(&fh);
            break;
          }

     }
//     } //end bracket of sbe task while(DEF_TRUE)

}


/*
 * Description: acquire sbe data
 * state 1: find a sync
 * state 2: get the whole sbe string
 * state 3: checkinh to get a valid string
 * state 4: form a data_structure: timestamp, a valid sbe data, hall effect: speed & direction
 * return 1 if success
 */
//
void sbe_hex_to_binary_f(char *raw_sbe_data, int indx, main_lw_struct* main_lw_frame){ //ebohannon - 14 Oct 2022
//  raw_sbe_data
  int hex_read_idx = 0;
  int hex_table_idx = 0;
  int hex_countdown = 0;
  char *hex_table_value;
//  char hex_table[] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
  char *hex_table_string = "0123456789ABCDEF";

  char raw_temp[6] = {'\0'};
  char raw_con[6]= {'\0'};
  char raw_press[6]= {'\0'};
  char raw_tpc[4]= {'\0'};

  int dec_temp = 0;
  int dec_con = 0;
  int dec_press = 0;
  int dec_tpc = 0;

  hex_countdown = 5;
  while (hex_read_idx < 6){
      raw_temp[hex_read_idx] = raw_sbe_data[hex_read_idx]; //grab first char of data
      hex_table_value = strchr(hex_table_string,raw_temp[hex_read_idx%6]); //find first occurance of hex char in hex table
      hex_table_idx = (int)(hex_table_value - hex_table_string); //grab index of hex table value
      dec_temp = dec_temp + hex_table_idx*(pow(16,hex_countdown)); //use hex index and read index to sum up to get decimal value
      hex_countdown--;
      hex_read_idx++;
  }
  hex_countdown = 5;
  while (hex_read_idx < 12) {
      raw_con[hex_read_idx] = raw_sbe_data[hex_read_idx];
      hex_table_value = strchr(hex_table_string,raw_con[hex_read_idx%6]);
      hex_table_idx = (int)(hex_table_value - hex_table_string);
      dec_con = dec_con + hex_table_idx*(pow(16,hex_countdown));
      hex_countdown--;
      hex_read_idx++;
  }
  hex_countdown = 5;
  while (hex_read_idx < 18) {
      raw_press[hex_read_idx] = raw_sbe_data[hex_read_idx];
      hex_table_value = strchr(hex_table_string,raw_press[hex_read_idx%6]);
      hex_table_idx = (int)(hex_table_value - hex_table_string);
      dec_press = dec_press + hex_table_idx*(pow(16,hex_countdown));
      hex_countdown--;
      hex_read_idx++;
    }
  hex_countdown = 3;
  while (hex_read_idx < 22) {
      raw_tpc[hex_read_idx] = raw_sbe_data[hex_read_idx];
      hex_table_value = strchr(hex_table_string,raw_tpc[hex_read_idx%6]);
      hex_table_idx = (int)(hex_table_value - hex_table_string);
      dec_tpc = dec_tpc + hex_table_idx*(pow(16,hex_countdown));
      hex_countdown--;
      hex_read_idx++;
    }

  //store decoded data to lw decode circular buffer
  main_lw_frame->livewire_decode_cir_buff[indx].temp = dec_temp;
  main_lw_frame->livewire_decode_cir_buff[indx].con = dec_con;
  main_lw_frame->livewire_decode_cir_buff[indx].press = dec_press;
  main_lw_frame->livewire_decode_cir_buff[indx].tpc = dec_tpc;

  return;
}

static int sbe_acq_state = 0;
int sbe_acq_f(void *p_arg)
{
    int ret_val = 0;
    main_sbe_struct* sbe_ptr;

    sbe_ptr = p_arg;
    int status;
    char sbe_input[SBE_BUFFER_SIZE];
    static int req_bytes = 0;
    size_t bytes_read = 0;
    int timestamp = 0;
//    uint64_t timestamp = 0; // when change timestamp to uint64_t -> hang
    uint64_t tick_cnt = 0;
    static int sbe_state = 0;
    sbe_ptr->sbe_acq_data.acq_state = 0;
//    char sbe_str[125] = "\0";
    char dec_sbe_str[125] = "\0";
    int len = 0;
    sbe_state = 1;
    float hall_data;
    int indx = 0;
    int sbe_data_index = 0;

    int dec_temp = 0;
    int dec_con = 0;
    int dec_press = 0;
    int dec_tpc = 0;

//    FRESULT retval = 0;
//    FIL fh;
//    char file_name[] = "sbe_sim_sample.txt";
//    unsigned int nwritten;

   //create test file to log sbe49 data
//   fs_mount_f();
//   retval = f_open(&fh, file_name, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
//   if (retval != FR_OK)
//   {
//       printf("Failed to open %s, error %u\n", file_name, retval);
//   }
//   f_close(&fh);


//    motion_data_t rpm_history_ptr[RPM_HISTORY_CNT];
//    volatile uint32_t rpm_history_write_index;
//    volatile uint32_t rpm_history_write_count;

    initADC();

        while(DEF_TRUE)
          {
    switch (sbe_state)
    {
      case 1: // finding a sync
        sbe_ptr->sbe_acq_data.read_req_bytes = req_bytes = 1;
        status = sl_iostream_read(sl_iostream_sbe49_handle,&sbe_input,req_bytes,&bytes_read);
        if(status != SL_STATUS_OK)
        {
        //    printf("Failed reading to find a delimiter sbe data in sbe_acq(): %d\n", status);
        }
        else  // succeed to read
        {
           // printf("sbe %d bytes: %s\n",bytes_read,sbe_input);
            if (sbe_input[0] == '\n')   // find a delimiter - sync => set the req_bytes and go the the second state
            {
            sbe_ptr->sbe_acq_data.read_req_bytes = req_bytes = SBE_BUFFER_SIZE; // request bytes is 24 bytes
            sbe_ptr->sbe_acq_data.acq_state = 1;  // go to the next state: get the whole string
            sbe_state = 2;
            }
         }
        break;
      case 2: // get the whole sbe string
        req_bytes = SBE_BUFFER_SIZE;  // 24 chars
        status = sl_iostream_read(sl_iostream_sbe49_handle,&sbe_input,req_bytes,&bytes_read);
        if(status != SL_STATUS_OK)
        {
           //printf("Failed to read whole sbe data in sbe_acq(): %d \n",status);
        }
        else  // succeed reading
        {
  //          sl_iostream_write(sl_iostream_vcom_handle, sbe_input, bytes_read);
//            len = strlen(sbe_input);
//            printf("sbe %d bytes: %s\n",len,sbe_input);
            // checking for the valid string
            // sbe str: "03FDF9190C990866964D4F\r\n"
          if (sbe_input[bytes_read-1] == '\n')   // find a delimiter - sync => set the req_bytes and go the the second state
          {
              if (sbe_input[bytes_read-2] == '\r') // valid sbe string
              {
                  sbe_ptr->sbe_acq_data.acq_state = 2;  // go to state 2 to form a string and send to sbe
                  sbe_state = 3;
      //            printf("sbe %d bytes: %s\n",bytes_read,sbe_input);
    //              sl_iostream_write(sl_iostream_vcom_handle, sbe_input, bytes_read);
              }
           }

          else // not a valid string: the end of the string is not '\n' go back state 0 to find a delimiter as a sync
          {
            //printf("lost a sync, go back to sync case\n");
            sbe_ptr->sbe_acq_data.read_req_bytes = req_bytes = 1;
            sbe_state = 1;
          }
        } // end of succeed reading
        break;
      case 3: // form the data string and sending string out or/and to the sd card

        // Wait for conversion to be complete
        if(!(ADC0->STATUS & _ADC_STATUS_SINGLEDV_MASK));
          adc_sample = ADC_DataSingleGet(ADC0); // Get ADC result

        // Start ADC conversion
        ADC_Start(ADC0, adcStartSingle);

        // get timestamp
        timestamp = 1234567890;
//        tick_cnt = sl_sleeptimer_get_tick_count64();  // get tick count
//        status = sl_sleeptimer_tick64_to_ms(tick_cnt, &timestamp);  // get timestamp
        // form the data string

//        hall_data = rpm_history_ptr[rpm_history_write_index].rpm;- doesn't work
        hall_data = global_hall_rpm; //does work to get hall data
        //write function get_hall_data();

        //save data to lw_sample_struct before forming lw_str
        indx = lw_data.lw_cir_buf_write_cnt%LW_CIR_BUFF_SIZE; // get the current indx to write

        // update the value of lw_data_structure
        lw_data.livewire_cir_buff[indx].timestamp = timestamp;  // save timestamp
        sbe_data_index = 0;
        while (sbe_data_index < SBE_DATA_LENGTH) {
            lw_data.livewire_cir_buff[indx].ctd_data[sbe_data_index] = sbe_input[sbe_data_index];
            sbe_data_index++;
        }
        lw_data.livewire_cir_buff[indx].rpm = hall_data;
        lw_data.livewire_cir_buff[indx].adc_data = adc_sample;

        //decode data
//        void sbe_hex_to_binary_f(char *raw_sbe_data, int indx, main_lw_struct main_lw_frame){ //ebohannon - 14 Oct 2022
        sbe_hex_to_binary_f(sbe_input, indx, &lw_data);

        dec_temp = lw_data.livewire_decode_cir_buff[indx].temp;
        dec_con = lw_data.livewire_decode_cir_buff[indx].con;
        dec_press = lw_data.livewire_decode_cir_buff[indx].press;
        dec_tpc = lw_data.livewire_decode_cir_buff[indx].tpc;
        //stream decoded data
        snprintf(dec_sbe_str,SBE_ENTRY_MAX_SIZE,"$LWG%d-%d-%d,%d,%d-%d-%u\n",timestamp,dec_temp,dec_con,dec_press,dec_tpc,(int)hall_data,adc_sample);
        len = strlen(dec_sbe_str);
        sl_iostream_write(sl_iostream_vcom_handle, dec_sbe_str, len);

        snprintf(sbe_str,SBE_ENTRY_MAX_SIZE,"$LWG%d-%s-%d-%u\n",timestamp,lw_data.livewire_cir_buff[indx].ctd_data,(int)hall_data,adc_sample);
        //printf("send out: %s\n",sbe_str);
        len = strlen(sbe_str);
        sl_iostream_write(sl_iostream_vcom_handle, sbe_str, len);

        lw_data.lw_cir_buf_write_cnt++;
        // send to the file
//        retval = f_open(&fh, file_name, FA_WRITE|FA_OPEN_APPEND);
//        if (retval == FR_OK){
//            retval = f_write(&fh, sbe_str, len, &nwritten);
//            f_close(&fh);
//        }
        // I try to send out the structure, but it does not work as I expect - maibui 14 Oct 2022
//        len = sizeof(lw_data.livewire_cir_buff[indx]);
//        sl_iostream_write(sl_iostream_vcom_handle, &lw_data.livewire_cir_buff[indx], len);
        // then go back to state 2 to acquire the whole sbe data string
        sbe_ptr->sbe_acq_data.read_req_bytes = req_bytes = SBE_BUFFER_SIZE;
        sbe_ptr->sbe_acq_data.acq_state = 1;
        sbe_state = 2;
        break;
    }
          } // end of while loop

    return ret_val;
}
