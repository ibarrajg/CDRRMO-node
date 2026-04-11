#ifndef RECEPTION_H
#define RECEPTION_H

#ifdef __cplusplus
extern "C" {
#endif

int reception_process(char *payload_out, int *sender_id, int *dst_id);

#ifdef __cplusplus
}
#endif

#endif