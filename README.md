# Projeto de Controle de Matriz de LEDs com Raspberry Pi Pico

Este projeto utiliza um Raspberry Pi Pico para controlar uma matriz de LEDs 5x5 e exibir números de 0 a 9, de acordo com cliques em botões. O projeto também inclui a funcionalidade de debounce para botões de interrupção e faz o um LED RGB piscar 5 vezes por segundo.

## Requisitos

- Raspberry Pi Pico
- Matriz de LEDs 5x5
- LED RGB
- Botões de interrupção
- SDK do Raspberry Pi Pico

## Funcionalidades

- Exibição de números de 0 a 9 em uma matriz de LEDs 5x5.
- Debounce para botões de interrupção que modficam os números na matriz.
- LED RGB piscando 5 vezes por segundo.

## Configuração do Hardware

- Conecte a matriz de LEDs ao pino de saída definido no código (`OUT_PIN`).
- Conecte os botões de interrupção aos pinos definidos no código (`PIN_BUTTON_A` e `PIN_BUTTON_B`).
- Conecte um LED ao pino (`LED_PIN_RED`).

## Compilação e Execução

1. Clone o repositório do projeto.
2. Configure o ambiente de desenvolvimento do Raspberry Pi Pico.
3. Compile o código usando o CMake.
4. Carregue o binário no Raspberry Pi Pico.

## Código Principal

O código principal está localizado no arquivo `main.c`. Ele inclui a configuração do clock do sistema, inicialização dos pinos GPIO, configuração das interrupções e controle da matriz de LEDs.

### Função `main`

A função `main` configura o clock do sistema para 128 MHz, inicializa os pinos GPIO para os botões de interrupção e o LED, configura a PIO para controlar a matriz de LEDs e inicializa um temporizador de repetição para piscar o LED no pino 13. O loop principal atualiza a matriz de LEDs com o número atual.

### Funções de Handler das Interrupções

A função `gpio_irq_handler` é chamada quando ocorre uma interrupção nos pinos dos botões (`PIN_BUTTON_A` e `PIN_BUTTON_B`). Ela verifica se a interrupção ocorreu dentro do tempo de debounce e, se não, atualiza o tempo da última interrupção e incrementa ou decrementa o número exibido na matriz de LEDs.

### Função de Repeating Timer

A função `repeating_timer_callback` é chamada repetidamente pelo temporizador. Ela inverte o estado do LED no pino 13 (`LED_PIN_RED`), fazendo com que ele pisque. A função retorna `true` para continuar sendo chamada pelo temporizador.

##Link para o vídeo de explicação:
https://youtu.be/3s7VlAPRx0Y
