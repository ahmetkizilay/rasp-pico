#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hello.pio.h"

int main() {
    // RP2040 has 2 PIOs with 4 state machines.
    // RP2350 (Pico 2) has 3 PIOs with 4 state machines.
    PIO pio = pio0;

    // Load the program into PIO's instructions memory
    uint offset = pio_add_program(pio, &hello_program);

    uint sm = pio_claim_unused_sm(pio, true);
    hello_program_init(pio, sm, offset, PICO_DEFAULT_LED_PIN);

    while (true) {
        pio_sm_put_blocking(pio, sm, 1);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0);
        sleep_ms(500);
    }
}