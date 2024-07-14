#include <stdint.h> // fixed-with integers

/* Basic SiFive UART driver for the FE300 platform */

#define DRIVER_STRING uart_sifive

// TODO(pkoscik): read these values out form the yaml file
#define BASE 0x10013000

/* Register offsets are stored as a size of variable
 * is this a good approach? idk, but a bunch of other
 * projects do this, so it might be fine?
 * TODO(pkoscik): rethink this approach
 */
struct sifive_uart_regs {
  uint32_t tx;
  uint32_t rx;
  uint32_t txctrl;
  uint32_t rxctrl;
  uint32_t ie;
  uint32_t ip;
  uint32_t div;
};

void uart_init() {
  volatile struct sifive_uart_regs *uart = (struct sifive_uart_regs *)BASE;

  /* enable TX/RX lines */
  uart->txctrl = 1;
  uart->rxctrl = 1;

  /* guess divider rate
   * TODO(pkoscik): fixing this will require introducing a concept of
   * a core clock - a lot of work - ignoring this for now
   */
  uart->div = 1736;

  /* disable interrupts - no support for that yet */
  uart->ie = 0;
}

void uart_char_out(unsigned char c) {
  volatile struct sifive_uart_regs *uart = (struct sifive_uart_regs *)BASE;
  uart->tx = (int)c;
}

int uart_char_in(unsigned char *c) {
  volatile struct sifive_uart_regs *uart = (struct sifive_uart_regs *)BASE;
  uint32_t rxdata = uart->rx;

  /* check for rxdata empty flag */
  if (rxdata & 1 << 31) {
    return -1;
  }

  /* return single byte of rxdata */
  *c = (unsigned char)(rxdata & 0xFF);
  return 0;
}
