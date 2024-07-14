#include "kernel.h"

extern int main(void);

void c_kernel_init() {
  /* initilize kernel structures and data */
  main();

  /* enter wfi */
  cpu_idle();
}
