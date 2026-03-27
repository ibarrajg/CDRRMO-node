#include "message_frame.h"
#include <string.h>

uint8_t calc_crc(uint8_t *buf, int len)
{
    uint8_t crc = 0;

    for (int i = 0; i < len; i++)
    {
        crc ^= buf[i];
    }

    return crc;
}


int build_message_frame(
    uint8_t *frame,
    uint8_t dst,
    uint8_t msg_id,
    uint8_t *data,
    uint8_t data_len
)
{
    if (data_len > MAX_DATA_LEN)
        return -1;

    int idx = 0;

    frame[idx++] = STX;
    frame[idx++] = SRC_ID;
    frame[idx++] = dst;
    frame[idx++] = msg_id;
    frame[idx++] = TYPE_MSG;
    frame[idx++] = data_len + 2;
    frame[idx++] = PAYLOAD_START;
    

    memcpy(&frame[idx], data, data_len);
    idx += data_len;

    frame[idx++] = PAYLOAD_END;

    uint8_t crc = calc_crc(frame, idx);
    frame[idx++] = crc;

    return idx;
}