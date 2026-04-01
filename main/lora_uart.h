#ifndef LORA_UART_H
#define LORA_UART_H

#ifdef __cplusplus
extern "C" {
#endif

void lora_uart_init(void);
void lora_uart_send(const char *data);
int  lora_uart_receive(char *buffer, int max_len);

#ifdef __cplusplus
}
#endif

#endif