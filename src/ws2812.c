#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "ws2812.pio.h"

// Definição do pino
#define WS2812_PIN 7

#define NUM_LEDS 25

// Variáveis Globais
PIO pio;
int sm;
uint dma_chan;
uint32_t matriz_led_5x5[NUM_LEDS];

// Representação do Dígitos no WS2812
uint32_t digit_frames[][NUM_LEDS] = {
    {// Dígito 0
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff},
    {// Dígito 1
     0x0000, 0x0000, 0xffff, 0x0000, 0x0000,
     0x0000, 0x0000, 0xffff, 0x0000, 0x0000,
     0x0000, 0x0000, 0xffff, 0x0000, 0x0000,
     0x0000, 0x0000, 0xffff, 0x0000, 0x0000,
     0x0000, 0x0000, 0xffff, 0xffff, 0x0000},
    {// Dígito 2
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0x0000,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0x0000, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff},
    {// Dígito 3
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0x0000, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff},
    {// Dígito 4
     0xffff, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff},
    {// Dígito 5
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0x0000, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0x0000,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff},
    {// Dígito 6
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0x0000,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff},
    {// Dígito 7
     0x0000, 0x0000, 0x0000, 0x0000, 0xffff,
     0x0000, 0xffff, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0xffff, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0000, 0xffff, 0x0000,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff},
    {// Dígito 8
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff},
    {// Dígito 9
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0x0000, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
     0xffff, 0x0000, 0x0000, 0x0000, 0xffff,
     0xffff, 0xffff, 0xffff, 0xffff, 0xffff}};

// Função para atualizar os LEDs usando DMA
void updateLEDS()
{
    dma_channel_wait_for_finish_blocking(dma_chan);
    while (!pio_sm_is_tx_fifo_empty(pio, sm))
    {
        sleep_us(10);
    }
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true));
    dma_channel_configure(dma_chan, &c, &pio->txf[sm], matriz_led_5x5, NUM_LEDS, true);
    sleep_us(300);
}

// Função para definir o estado dos LEDs
void setLEDS(uint32_t *estado)
{
    memcpy(matriz_led_5x5, estado, sizeof(matriz_led_5x5));
    updateLEDS();
}