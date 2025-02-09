# Comunicação Serial com RP2040

Atividade 1 da Unidade 4 | Capítulo 6 do curso, projeto visa consolidar os conceitos de comunicação serial (UART, I2C) e manipulação de hardware utilizando a placa BitDogLab com o microcontrolador RP2040. O sistema integra LEDs endereçáveis, um display OLED, botões e comunicação serial para criar interações dinâmicas.

## 🎯 Objetivos
- Implementar comunicação serial via UART e I2C.
- Controlar LEDs comuns e endereçáveis (WS2812).
- Utilizar interrupções e debounce para tratamento de botões.
- Exibir informações em um display SSD1306.
- Integrar entrada de dados via Serial Monitor.

---

## 🔧 Componentes Utilizados
| Componente               | Conexão na BitDogLab      |
|--------------------------|---------------------------|
| Matriz de LEDs WS2812 5x5| GPIO 7                    |
| LED RGB                  | GPIO 13 (Vermelho), 12 (Azul), 11 (Verde) |
| Botão A                  | GPIO 5                    |
| Botão B                  | GPIO 6                    |
| Display SSD1306 (I2C)    | GPIO 14 (SDA), GPIO 15 (SCL) |

---

## 💻 Configuração do Ambiente
1. Instale o **Pico SDK** conforme a documentação oficial do Raspberry Pi.
2. Configure o **CMake** e o **Visual Studio Code** para compilar códigos para o Raspberry Pi Pico.
3. Integre o **Wokwi** ao VS Code para simulação do circuito.
4. Clone este repositório:
   ```sh
   git clone https://github.com/paulohebert/atv_comunicacao_serial_embarcatech
   cd atv_comunicacao_serial_embarcatech
   ```
---

## ⚙️ Compilação e Upload
1. Compile o código utilizando o CMake:
   ```sh
   mkdir build
   cd build
   cmake ..
   ninja
   ```
2. Transfira o arquivo `.uf2` gerado para o BitDogLab.

---

## 🚀 Funcionalidades
1. **Entrada Serial**  
   - Exibição de caracteres recebidos via serial no display SSD1306.
   - Exibição de Números (0-9) na matriz WS2812.
2. **Interação com Botões**  
   - **Botão A**: Alterna o LED verde do RGB e exibe mensagens no display SSD1306 e Serial Monitor.  
   - **Botão B**: Alterna o LED azul do RGB e exibe mensagens no display SSD1306 e Serial Monitor.
