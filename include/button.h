#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <stdint.h>

// Define LED pins
#define LED_B_PIN 12
#define LED_G_PIN 11

// Define Button pins
#define BTN_A_PIN 5
#define BTN_B_PIN 6

void button_irq_handler(unsigned int gpio, uint32_t events);

#endif // BUTTON_HANDLER_H