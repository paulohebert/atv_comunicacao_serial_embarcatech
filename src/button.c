#include <stdio.h>
#include "pico/stdlib.h"
#include "button.h"

static volatile uint32_t last_time = 0;

// Interrupção ao botão ser pressionado
void button_irq_handler(unsigned int gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Tratamento do bouncing
    if (current_time - last_time > 250000)
    {
        last_time = current_time;

        // Verifica o botão pressionado
        if (gpio == BTN_A_PIN)
        {
            // Alterna o estado do LED Verde
            gpio_put(LED_G_PIN, !gpio_get(LED_G_PIN));
            printf("Botão A Pressionado\n");
            printf("LED VERDE %s\n", gpio_get(LED_G_PIN) ? "LIGADO" : "DESLIGADO");
        }
        else if (gpio == BTN_B_PIN)
        {
            // Alterna o estado do LED Azul
            gpio_put(LED_B_PIN, !gpio_get(LED_B_PIN));
            printf("Botão B Pressionado\n");
            printf("LED AZUL %s\n", gpio_get(LED_B_PIN) ? "LIGADO" : "DESLIGADO");
        }
    }
}
