/* API wrapper for serial drivers */
#pragma once

int uart_init(void);
void uart_char_out(unsigned char c);
int uart_char_in(unsigned char *c);

void uart_string_out(char *str) {
  if (str == 0x0) {
    return;
  }

  while (*str) {
    uart_char_out(*str);
    str++;
  }
}
