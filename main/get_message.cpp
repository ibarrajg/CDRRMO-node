#include <stdio.h>
#include <string.h>
#include "get_message.h"

#define MAX_MSG_LEN 256

int get_user_message(char *out_msg)
{
    if (fgets(out_msg, MAX_MSG_LEN, stdin) == NULL) {
        return 0;
    }

    out_msg[strcspn(out_msg, "\n")] = '\0';

    if (out_msg[0] == '\0') {
        return 0;
    }

    return 1;
}
