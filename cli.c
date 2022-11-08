/***************************************************************************//**
 * @file
 * @brief cli micrium os kernel examples functions
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
#include <string.h>
#include <stdio.h>
#include "cli.h"
#include "sl_cli.h"
#include "sl_cli_instances.h"
#include "sl_cli_arguments.h"
#include "sl_cli_handles.h"
#include "sl_simple_led.h"
#include "sl_simple_led_instances.h"
#include "os.h"
#include "ff.h"
#include "sdio.h"
#include "diskio.h"
//#include "mod_fatfs_chan.h"


/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define CLI_DEMO_TASK_STACK_SIZE     256
#define CLI_DEMO_TASK_PRIO            15

/*******************************************************************************
 *********************   LOCAL FUNCTION PROTOTYPES   ***************************
 ******************************************************************************/

//void echo_str(sl_cli_command_arg_t *arguments);
//void echo_int(sl_cli_command_arg_t *arguments);
//void led_cmd(sl_cli_command_arg_t *arguments);
void wtest_cmd_f(sl_cli_command_arg_t *arguments);
void cli_ls_cmd_f(sl_cli_command_arg_t *arguments);
//FRESULT scan_files_1 (char* path );
void mountfs_cmd_f(sl_cli_command_arg_t *arguments);
void unmountfs_cmd_f(sl_cli_command_arg_t *arguments);
void catfs_cmd_f(sl_cli_command_arg_t *arguments);
void cli_rm_cmd_f (sl_cli_command_arg_t *arguments);
void cli_mkdir_cmd_f (sl_cli_command_arg_t *arguments);
void cli_mv_cmd_f (sl_cli_command_arg_t *arguments);
void a2d_start_cmd_f (sl_cli_command_arg_t *arguments);
void a2d_stop_cmd_f (sl_cli_command_arg_t *arguments);

#ifdef TASK_TEST
/* Example Task Defines:            */
#define  APP_EXAMPLE_TASK_PRIO            21u  /*   Task Priority.                 */
#define  APP_EXAMPLE_TASK_STK_SIZE       256u  /*   Stack size in CPU_STK.         */

                                                        /* Example Task Data:      */
OS_TCB   App_ExampleTaskTCB;                            /*   Task Control Block.   */
CPU_STK  App_ExampleTaskStk[APP_EXAMPLE_TASK_STK_SIZE]; /*   Stack.                */

                                                        /* Example Task Code:      */
void  App_ExampleTask (void  *p_arg);                   /*   Function.             */

