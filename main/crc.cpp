#include "crc.h"

unsigned short calculate_crc16(const char *data)
{
    unsigned short crc = 0xFFFF;

    while (*data) {
        crc ^= (unsigned char)(*data);

        for (int i = 0; i < 8; i++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }

        data++;
    }

    return crc;
}