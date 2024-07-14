#include "kernel.h"
#include "serial.h"

#define GET_OFFSET(n, m) [n * 8 + m]

void int2str(int num, char *str) {
  int i = 0;
  int isNegative = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  if (num < 0) {
    isNegative = 1;
    num = -num;
  }

  while (num != 0) {
    str[i++] = (num % 10) + '0';
    num = num / 10;
  }

  if (isNegative) {
    str[i++] = '-';
  }

  str[i] = '\0';

  int start = 0;
  int end = i - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}

const int LUT[64] = {8, 9, 6, 2, 5, 2, 5, 5, 5, 5, 5, 9, 3, 4, 8, 6,
                     5, 2, 1, 7, 4, 5, 9, 5, 8, 9, 6, 2, 5, 2, 5, 5,
                     5, 5, 5, 9, 3, 4, 8, 6, 5, 2, 1, 7, 4, 5, 9, 5,
                     8, 9, 6, 2, 5, 2, 5, 5, 5, 5, 5, 9, 3, 3, 4, 1};

void pm(int *matrix) {
  char str[32];

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int2str(matrix GET_OFFSET(i, j), str);
      uart_string_out(str);
      uart_char_out(' ');
    }
    uart_char_out('\n');
  }
}

void mm(int *a, int *b, int *c) {
  int i, j, k;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      for (k = 0; k < 8; k++) {
        c GET_OFFSET(i, j) += a GET_OFFSET(i, k) * b GET_OFFSET(k, j);
      }
    }
  }
}

static int m1[64];
static int m2[64];
static int mr[64];

int main() {
  uart_init();

  /* initialize matrices */
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      m1 GET_OFFSET(i, j) = LUT GET_OFFSET(i, j);
      m2 GET_OFFSET(i, j) = LUT GET_OFFSET(i, j);
    }
  }

  mm(m1, m2, mr);
  pm(mr);
}
