#ifndef MESSAGE_FRAME_H
#define MESSAGE_FRAME_H

#include <stdint.h>

#define STX           0xAA

// Node IDs
#define NODE_A_ID     0x01
#define NODE_B_ID     0x02
#define NODE_C_ID     0x03
#define MAIN_NODE_ID  0x00

#define SRC_ID        NODE_A_ID   // change per device

#define TYPE_MSG      0x01 //

#define MAX_DATA_LEN  64

// Payload markers
#define PAYLOAD_START '<'
#define PAYLOAD_END   '>'

#ifdef __cplusplus
extern "C" {
#endif

uint8_t calc_crc(uint8_t *buf, int len);

int build_message_frame(
    uint8_t *frame,
    uint8_t dst,
    uint8_t msg_id,
    uint8_t *data,
    uint8_t data_len
);

#ifdef __cplusplus
}
#endif

#endif