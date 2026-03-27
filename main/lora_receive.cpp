#include "lora_receive.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "message_frame.h"
#include <string.h>

#define LORA_UART UART_NUM_1

void lora_rx_init(void)
{
    // Optional setup
}

int lora_receive(char *buffer, int max_len)
{
    int len = uart_read_bytes(
        LORA_UART,
        (uint8_t *)buffer,
        max_len - 1,
        pdMS_TO_TICKS(10)
    );

    if (len > 0) {
        buffer[len] = '\0';
        return len;
    }

    return 0;
}


bool parse_message_frame(
    uint8_t *frame,
    int frame_len,
    uint8_t *received_src,
    uint8_t *received_dst,
    uint8_t *received_msg_id,
    uint8_t *received_type,
    char *received_payload
)
{
    if (frame_len < 7)
        return false;

    uint8_t received_stx, received_len, received_crc;

    int idx = 0;

    // STX check
    received_stx = frame[idx++];
    if (received_stx != STX)
        return false;

    // Extract fields
    *received_src       = frame[idx++];
    *received_dst       = frame[idx++];
    *received_msg_id    = frame[idx++];
    *received_type      = frame[idx++];
    received_len        = frame[idx++];

    // Validate length
    if (received_len < 2)
        return false;

    int payload_len = received_len - 2;

    if (payload_len > MAX_DATA_LEN)
        return false;

    // Check start marker
    if (frame[idx] != PAYLOAD_START)
        return false;

    idx++;

    // Bounds check
    if (idx + payload_len >= frame_len)
        return false;

    // Copy payload
    memcpy(received_payload, &frame[idx], payload_len);
    received_payload[payload_len] = '\0';

    idx += payload_len;

    // Check end marker
    if (frame[idx] != PAYLOAD_END)
        return false;

    idx++;

    // Extract CRC (not validated yet)
    received_crc = frame[idx++];

    return true;
}