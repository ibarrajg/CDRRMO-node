#include <string.h>
#include "central_logic.h"
#include "loop_guard.h"

#define CENTRAL_ID 0

int handle_central_message(const char *type, int sender_id, const char *raw_frame)
{
    // Ignore ACK frames received by the central node.
    if (strcmp(type, "ACK") == 0) {
        return ACTION_NONE;
    }

    // Only normal messages are relevant to the central node.
    if (strcmp(type, "MSG") != 0) {
        return ACTION_NONE;
    }

    // Ignore the central node's own message when it comes back through retransmission.
    if (sender_id == CENTRAL_ID) {
        return ACTION_NONE;
    }

    // Ignore duplicate retransmissions of the same message.
    if (!loop_guard(raw_frame)) {
        return ACTION_ACK_ONLY; // still ACK to prevent sender retries, but don't show again
    }

    return ACTION_SHOW_AND_ACK;
}
