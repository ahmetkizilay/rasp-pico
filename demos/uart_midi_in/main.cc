#include <cmath>
#include <stdio.h>

#include "hardware/timer.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart1
#define BAUD_RATE 31250

#define UART_RX_PIN 21

/*

Listen for MIDI messages on UART1 RX pin (pin 21) and print the messages to the console.

MIDI Messages

Channel Voice Messages
8X (b1000XXXX) - Note Off (3 bytes)
9X (b1001XXXX) - Note on (3 bytes) (Third byte is velocity. It can be set to 0 to turn off the note)
AX (b1010XXXX) - Polyphonic Aftertouch (3 bytes)
BX (b1011XXXX) - Control Change (3 bytes)
CX (b1100XXXX) - Program Change (2 bytes)
DX (b1101XXXX) - Channel Aftertouch (2 bytes)
EX (b1110XXXX) - Pitch Wheel (3 bytes)


Control Change Messages (First byte BX, Second byte CC, Third byte Value)

78 (b01111000), 00 (b00000000) - All Sound Off

System Real-Time Messages
F8 (b11111000) - Timing Clock (1 byte)
FA (b11111010) - Start (1 byte)
FB (b11111011) - Continue (1 byte)
FC (b11111100) - Stop (1 byte)

*/
int main() {
  stdio_init_all();

  uint baudrate = uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  char message[3] = {0};
  bool new_message = false;
  char message_index = 0;
  uint32_t time_received = 0;
  bool print_timing_clock = false;
  int message_length = 3;

  const size_t pulse_buffer_size = 12;
  int32_t last_pulse = 0;
  int pulse_index = 0;

  while (true) {
    char c = uart_getc(UART_ID);
    if (c > 127) {
      new_message = true;
      message_index = 0;
      time_received = time_us_32();
      
      // detect tempo.
      if (c == 0XF8) {
        if (pulse_index == 0) {
          // calculate difference between the first and last pulse in the buffer.
          // 24 pulses per quarter note.
          double tempo = 60000000.0 / (time_received - last_pulse) / (24 / pulse_buffer_size);
          tempo = round(tempo * 2) / 2;
          printf("Tempo: %.1f BPM\n", tempo); 
          last_pulse = time_received;
        }
        pulse_index = (pulse_index + 1) % pulse_buffer_size;
      }

      // Determine message length
      switch (c >> 4) {
        case 0xC:
        case 0xD:
          message_length = 2;
          break;
        case 0XF:
          // let's just assume so for now.
          message_length = 1;
          break;
        case 0x8:
        case 0x9:
        case 0xE:
        default:
          message_length = 3;
          break;
      }
    }

    // collect messages. MIDI message can be maximum of 3 bytes.
    if (new_message && message_index < 3) {
      message[message_index++] = c;
    } else {
      printf("Unexpected message %02hhx, %zu\n", (unsigned char)c, time_received);
      
      new_message = false;
      message_index = 0;

      continue;
    }

    // print the message if it is complete
    if (message_index == message_length) {
      if (message_length == 1) {
        switch (c) {
          case 0xFA:
            printf("Sequence Start, %zu\n", time_received);
            break;
          case 0xFB:
            printf("Continue, %zu\n", time_received);
            break;
          case 0xFC:
            printf("Stop, %zu\n", time_received);
            break;
          case 0XF8:
            if (print_timing_clock) {
              printf("Time Clock, %zu\n", time_received);
            }
            break;
          default:
            break;
        }
      } else if (message_length == 2) {
        switch (message[0] >> 4) {
          case 0xC:
            printf("Program Change ~ ");
            break;
          case 0xD:
            printf("Channel Aftertouch ~ ");
            break;
          default:
            break;
        }
        printf("%02hhx %02hhx, %zu\n", (unsigned char)message[0], (unsigned char)message[1],
               time_received);
      } else if (message_length == 3) {
        switch (message[0] >> 4) {
          case 0x8:
            printf("Note Off ~ ");
            break;
          case 0x9:
            printf("Note On ~ ");
            break;
          case 0xA:
            printf("Polyphonic Aftertouch ~ ");
            break;
          case 0xB:
            printf("Control Change ~ ");
            break;
          case 0xE:
            printf("Pitch Wheel ~ ");
            break;
          default:
            break;
        }
        printf("%02hhx %02hhx %02hhx, %zu\n", (unsigned char)message[0], (unsigned char)message[1],
               (unsigned char)message[2], time_received);
      }

      // Reset the message state
      new_message = false;
      message_index = 0;
    }
  }
}
