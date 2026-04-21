#ifndef CENTRAL_LOGIC_H
#define CENTRAL_LOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

#define ACTION_NONE 0
#define ACTION_SHOW_AND_ACK 1
#define ACTION_ACK_ONLY 2

int handle_central_message(const char *type, int sender_id, const char *raw_frame);

#ifdef __cplusplus
}
#endif

#endif
