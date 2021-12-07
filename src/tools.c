void intToString(int val, char* buf, int maxsize) {
  int i = maxsize - 2;

  while (val != 0) { 
    buf[i--] = (val % 10) + '0';
    val /= 10;
  }
}