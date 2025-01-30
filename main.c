#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "main.pio.h"

//número de LEDs
#define NUM_PIXELS 25

#define LED_PIN_RED 13

//pino de saída
#define OUT_PIN 7

#define DEBOUNCE_TIME_MS 200 // Tempo de debounce em milissegundos

// Variáveis para armazenar o tempo da última interrupção
absolute_time_t last_interrupt_time_0 = {0};
absolute_time_t last_interrupt_time_1 = {0};
//botão de interupção
const uint button_0 = 5;
const uint button_1 = 6;
bool led_on = false;
int numero = 0;
double numeros[10][25] = {
    // 0
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1},
    // 1
    {0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.1, 0.0,
     0.1, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1},
    // 2
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.0, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1},
    // 3
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1},
    // 4
    {0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.1},
    // 5
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.0, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1},
    // 6
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.0, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1},
    // 7
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.1, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0},
    // 8
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1},
    // 9
    {0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.1,
     0.1, 0.1, 0.1, 0.1, 0.1,
     0.1, 0.0, 0.0, 0.0, 0.0,
     0.1, 0.1, 0.1, 0.1, 0.1}
};

// Função de callback que será chamada repetidamente pelo temporizador
// O tipo bool indica que a função deve retornar verdadeiro ou falso para continuar ou parar o temporizador.
bool repeating_timer_callback(struct repeating_timer *t) {
    led_on = !led_on;
    gpio_put(LED_PIN_RED,led_on);
    return true;
}


//rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events){
    absolute_time_t current_time = get_absolute_time();

    if (gpio == button_0)
    {
        if (absolute_time_diff_us(last_interrupt_time_0, current_time) < DEBOUNCE_TIME_MS * 1000)
        {
            return; // Ignora a interrupção se estiver dentro do tempo de debounce
        }
        last_interrupt_time_0 = current_time;

        printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
        printf("Botão A pressionado\n");
        if (numero >= 9) {
            numero = 0;
        } else {
            numero++;
        }
        printf("%d\n", numero);
    }
    else if (gpio == button_1)
    {
        if (absolute_time_diff_us(last_interrupt_time_1, current_time) < DEBOUNCE_TIME_MS * 1000)
        {
            return; // Ignora a interrupção se estiver dentro do tempo de debounce
        }
        last_interrupt_time_1 = current_time;

        printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
        printf("Botão B pressionado\n");
        if (numero <= 0) {
            numero = 9;
        } else {
            numero--;
        }
        printf("%d\n", numero);
    }
}
//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double r, double g, double b)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(desenho[24-i], 0,0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}


int main()
{
    PIO pio = pio0; 
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;

    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    //configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    //inicializar o botão de interrupção - GPIO5
    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN);
    gpio_pull_up(button_0);

    //inicializar o botão de interrupção - GPIO5
    gpio_init(button_1);
    gpio_set_dir(button_1, GPIO_IN);
    gpio_pull_up(button_1);

    //interrupção da gpio habilitada
    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, 1, & gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_1, GPIO_IRQ_EDGE_FALL, 1, & gpio_irq_handler);

    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);

    struct repeating_timer timer;

    add_repeating_timer_ms(100, repeating_timer_callback, NULL, &timer);

    while (true)
    {
        // Atualizar a matriz de LEDs
        desenho_pio(numeros[numero], valor_led, pio, sm, r, g, b);

        sleep_ms(100);
    }
}
