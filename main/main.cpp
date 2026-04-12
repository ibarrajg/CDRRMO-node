#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "lora_uart.h"
#include "transmission.h"
#include "reception.h"
#include "message_frame.h"
#include "loop_guard.h"

extern "C" void app_main(void)
{
    lora_uart_init();

    char payload[256];
    char raw[300];
    char ack_frame[300];
    char type[4];
    int sender_id;
    int dst_id;

    while (1)
    {
        // Handle outgoing message from the UI
        transmission_process();

        // Handle incoming valid frame
        if (reception_process(payload, raw, &sender_id, &dst_id, type))
        {
            // Only normal messages should be displayed and acknowledged
            if (strcmp(type, "MSG") == 0)
            {
                // Ignore the central node's own message coming back
                if (sender_id != 0)
                {
                    // Ignore duplicate retransmissions of the same message
                    if (loop_guard(raw))
                    {
                        // Send received message to the UI
                        printf("%s\n", payload);

                        // Create ACK frame addressed back to the sender
                        create_ack_frame(sender_id, ack_frame);

                        // Send ACK automatically through LoRa
                        lora_uart_send(ack_frame);

                        // Tell the UI that an ACK was sent
                        //printf("ACK|Message received\n");
                    }
                }
            }

            // Ignore ACK frames received by the central node
            else if (strcmp(type, "ACK") == 0)
            {
                // Do nothing
            }
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
