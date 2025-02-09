#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "ws2812.h"
#include "ws2812.pio.h"
#include "ssd1306.h"
#include "font.h"
#include "button.h"

// Configura a GPIO
void init_gpio(uint8_t pin, bool dir)
{
    gpio_init(pin);
    gpio_set_dir(pin, dir);
    if (dir == GPIO_IN)
        gpio_pull_up(pin);
}

void setup()
{
    // Configura a comunicação serial uart e usb
    stdio_init_all();

    // Inicializa os LEDS Verde e Azul
    init_gpio(LED_B_PIN, GPIO_OUT);
    init_gpio(LED_G_PIN, GPIO_OUT);

    // Inicializa os botões A e B, além da interrupção ao ser pressionado
    init_gpio(BTN_A_PIN, GPIO_IN);
    init_gpio(BTN_B_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    gpio_set_irq_enabled(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Configura a Matriz de Leds 5x5 e a PIO para comunicação
    init_gpio(WS2812_PIN, GPIO_OUT);
    pio = pio0;
    sm = 0;
    dma_chan = dma_claim_unused_channel(true);
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    // Configura a comunicação I2C com SSD1306
    // Inicializa o I2C em 400Khz
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                 // Define o SDA do I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                 // Define o SCL do I2C
    gpio_pull_up(I2C_SDA);                                     // Colocar um Resitor Pull Up para o data line
    gpio_pull_up(I2C_SCL);                                     // Colocar um Resitor Pull Up para o clock line
    ssd1306_init(WIDTH, HEIGHT, false, I2C_ADDRESS, I2C_PORT); // Inicializa o display
    ssd1306_config();                                          // Configura o display
    ssd1306_send_data();                                       // Envia os dados para o display

    // Limpa o display ssd1306
    ssd1306_fill(false);
    ssd1306_send_data();
}

int main()
{
    setup();

    char last_input = '\0';
    while (true)
    {
        char c = getchar_timeout_us(0); // Recebe um caractere enviado pelo serial
        if (c != PICO_ERROR_TIMEOUT)
        {
            // Verifica se é uma letra e salva o caractere para exibir no display SSD1306
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                last_input = c;
            }
            // Verifica se é um número e salva o caractere para exibir no display SSD1306 e na matriz de leds WS2812
            else if ((c >= '0' && c <= '9'))
            {
                last_input = c;
                setLEDS(digit_frames[c - '0']); // Atualiza o número exibido no WS2812
            }
        }

        ssd1306_fill(false);                            // Limpa o display
        ssd1306_rect(90, 84, 32, 16, true, false);      // Coloca a borda do ON e OFF do LED AZUL
        ssd1306_rect(90 + 18, 84, 32, 16, true, false); // Coloca a borda do ON e OFF do LED VERDE
        ssd1306_draw_string("LED AZUL", 8, 30);         // Exibe "LED AZUL" no display
        ssd1306_draw_string("LED VERDE", 8, 48);        // Exibe "LED VERDE" no display

        // Mostra o estado do LED AZUL no SSD1306
        if (gpio_get(LED_B_PIN))
            ssd1306_draw_string("ON", 93, 30);
        else
            ssd1306_draw_string("OFF", 90, 30);

        // Mostra o estado do LED VERDE no SSD1306
        if (gpio_get(LED_G_PIN))
            ssd1306_draw_string("ON", 93, 48);
        else
            ssd1306_draw_string("OFF", 90, 48);

        // Exibe no diplay se o último caractere recebido é um dígito ou uma letra
        ssd1306_draw_string(
            (last_input >= '0' && last_input <= '9')
                ? "Digito: ' '"
            : (last_input >= 'A' && last_input <= 'Z') || (last_input >= 'a' && last_input <= 'z')
                ? "Letra:  ' '"
                : "",
            8, 10);
        ssd1306_draw_char(last_input, 80, 10); // Mostra o último caractere obtido no serial
        ssd1306_send_data();                   // Atualiza o display o SSD1306

        sleep_ms(200);
    }
}
