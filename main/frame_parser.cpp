#include <stdio.h>
#include <string.h>
#include "frame_parser.h"
#include "crc.h"   // use shared CRC module

int parse_message_frame(const char *frame, char *payload_out)
{
    int len = strlen(frame);

    // ===== 1. Check frame markers =====
    if (len < 5 || frame[0] != '{' || frame[len - 1] != '}')
        return 0;

    // ===== 2. Remove { } =====
    char inner[256];
    strncpy(inner, frame + 1, len - 2);
    inner[len - 2] = '\0';

    // Example now:
    // 01|02|MSG|5|<hello>|3F2A

    // ===== 3. Extract CRC =====
    char *last_pipe = strrchr(inner, '|');
    if (!last_pipe) return 0;

    unsigned short received_crc;
    sscanf(last_pipe + 1, "%hx", &received_crc);

    // Remove CRC part
    *last_pipe = '\0';

    // Now:
    // 01|02|MSG|5|<hello>

    // ===== 4. Compute CRC =====
    unsigned short computed_crc = calculate_crc16(inner);

    if (computed_crc != received_crc)
        return 0;  // CRC mismatch

    // ===== 5. Extract payload using < > =====
    char *start = strchr(inner, '<');
    char *end   = strchr(inner, '>');

    if (!start || !end || end <= start)
        return 0;

    int payload_len = end - start - 1;

    strncpy(payload_out, start + 1, payload_len);
    payload_out[payload_len] = '\0';

    return 1; // success
}