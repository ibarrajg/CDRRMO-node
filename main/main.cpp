#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "lora_uart.h"
#include "transmission.h"
#include "reception.h"
#include "message_frame.h"
#include "central_logic.h"

static void transmission_task(void *pvParameters)
{
    while (1)
    {
        // Handle outgoing message from the UI
        transmission_process();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void reception_task(void *pvParameters)
{
    char payload[300];
    char raw[1024];
    char ack_frame[1024];
    char type[4];
    int sender_id;
    int dst_id;
    int action;

    while (1)
    {
        // Handle incoming valid frame
        if (reception_process(payload, raw, &sender_id, &dst_id, type))
        {
            action = handle_central_message(type, sender_id, raw);

            if (action == ACTION_SHOW_AND_ACK)
            {
                // Send received message to the UI
                printf("%02d|%s\n", sender_id, payload);

                // Create ACK frame addressed back to the sender
                create_ack_frame(sender_id, ack_frame);

                // Send ACK automatically through LoRa
                lora_uart_send(ack_frame);
            }
            else if (action == ACTION_ACK_ONLY)
            {
                create_ack_frame(sender_id, ack_frame);
                lora_uart_send(ack_frame);
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    lora_uart_init();

    xTaskCreatePinnedToCore(
        transmission_task,
        "tx_task",
        4096,
        NULL,
        5,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        reception_task,
        "rx_task",
        4096,
        NULL,
        5,
        NULL,
        1
    );
}
