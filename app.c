/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "app.h"
#include "simple_button_kernel.h"

 main_app_struct_ptr app_struct_ptr;
/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
int app_init(void)
{
  int retval = 0;
  bool status = 0;

  printf("\r\nStarted MOD cli fatfs micrium OS... ver: 18 Oct 2022\r\n\r\n");

  // adding initialize for fatfs - mb 12 Sept 2022
  retval = fs_bsp_init();
  if (retval) printf("failed initialize fatfs from app_init()\n");
  printf("\r\n Done initialize bsp\r\n");

  if ((app_struct_ptr = (main_app_struct*)malloc(sizeof(main_app_struct))) == NULL)
      printf("Failed to allocate the main structure\r\n");

 // all_tasks_init(); // initialize all tasks in here
//  acq_task_init(&app_struct_ptr->acq_struct);
//  printf("\r\nDone initialize all task\r\n");


  // add the tasks
  // set all vars of task1 needs and create the task
 // if(TASK_1)
 //   task_1_create();

 // if(ACQ_TASK)
 //   acq_task_create(&app_struct_ptr->acq_struct);

//  if(LOGGING_TASK)
//    logging_task_create(&app_struct_ptr->logging_struct);

//  if(STREAMING_TASK)
//    streaming_task_create(&app_struct_ptr->streaming_struct);
  simple_button_init();
  sbe_task_create(&app_struct_ptr->sbe_struct);
  status = cli_app_init();  // cli_init()

  if(status)
    printf("failed initialize cli_app_init() from app_init()\n");
  printf("\r\nDone initialize cli\r\n");

    return retval;
}

int app_cleanup(void)
{
  int retval = 0;
  if (app_struct_ptr)
    {
      free(app_struct_ptr);
      app_struct_ptr = NULL;
    }
  return retval;

}
