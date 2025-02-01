#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
// arquivo .pio
#include "main.pio.h"

// número de LEDs
#define NUM_PIXELS 25

#define LED_PIN_RED 13

// pino de saída do pio
#define OUT_PIN 7

#define DEBOUNCE_TIME_MS 300 // Tempo de debounce em milissegundos

// Variáveis para armazenar o tempo da última interrupção
absolute_time_t last_interrupt_time_0 = {0};
absolute_time_t last_interrupt_time_1 = {0};

// botão de interupção
const uint PIN_BUTTON_A = 5;
const uint PIN_BUTTON_B = 6;
// estado do led
bool led_on = false;
// número a ser exibido
int numero = 0;
// lista de números
double numeros[10][25] = {
    // 0
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0},
    // 1
    {0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.1, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0},
    // 2
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.0, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0},
    // 3
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0},
    // 4
    {0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.1, 0.0},
    // 5
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.0, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0},
    // 6
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.0, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0},
    // 7
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.1, 0.0},
    // 8
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0},
    // 9
    {0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0,
     0.0, 0.1, 0.0, 0.0, 0.0,
     0.0, 0.1, 0.1, 0.1, 0.0}};

// Função de callback que será chamada repetidamente pelo temporizador
bool repeating_timer_callback(struct repeating_timer *t)
{
    // Inverte o estado do LED
    led_on = !led_on;
    gpio_put(LED_PIN_RED, led_on);
    // Retorna verdadeiro para continuar chamando a função
    return true;
}

// rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obter o tempo atual para o debounce
    absolute_time_t current_time = get_absolute_time();

    if (gpio == PIN_BUTTON_A)
    {
        // Verifica se a interrupção ocorreu dentro do tempo de debounce
        if (absolute_time_diff_us(last_interrupt_time_0, current_time) < DEBOUNCE_TIME_MS * 1000)
        {
            return; // Ignora a interrupção se estiver dentro do tempo de debounce
        }
        // Atualiza o tempo da última interrupção
        last_interrupt_time_0 = current_time;

        printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
        printf("Botão A pressionado\n");
        
        // Incrementa o número se número for menor que 9, caso contrário, zerar
        if (numero < 9)
        {
            numero++;
        }
        else
        {
            numero = 0;
        }
        printf("%d\n", numero);
    }
    else if (gpio == PIN_BUTTON_B)
    {
        // Verifica se a interrupção ocorreu dentro do tempo de debounce
        if (absolute_time_diff_us(last_interrupt_time_1, current_time) < DEBOUNCE_TIME_MS * 1000)
        {
            return; // Ignora a interrupção se estiver dentro do tempo de debounce
        }
        // Atualiza o tempo da última interrupção
        last_interrupt_time_1 = current_time;

        printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
        printf("Botão B pressionado\n");
        // Decrementa o número se número for maior que 0, caso contrário, definir para 9
        if (numero > 0)
        {
            numero--;
        }
        else
        {
            numero = 9;
        }
        printf("%d\n", numero);
    }
}
// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double r, double g, double b)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        // Define a intensidade de cada cor, nesse caso, apenas a cor vermelha
        valor_led = matrix_rgb(desenho[24 - i], 0, 0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

int main()
{
    PIO pio = pio0;
    bool ok;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    // coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    // configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // inicializar o botão de interrupção A - GPIO5
    gpio_init(PIN_BUTTON_A);
    gpio_set_dir(PIN_BUTTON_A, GPIO_IN);
    gpio_pull_up(PIN_BUTTON_A);

    // inicializar o botão de interrupção B - GPIO6
    gpio_init(PIN_BUTTON_B);
    gpio_set_dir(PIN_BUTTON_B, GPIO_IN);
    gpio_pull_up(PIN_BUTTON_B);

    // interrupção da gpio habilitada para o botão A e B
    gpio_set_irq_enabled_with_callback(PIN_BUTTON_A, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(PIN_BUTTON_B, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);

    // Inicializa o LED vermelho
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);

    // Inicializa o temporizador de repetição para piscar o LED
    struct repeating_timer timer;
    add_repeating_timer_ms(100, repeating_timer_callback, NULL, &timer);// 100 ms pra poder chamar a função 10 vezes por segundo

    while (true)
    {
        // Atualizar a matriz de LEDs com o número atual de acordo com a posição do indice na lista de números
        desenho_pio(numeros[numero], valor_led, pio, sm, r, g, b);
        // Aguarda 100 ms para a CPU não ficar 100% ocupada
        sleep_ms(100);
    }
}
