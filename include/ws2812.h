#ifndef WS2812_CONTROL_H
#define WS2812_CONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "ws2812.pio.h"

#define WS2812_PIN 7
#define NUM_LEDS 25

extern PIO pio;
extern int sm;
extern uint dma_chan;
extern uint32_t matriz_led_5x5[NUM_LEDS];
extern uint32_t digit_frames[][NUM_LEDS];

void updateLEDS();
void setLEDS(uint32_t *estado);

#endif // WS2812_CONTROL_H
