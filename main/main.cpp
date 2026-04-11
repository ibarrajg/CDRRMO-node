#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "lora_uart.h"
#include "transmission.h"
#include "reception.h"
#include "message_frame.h"

extern "C" void app_main(void)
{
    printf("SYSTEM RUNNING\n");

    lora_uart_init();

    char payload[256];
    char ack_frame[300];
    int sender_id;
    int dst_id;

    while (1)
    {
        // Check if there is a user message to send
        if (transmission_process())
        {
            printf("Message sent\n");
        }

        // Check if there is an incoming valid frame
        if (reception_process(payload, &sender_id, &dst_id))
        {
            // Display only the extracted payload
            printf("Received message: %s\n", payload);

            // Create ACK frame addressed back to the sender
            create_ack_frame(sender_id, ack_frame);

            // Send ACK automatically
            lora_uart_send(ack_frame);

            printf("ACK sent: %s\n", ack_frame);
        }

        // Small delay to avoid tight looping
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
