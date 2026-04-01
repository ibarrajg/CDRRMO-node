#include <stdio.h>
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

    while (1)
    {
        // Only print if something is sent
        if (transmission_process())
        {
            printf("Message sent\n");
        }

        // Only print if something is received
        if (reception_process(payload))
        {
            printf("Payload: %s\n", payload);
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}