void  App_TaskCreate (void)
{
    RTOS_ERR     err;

    OSTaskCreate(&App_ExampleTaskTCB,                /* Pointer to the task's TCB.  */
                 "Example Task.",                    /* Name to help debugging.     */
                 &App_ExampleTask,                   /* Pointer to the task's code. */
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

void  App_ExampleTask (void  *p_arg)
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

#endif

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/
/*
static const sl_cli_command_info_t cmd__echostr = \
  SL_CLI_COMMAND(echo_str,
                 "echoes string arguments to the output",
                 "Just a string...",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

static const sl_cli_command_info_t cmd__echoint = \
  SL_CLI_COMMAND(echo_int,
                 "echoes integer arguments to the output",
                 "Just a number...",
                 { SL_CLI_ARG_INT8, SL_CLI_ARG_ADDITIONAL, SL_CLI_ARG_END, });

static const sl_cli_command_info_t cmd__led = \
  SL_CLI_COMMAND(led_cmd,
                 "Change an led status",
                 "led number: 0 or 1"SL_CLI_UNIT_SEPARATOR "instruction: on, off, or toggle",
                 { SL_CLI_ARG_UINT8, SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });
*/
static const sl_cli_command_info_t cmd__wtest = \
  SL_CLI_COMMAND(wtest_cmd_f,
                 "tests file system",
                 "enter a file name and a string to store",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

static const sl_cli_command_info_t cmd__ls = \
  SL_CLI_COMMAND(cli_ls_cmd_f,
                 "tests file system ls cmd",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

// maibui - 2 Aug 2022
static const sl_cli_command_info_t cmd__mount = \
  SL_CLI_COMMAND(mountfs_cmd_f,
                 "mount file system cmd",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

// maibui - 2 Aug 2022
static const sl_cli_command_info_t cmd__unmount = \
  SL_CLI_COMMAND(unmountfs_cmd_f,
                 "unmount file system",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

// maibui - 2 Aug 2022
static const sl_cli_command_info_t cmd__cat = \
  SL_CLI_COMMAND(catfs_cmd_f,
                 "cat the file",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

// maibui - 2 Aug 2022
static const sl_cli_command_info_t cmd__rm = \
  SL_CLI_COMMAND(cli_rm_cmd_f,
                 "remove the file",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

// maibui - 3 Aug 2022
static const sl_cli_command_info_t cmd__mdir = \
  SL_CLI_COMMAND(cli_mkdir_cmd_f,
                 "make a new directory",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

// maibui - 3 Aug 2022
static const sl_cli_command_info_t cmd__mv = \
  SL_CLI_COMMAND(cli_mv_cmd_f,
                 "rename the file",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

// maibui - 10 Sept 2022
static const sl_cli_command_info_t cmd__a2d_start = \
  SL_CLI_COMMAND(a2d_start_cmd_f,
                 "a2d_start log",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });
// maibui - 10 Sept 2022
static const sl_cli_command_info_t cmd__a2d_stop = \
  SL_CLI_COMMAND(a2d_stop_cmd_f,
                 "a2d_stop log",
                 "",
                 { SL_CLI_ARG_WILDCARD, SL_CLI_ARG_END, });

/*
static sl_cli_command_entry_t a_table[] = {
  { "echo_str", &cmd__echostr, false },
  { "echo_int", &cmd__echoint, false },
  { "led", &cmd__led, false },
  { "wtest", &cmd__fswtest, false },
  { "ls", &cmd__ls_cmd, false },
  { NULL, NULL, false },
};
*/
static sl_cli_command_entry_t a_table[] = {
  { "mount_fs", &cmd__mount, false },
  { "unmount_fs", &cmd__unmount, false },
  { "ls", &cmd__ls, false },
  { "wtest", &cmd__wtest, false },
  { "cat", &cmd__cat, false },
  { "rm", &cmd__rm, false },
  { "mkdir", &cmd__mdir, false },
  { "mv", &cmd__mv, false },
  { "a2d_start", &cmd__a2d_start},
  { "a2d_stop", &cmd__a2d_stop},
  { NULL, NULL, false },
};

static sl_cli_command_group_t a_group = {
  { NULL },
  false,
  a_table
};

/*******************************************************************************
 *************************  EXPORTED VARIABLES   *******************************
 ******************************************************************************/

sl_cli_command_group_t *command_group = &a_group;

/*******************************************************************************
 *************************   LOCAL FUNCTIONS   *********************************
 ******************************************************************************/

/***************************************************************************//**
 * Callback for echo_str
 *
 * This function is used as a callback when the echo_str command is called
 * in the cli. It simply echoes back all the arguments provided as strings.
 ******************************************************************************/
/*
void echo_str(sl_cli_command_arg_t *arguments)
{
  char *ptr_string;

  printf("<<echo_str command>>\r\n");

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++) {
    ptr_string = sl_cli_get_argument_string(arguments, i);

    printf("%s\r\n", ptr_string);
  }
}
*/
/***************************************************************************//**
 * Callback for echo_int
 *
 * This function is used as a callback when the echo_int command is called
 * in the cli. It simply echoes back all the arguments provided as integers.
 ******************************************************************************/
/*
void echo_int(sl_cli_command_arg_t *arguments)
{
  int8_t argument_value;

  printf("<<echo_int command>>\r\n");

  // Read all the arguments provided as integers and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++) {
    argument_value = sl_cli_get_argument_int8(arguments, i);

    printf("%i\r\n", argument_value);
  }
}*/

/***************************************************************************//**
 * Callback for the led
 *
 * This function is used as a callback when the led command is called
 * in the cli. The command is used to turn on, turn off and toggle leds.
 ******************************************************************************/
/*void led_cmd(sl_cli_command_arg_t *arguments)
{
  uint8_t led_number;
  char *instruction;
  sl_led_t led;

  // Make sure that both the arguments were provided
  if (sl_cli_get_argument_count(arguments) < 2) {
    printf("Please provide inputs.\r\n");
    return;
  }

  // Read the provided led number and verify it is valid
  led_number = sl_cli_get_argument_uint8(arguments, 0);
  if (led_number > 1) {
    printf("Invalid led. Only led 0 and 1 present.\r\n");
    return;
  }

  // Get an handle to the appropriate led
  led = led_number == 0 ? sl_led_led0 : sl_led_led1;

  // Get the instruction provided
  instruction = sl_cli_get_argument_string(arguments, 1);

  if (strcmp(instruction, "on") == 0) {
    // led on instruction provided
    sl_led_turn_on(&led);
  } else if (strcmp(instruction, "off") == 0) {
    // led off instruction provided
    sl_led_turn_off(&led);
  } else if (strcmp(instruction, "toggle") == 0) {
    // led toggle instruction provided
    sl_led_toggle(&led);
  } else {
    // led off instruction provided
    printf("Incorrect instruction. Please use on, off or toggle\r\n");
  }
}
*/
/***************************************************************************//**
 * Callback for wtest
 *
 * This function is used as a callback when the wtest command is called
 * in the cli. It simply writing a provided strings in to the provided file
 ******************************************************************************/
void wtest_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;
  char *filename;

  printf("<<fswrite command>>\r\n");

  if (sl_cli_get_argument_count(arguments)!=2)
  {
    printf("command: wtest filename str_to_write\n");
    return;
  }

  // command: fswtest filename string => filename = arg0, str = arg1

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++)
  {
 //     if (i==0)
        filename = sl_cli_get_argument_string(arguments, 0);
 //     if (i==1)
        ptr_string = sl_cli_get_argument_string(arguments, 1);
  }
  printf("fn = %s, str = %s\r\n",filename, ptr_string);

  fs_write_tst_f(filename,ptr_string);  // define in mod_fatfs_chan.c

}

void mountfs_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;

  printf("<<mount command>>\r\n");

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++) {
    ptr_string = sl_cli_get_argument_string(arguments, i);
  }
  printf("%s\r\n", ptr_string);

  fs_mount_f();

}

void unmountfs_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;

  printf("<<unmount command>>\r\n");

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++) {
    ptr_string = sl_cli_get_argument_string(arguments, i);
  }
  printf("%s\r\n", ptr_string);

  fs_unmount_f();

}

void catfs_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;
  char *filename;

  printf("<<cat command>>\r\n");
  if (sl_cli_get_argument_count(arguments)!=1)
  {
    printf("command: cat filename\n");
    return;
  }

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++) {
    ptr_string = filename = sl_cli_get_argument_string(arguments, i);
  }

  printf("%s\r\n", ptr_string);

  fs_cat_f(filename);

}


void cli_ls_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;

  printf("<<ls command>>\r\n");

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++) {
    ptr_string = sl_cli_get_argument_string(arguments, i);
  }
  printf("%s\r\n", ptr_string);

  fs_ls_cmd_f();

}

