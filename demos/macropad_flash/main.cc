#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    
    while(1) {
        printf("Flash: %d\n", PICO_FLASH_SIZE_BYTES);
        sleep_ms(1000);
    }
}
