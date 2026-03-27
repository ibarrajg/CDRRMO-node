#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "message.h"
#include "lora_transmit.h"
#include "lora_receive.h"
#include "message_frame.h"

#ifdef __cplusplus
}
#endif


// =======================
// TX TASK (Sender)
// =======================
void console_task(void *arg)
{
    char msg[128] = {0};

    uint8_t frame[256];
    uint8_t msg_id = 0;

    uint8_t dst = 0x00;   // MAIN NODE (recommended)

    while (1) {

        get_message(msg, sizeof(msg));

        if (msg[0] != '\0') {

            msg_id++;

            size_t msg_len = strlen(msg);

            int frame_len = build_message_frame(
                frame,
                dst,
                msg_id,
                (uint8_t*)msg,
                msg_len
            );

            if (frame_len > 0) {
                lora_send((char*)frame);
                printf("LoRa frame sent\n");
            }
        }
    }
}


// =======================
// RX TASK (Receiver)
// =======================
void lora_rx_task(void *arg)
{
    uint8_t frame[256];

    uint8_t src, dst, msg_id, type;
    char payload[128];

    while (1) {

        int len = lora_receive((char*)frame, sizeof(frame));

        if (len > 0) {

            if (parse_message_frame(frame, len, &src, &dst, &msg_id, &type, payload)) {

                // 🔥 FINAL OUTPUT (clean)
                printf("%s\n", payload);

            } else {
                // Optional debug
                printf("Invalid frame\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


// =======================
// MAIN FUNCTION
// =======================
void app_main(void)
{
    // UART0 → PC (printf output)
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .flags = {
            .backup_before_sleep = 0,
        }
    };

    uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);

    // Initialize LoRa
    lora_init();

    // Create tasks
    xTaskCreate(console_task, "console_task", 4096, NULL, 5, NULL);
    xTaskCreate(lora_rx_task, "lora_rx_task", 4096, NULL, 5, NULL);
}