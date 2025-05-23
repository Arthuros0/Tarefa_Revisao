#ifndef MATRIZ_LEDS_H
#define MATRIZ_LEDS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

#define VERDE 11  
#define VERMELHO 13

extern uint sm;

void init_led(uint8_t pin);

void muda_estado(uint8_t pin);

uint8_t obter_index(uint8_t i);

void desenha_frame(const uint32_t matriz[2][25],uint8_t frame);

void apaga_matriz();

void setup_led_matrix();


extern const uint32_t estados[2][25];
#endif