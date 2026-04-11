#ifndef MESSAGE_FRAME_H
#define MESSAGE_FRAME_H

#ifdef __cplusplus
extern "C" {
#endif

// Builds a normal message frame for outgoing user messages
void create_message_frame(const char *input, char *output);

// Builds an ACK frame addressed to the original sender
void create_ack_frame(int dst_id, char *output);

#ifdef __cplusplus
}
#endif

#endif
