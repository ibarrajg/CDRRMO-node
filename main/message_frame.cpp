#include <stdio.h>
#include <string.h>
#include "message_frame.h"
#include "crc.h"   // Shared CRC module used to generate frame checksum

// Fixed frame fields for the central node
static const char *SRC  = "00";
static const char *DST  = "00";
static const char *TYPE = "MSG";

// Build a normal message frame in the format:
// {SRC|DST|TYPE|LEN|<PAYLOAD>|CRC}
void create_message_frame(const char *input, char *output)
{
    // Get payload length for the LEN field
    int len = strlen(input);

    // Temporary buffer holds the frame body before CRC is added
    char temp[1024];

    // Build frame body without CRC first
    // Format: SRC|DST|TYPE|LEN|<PAYLOAD>
    sprintf(temp, "%s|%s|%s|%d|<%s>", SRC, DST, TYPE, len, input);

    // Compute CRC over the frame body only
    unsigned short crc = calculate_crc16(temp);

    // Build final frame with CRC appended and wrapped in braces
    sprintf(output, "{%s|%04X}", temp, crc);
}

// Build an ACK frame to reply to the device that sent the message
// Format: {SRC|DST|ACK|LEN|<ACK>|CRC}
void create_ack_frame(int dst_id, char *output)
{
    // Convert numeric destination ID into a 2-digit string
    // Example: 1 becomes "01"
    char dst[3];
    sprintf(dst, "%02d", dst_id);

    // Predefined payload for acknowledgement
    const char *ack_payload = "Message received";

    // Temporary buffer holds the frame body before CRC is added
    char temp[1024];

    // Build ACK frame body without CRC first
    sprintf(temp, "%s|%s|%s|%d|<%s>",
            SRC, dst, "ACK", (int)strlen(ack_payload), ack_payload);

    // Compute CRC over the ACK frame body
    unsigned short crc = calculate_crc16(temp);

    // Build final ACK frame with CRC and frame markers
    sprintf(output, "{%s|%04X}", temp, crc);
}
