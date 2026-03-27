#ifndef LORA_RECEIVE_H
#define LORA_RECEIVE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void lora_rx_init(void);

// Read raw UART data
int lora_receive(char *buffer, int max_len);

// Parse received frame
bool parse_message_frame(
    uint8_t *frame,
    int frame_len,
    uint8_t *received_src,
    uint8_t *received_dst,
    uint8_t *received_msg_id,
    uint8_t *received_type,
    char *received_payload
);

#ifdef __cplusplus
}
#endif

#endif