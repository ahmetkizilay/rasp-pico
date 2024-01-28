#include <stdio.h>
#include "pico/stdlib.h"

#define MACROPAD_LED 13

int main() {
    stdio_init_all();

    const uint LED_PIN = MACROPAD_LED;
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while(1) {
        gpio_put(LED_PIN, 1);
        printf("LED ON!\n");
        sleep_ms(500);

        gpio_put(LED_PIN, 0);
        printf("LED OFF!\n");
        sleep_ms(500);
    }
}