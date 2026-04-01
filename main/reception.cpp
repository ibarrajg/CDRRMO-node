#include <stdio.h>
#include <string.h>

#include "reception.h"
#include "lora_uart.h"
#include "frame_parser.h"


int reception_process(char *payload_out)
{
    char rx_buffer[300];

    // Step 1: Receive data from UART LoRa
    int len = lora_uart_receive(rx_buffer, sizeof(rx_buffer));

    if (len > 0) {

        printf("Raw Received: %s\n", rx_buffer);

        // Step 2: Parse frame
        if (parse_message_frame(rx_buffer, payload_out)) {

            printf("Valid frame received. Payload: %s\n", payload_out);
            return 1;
        } else {
            printf("Invalid frame or CRC error\n");
        }
    }

    return 0; // no valid payload
}