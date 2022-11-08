/***************************************************************************//**
 * @file
 * @brief Simple button kernel examples functions
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

#include "sl_simple_led_instances.h"
#include "sl_simple_button_instances.h"
#include "simple_button_kernel.h"
#include "os.h"
#include "hall_effects.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#ifndef TASK_STACK_SIZE
#define TASK_STACK_SIZE      64
#endif

#ifndef TASK_PRIO
#define TASK_PRIO            24
#endif

#ifndef BUTTON_INSTANCE_0
#define BUTTON_INSTANCE_0   sl_button_hall_effect_u
#endif

#ifndef LED_INSTANCE_0
#define LED_INSTANCE_0      sl_led_led0
#endif

#ifndef BUTTON_INSTANCE_1
#define BUTTON_INSTANCE_1   sl_button_hall_effect_v
#endif

#ifndef LED_INSTANCE_1
#define LED_INSTANCE_1      sl_led_led1
#endif

#ifndef BUTTON_INSTANCE_2
#define BUTTON_INSTANCE_2   sl_button_hall_effect_w
#endif

#ifndef LED_INSTANCE_2
#define LED_INSTANCE_2      sl_led_led2
#endif

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/

static OS_TCB tcb;
static CPU_STK stack[TASK_STACK_SIZE];

/*******************************************************************************
 *********************   LOCAL FUNCTION PROTOTYPES   ***************************
 ******************************************************************************/

static void simple_button_task(void *arg);

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * Initialize example.
 ******************************************************************************/
void simple_button_init(void)
{
  RTOS_ERR err;

  // Create Task
  OSTaskCreate(&tcb,
               "simple button task",
               simple_button_task,
               DEF_NULL,
               TASK_PRIO,
               &stack[0],
               (TASK_STACK_SIZE / 10u),
               TASK_STACK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);
  EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
}

/***************************************************************************//**
 * Task.
 ******************************************************************************/
static void simple_button_task(void *arg)
{
  (void)&arg;
  do {
    RTOS_ERR err;

    // Delay task to allow sleep
    OSTimeDlyHMSM(24, 0, 0, 0, OS_OPT_TIME_DLY, &err);
    EFM_ASSERT((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE));
  } while (1);
}

/***************************************************************************//**
 * Callback on button change.
 ******************************************************************************/
void sl_button_on_change(const sl_button_t *handle)
{
  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
    if (&BUTTON_INSTANCE_0 == handle) {
      sl_led_toggle(&LED_INSTANCE_0);
      hall_sense_u_f();

    }
    else if (&BUTTON_INSTANCE_1 == handle) {
      sl_led_toggle(&LED_INSTANCE_1);
      hall_sense_v_f();
    }
    else if (&BUTTON_INSTANCE_2 == handle) {
      sl_led_toggle(&LED_INSTANCE_2);
      hall_sense_w_f();
    }
  }
}
