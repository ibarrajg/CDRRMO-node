#include <stdio.h>
#include <string.h>
#include "get_message.h"

#define MAX_MSG_LEN 256

int get_user_message(char *out_msg)
{
    // Read one line from the USB serial console
    if (fgets(out_msg, MAX_MSG_LEN, stdin) == NULL) {
        return 0;
    }

    // Remove newline
    out_msg[strcspn(out_msg, "\r\n")] = '\0';

    // Ignore empty input
    if (out_msg[0] == '\0') {
        return 0;
    }

    return 1;
}