/***************************************************************************//**
 * Callback for rm (remove the file)
 *
 * This function is used as a callback when the rm command is called
 * in the cli. It simply remove the provided file
 ******************************************************************************/
void cli_rm_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;

  printf("<<rm command>>\r\n");

  if (sl_cli_get_argument_count(arguments)!=1)
  {
    printf("usage: rm filename\n");
    return;
  }

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++)
  {
        ptr_string = sl_cli_get_argument_string(arguments, i);
  }
  printf("fn = %s\r\n",ptr_string);

  fs_rm_cmd_f(ptr_string);  // define in mod_fatfs_chan.c

}

/***************************************************************************//**
 * Callback for mkdir (make a new directory)
 *
 * This function is used as a callback when the mkdir command is called
 * in the cli. It simply make a new directory
 ******************************************************************************/

void cli_mkdir_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *dir_name;

  printf("<<mkdir command>>\r\n");
  if (sl_cli_get_argument_count(arguments)!=1)
  {
    printf("usage: mkdir dirname\n");
    return;
  }

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++)
  {
      dir_name = sl_cli_get_argument_string(arguments, i);
  }
  printf("dirname = %s\r\n",dir_name);

  if (dir_name == NULL)
  {
    printf("usage: mkdir <dirname>");
    return;
  }
  fs_mkdir_cmd_f(dir_name);
}

/***************************************************************************//**
 * Callback for mkdir (make a new directory)
 *
 * This function is used as a callback when the mkdir command is called
 * in the cli. It simply make a new directory
 ******************************************************************************/

void cli_mv_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *old_fn;
  char *new_fn;

  printf("<<mv command>>\r\n");
  if (sl_cli_get_argument_count(arguments)!=2)
  {
    printf("usage: mv old_filename new_filename\n");
    return;
  }

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++)
  {
      old_fn = sl_cli_get_argument_string(arguments, 0);
      new_fn = sl_cli_get_argument_string(arguments, 1);
  }
  printf("old_fname = %s, new_fname = %s\r\n",old_fn, new_fn);

  fs_mv_cmd_f(old_fn, new_fn);
}
/***************************************************************************//**
 * Callback for a2d start
 *
 * This function is used as a callback when the rm command is called
 * in the cli. It simply remove the provided file
 ******************************************************************************/
void a2d_start_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;

  printf("<<a2d_start command>>\r\n");

  if (sl_cli_get_argument_count(arguments)>1)
  {
    printf("usage: a2d_start\n");
    return;
  }

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++)
  {
        ptr_string = sl_cli_get_argument_string(arguments, i);
  }
  printf("command = %s\r\n",ptr_string);

//  fs_a2d_start_cmd_f(ptr_string);  // define in mod_fatfs_chan.c

}

/***************************************************************************//**
 * Callback for a2d start
 *
 * This function is used as a callback when the rm command is called
 * in the cli. It simply remove the provided file
 ******************************************************************************/
void a2d_stop_cmd_f (sl_cli_command_arg_t *arguments)
{
  char *ptr_string;

  printf("<<a2d_stop command>>\r\n");

  if (sl_cli_get_argument_count(arguments)>1)
  {
    printf("usage: a2d_stop\n");
    return;
  }

  // Read all the arguments provided as strings and print them back
  for (int i = 0; i < sl_cli_get_argument_count(arguments); i++)
  {
        ptr_string = sl_cli_get_argument_string(arguments, i);
  }
  printf("command = %s\r\n",ptr_string);

//  fs_a2d_stop_cmd_f(ptr_string);  // define in mod_fatfs_chan.c

}

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/*******************************************************************************
 * Initialize cli example.
 ******************************************************************************/
bool cli_app_init(void)
{
  bool status = 0;
  int retval = 0;

  status = sl_cli_command_add_command_group(sl_cli_inst_handle, command_group);
  EFM_ASSERT(status);

  return retval;
}
