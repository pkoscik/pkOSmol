void busy_wait() {
  /* wait a bunch of cycles */
  long long cycles = 0x10000;
  while (cycles) {
    cycles--;
  }
}
