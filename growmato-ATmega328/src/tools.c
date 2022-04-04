#include <stdbool.h>

void intToString(int val, char* buf, int maxsize) {
  int i = 0;
  bool isNeg = val < 0;
  unsigned int n1 = isNeg ? -val : val;

  while (n1 != 0) {
    buf[i++] = (n1 % 10) + '0';
    n1 = n1 / 10;
  }

  if (isNeg) buf[i++] = '-';

  buf[i] = '\0';

  for(int t = 0; t < i / 2; t++) {
    buf[t] ^= buf[i-t-1];
    buf[i-t-1] ^= buf[t];
    buf[t] ^= buf[i-t-1];
  }

  if (val == 0) {
    buf[0] = '0';
    buf[1] = '\0';
  }
}
