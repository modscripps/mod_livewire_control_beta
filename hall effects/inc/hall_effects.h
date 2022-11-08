//Hall Effect Sensor Functions Header File
//by Ewan Bohannon


#ifndef HALL_EFFECT_H
#define HALL_EFFECT_H

#include "sl_simple_button_HALL_EFFECT_U_config.h"
#include "sl_simple_button_HALL_EFFECT_V_config.h"
#include "sl_simple_button_HALL_EFFECT_W_config.h"

#define HALL_EFFECT_CW 1
#define HALL_EFFECT_CCW -1

//TODO redefine in common file
#define hall_effect_port_u SL_SIMPLE_BUTTON_HALL_EFFECT_U_PORT //PD1..pin to read Hall Effect voltage
//PF0 initially chosen, but when used, the IFS is already set upon init and GPIO is high
#define hall_effect_port_v SL_SIMPLE_BUTTON_HALL_EFFECT_V_PORT //PF1
#define hall_effect_port_w SL_SIMPLE_BUTTON_HALL_EFFECT_W_PORT //PF2

#define hall_effect_pin_u SL_SIMPLE_BUTTON_HALL_EFFECT_U_PIN //PD1..pin to read Hall Effect voltage
//PF0 initially chosen, but when used, the IFS is already set upon init and GPIO is high
#define hall_effect_pin_v SL_SIMPLE_BUTTON_HALL_EFFECT_V_PIN //PF1
#define hall_effect_pin_w SL_SIMPLE_BUTTON_HALL_EFFECT_W_PIN //PF2
#define PULSE_PER_REV 4

#define RPM_HISTORY_CNT 100

//these are the be non-global
extern int hall_effect_direct;
extern float hall_effect_ppm;
extern float hall_effect_rpm;

//typedef struct {
//  uint64_t time_stamp_ms;
//  float rpm; //speed and directions
//} motion_data_t;

//extern motion_data_t rpm_history_ptr[];
//extern volatile uint32_t rpm_history_write_index;
//extern volatile uint32_t rpm_history_write_count;
extern float global_hall_rpm;

void hall_effect_init_f(void);

/**************************************************************************//**
 * @brief  HallV function
 *****************************************************************************/
void hall_sense_u_f(void);

/**************************************************************************//**
 * @brief  HallV function
 *****************************************************************************/
void hall_sense_v_f(void);

/**************************************************************************//**
 * @brief  HallW function
 *****************************************************************************/
void hall_sense_w_f(void);

/**************************************************************************//**
 * @brief  get_hall_data function
 *****************************************************************************/
void get_hall_data_f(void);

#endif // HALL_EFFECT_H
