#ifndef JOY_BUTTON_H
#define JOY_BUTTON_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include <stdio.h>
#include "matriz_leds.h"

#define Y_JOY 26  
#define X_JOY 27  
#define Y_CHANNEL 0 //Canal do ADC vinculado ao Y_JOY
#define X_CHANNEL 1 //Canal do ADC vinculado ao X_JOY 

#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_SW 22

extern volatile absolute_time_t debounce_button;
extern volatile bool shoot;
extern volatile bool dificil;
extern volatile bool strike;
extern uint8_t pos_x;
extern uint8_t pos_y;
extern uint8_t pos_atual;
extern uint8_t pos_anterior;

void joystick_setup();      //Configura Joystick
void joystick_pwm_display_control(ssd1306_t *ssd); //Controla LED e Display
void button_callback(uint pin, uint32_t events);
void button_init(uint8_t pin);
int64_t game_timer(alarm_id_t id, void *user_data);

#endif