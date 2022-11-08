
#include "hall_effects.h"

//Hall Effect Sensor Functions Header File
//by Ewan Bohannon
#include "stdbool.h"
#include "sl_sleeptimer.h"
#include "em_gpio.h"

//global variables
int hall_effect_direct = 1;
float hall_effect_ppm;
float hall_effect_rpm;
//motion_data_t rpm_history_ptr[RPM_HISTORY_CNT];
//volatile uint32_t rpm_history_write_index;
//volatile uint32_t rpm_history_write_count;
//local variables
int pulseCount;

uint64_t startTime;

uint64_t prevTime;

float pulseTimeU;
float pulseTimeV;
float pulseTimeW;

float AvPulseTime;
float global_hall_rpm = 0;


int test;
int count;
int prevcount;


bool hall_effect_u_value;
bool hall_effect_v_value;
bool hall_effect_w_value;

void hall_effect_init_f(void){
  sl_status_t status;

  status =
      sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(),&prevTime);          // Set startTime to current microcontroller elapsed time value
  if(status != SL_STATUS_OK){
      return;
  }

//  rpm_history_write_index = 0;
//  rpm_history_write_count = 0;
//  //initialize temporary null hall struct for streaming
//  rpm_history_ptr[rpm_history_write_index].time_stamp_ms = 0;
//  rpm_history_ptr[rpm_history_write_index].rpm = 0;

}

/**************************************************************************//**
 * @brief  HallV function
 *****************************************************************************/
void hall_sense_u_f(void)
{
  sl_status_t status;

      status =
          sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(),&startTime);          // Set startTime to current microcontroller elapsed time value in ms from ticks
      if(status != SL_STATUS_OK){
          return;
      }

  //millisCC0,1,2 - grab time
  hall_effect_u_value = GPIO_PinInGet(hall_effect_port_u, hall_effect_pin_u);         // Read the current W hall sensor value
  hall_effect_w_value = GPIO_PinInGet(hall_effect_port_w, hall_effect_pin_w);           // Read the current V (or U) hall sensor value
  hall_effect_direct = (hall_effect_u_value == hall_effect_w_value) ? HALL_EFFECT_CW : HALL_EFFECT_CCW;     // Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * hall_effect_direct);       // Add 1 to the pulse count
  pulseTimeU = startTime - prevTime;        // Calculate the current time between pulses
  hall_effect_ppm = (1000 / pulseTimeU) * 60;          // Calculate the pulses per min (1000 millis in 1 second)
  hall_effect_rpm = hall_effect_ppm / PULSE_PER_REV;           // Calculate revs per minute based on 90 pulses per rev
  prevTime = startTime;           // Remember the start time for the next interrupt

//  rpm_history_ptr[rpm_history_write_index].time_stamp_ms = startTime;
//  rpm_history_ptr[rpm_history_write_index].rpm = hall_effect_rpm;
  global_hall_rpm = hall_effect_direct*hall_effect_rpm;
//  rpm_history_write_index++;
//  rpm_history_write_index %= RPM_HISTORY_CNT;
//  rpm_history_write_count++;
}

/**************************************************************************//**
 * @brief  HallV function
 *****************************************************************************/
void hall_sense_v_f(void)
{
  sl_status_t status;

    status =
        sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(),&startTime);          // Set startTime to current microcontroller elapsed time value in ms from ticks
    if(status != SL_STATUS_OK){
        return;
    }

  hall_effect_v_value = GPIO_PinInGet(hall_effect_port_v, hall_effect_pin_v);         // Read the current W hall sensor value
  hall_effect_u_value = GPIO_PinInGet(hall_effect_port_u, hall_effect_pin_u);           // Read the current V (or U) hall sensor value
  hall_effect_direct = (hall_effect_v_value == hall_effect_u_value) ? HALL_EFFECT_CW : HALL_EFFECT_CCW;     // Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * hall_effect_direct);       // Add 1 to the pulse count
  pulseTimeV = startTime - prevTime;        // Calculate the current time between pulses
  hall_effect_ppm = (1000 / pulseTimeV) * 60;          // Calculate the pulses per min (1000 millis in 1 second)
  hall_effect_rpm = hall_effect_ppm / PULSE_PER_REV;           // Calculate revs per minute based on 90 pulses per rev
  prevTime = startTime;           // Remember the start time for the next interrupt

//  rpm_history_ptr[rpm_history_write_index].time_stamp_ms = startTime;
//  rpm_history_ptr[rpm_history_write_index].rpm = hall_effect_rpm;
  global_hall_rpm = hall_effect_direct*hall_effect_rpm;
//  rpm_history_write_index++;
//  rpm_history_write_index %= RPM_HISTORY_CNT;
//  rpm_history_write_count++;
}

/**************************************************************************//**
 * @brief  HallW function
 *
 *****************************************************************************/
void hall_sense_w_f(void)
{
  sl_status_t status;

  status =
      sl_sleeptimer_tick64_to_ms(sl_sleeptimer_get_tick_count64(),&startTime);          // Set startTime to current microcontroller elapsed time value in ms from ticks
  if(status != SL_STATUS_OK){
      return;
  }

  hall_effect_w_value = GPIO_PinInGet(hall_effect_port_w, hall_effect_pin_w);         // Read the current W hall sensor value
  hall_effect_v_value = GPIO_PinInGet(hall_effect_port_v, hall_effect_pin_v);           // Read the current V (or U) hall sensor value
  hall_effect_direct = (hall_effect_w_value == hall_effect_v_value) ? HALL_EFFECT_CW : HALL_EFFECT_CCW;     // Determine rotation direction (ternary if statement)
  pulseCount = pulseCount + (1 * hall_effect_direct);       // Add 1 to the pulse count
  pulseTimeW = (float)(startTime - prevTime);        // Calculate the current time between pulses
  hall_effect_ppm = (1000 / pulseTimeW) * 60;          // Calculate the pulses per min (1000 millis in 1 second)
  hall_effect_rpm = ((float)hall_effect_direct)*hall_effect_ppm / PULSE_PER_REV;           // Calculate revs per minute based on 90 pulses per rev
  prevTime = startTime;  // Remember the start time for the next interrupt


//  rpm_history_ptr[rpm_history_write_index].time_stamp_ms = startTime;
//  rpm_history_ptr[rpm_history_write_index].rpm = hall_effect_rpm;
  global_hall_rpm = hall_effect_direct*hall_effect_rpm;
//  rpm_history_write_index++;
//  rpm_history_write_index %= RPM_HISTORY_CNT;
//  rpm_history_write_count++;
}
