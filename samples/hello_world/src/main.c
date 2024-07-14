#include "kernel.h"
#include "serial.h"

int main() {
  uart_init();
  uart_string_out("Hello World - pkOSmol\n");

  unsigned char cin = 0;
  for (;;) {
    if (!uart_char_in(&cin)) {
      uart_char_out(cin);
    }
    busy_wait();
  }
}
