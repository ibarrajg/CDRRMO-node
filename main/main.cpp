#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "lora_uart.h"
#include "transmission.h"
#include "reception.h"

extern "C" void app_main(void)
{
    printf("SYSTEM RUNNING\n");

    lora_uart_init();

    char payload[256];
    char raw[300];
    static char last_raw[300] = "";

    while (1)
    {
        // 🔹 TRANSMIT (user input)
        if (transmission_process())
        {
            printf("Message sent\n\n\n");
        }

        // 🔹 RECEIVE + REPEAT
        if (reception_process(payload, raw))
        {
            printf("Payload: %s\n", payload);

            // 🔥 Prevent infinite loop
            if (strcmp(raw, last_raw) != 0)
            {
                strcpy(last_raw, raw);

                // small delay before retransmit
                vTaskDelay(100 / portTICK_PERIOD_MS);

                // 🔁 retransmit exact raw frame
                lora_uart_send(raw);

                printf("Retransmitting: %s\n\n\n", raw);
            }
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}