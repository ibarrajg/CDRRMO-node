#include "message.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int get_message(char *buffer, int max_len)
{
    int len = uart_read_bytes(
        UART_NUM_0,
        (uint8_t *)buffer,
        max_len - 1,
        pdMS_TO_TICKS(1000)
    );

    if (len > 0) {
        buffer[len] = '\0';

        for (int i = 0; i < len; i++) {
            if (buffer[i] == '\r' || buffer[i] == '\n') {
                buffer[i] = '\0';
                break;
            }
        }
         return 1;  //return value
    } else {
        buffer[0] = '\0';
        return 0;  //return value
    }
}
