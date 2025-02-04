#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_RED_PIN 13    // Pino do LED vermelho
#define LED_YELLOW_PIN 12 // Pino do LED amarelo
#define LED_GREEN_PIN 11  // Pino do LED verde

// Mapeamento dos LEDs nos pinos, de acordo com o estado do semáforo
int semaforo[3][3] = {
    {1, 0, 0},  // Posição 0 Contador = LED vermelho aceso
    {0, 1, 0},  // Posição 1 Contador = LED amarelo aceso
    {0, 0, 1}   // Posição 2 Contador = LED verde aceso
};

// Vetor com os pinos dos LEDs, de acordo com a ordem no semáforo
int leds_pins[3] = {LED_RED_PIN, LED_YELLOW_PIN, LED_GREEN_PIN};

volatile int contador = 0;  // Contador para alternar os estados

bool semaforo_timer_callback(repeating_timer_t *rt) {
    // Laço único para controlar o estado de todos os LEDs
    for (int i = 0; i < 3; i++) {
        // Aciona os LEDs conforme o estado atual
        gpio_put(leds_pins[i], semaforo[contador][i]);  // Aplica o mapeamento
    }

    // Incrementa o contador para o próximo estado
    contador = (contador + 1) % 3;  // Vai de 0 a 2 e reinicia para 0

    return true;  // Retorna true para continuar repetindo o timer
}

int main() {
    
    stdio_init_all();

     // Inicializa e configura os pinos dos segmentos como saída
    for (int i = 0; i < 3; i++) {
        gpio_init(leds_pins[i]);
        gpio_set_dir(leds_pins[i], GPIO_OUT);
    }

    // Inicializa o timer de repetição a cada 1 segundo (1000 ms)
    repeating_timer_t timer;
    add_repeating_timer_ms(3000, semaforo_timer_callback, NULL, &timer);

    // Deixa o programa rodando para o timer ser acionado
    while (true) {
        printf("Execution Time: 1s\n");
        sleep_ms(1000);  // Espera 1 segundo
    }

    return 0;
}

