#ifndef FRAME_PARSER_H
#define FRAME_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

int parse_message_frame(const char *frame, char *payload_out);

#ifdef __cplusplus
}
#endif

#endif