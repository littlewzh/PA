#include <unistd.h>
#include <stdio.h>
//extern int _write(int fd, void *buf, size_t count);
int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  uint32_t p=&i;
  printf("0x%08x\n",p);
  //volatile int j = 0;
 // while (1) {
   // j ++;
   // if (j == 1000000) {
      printf("Hello World from Navy-apps for the %dth time!\n", i ++);
   //   j = 0;
  //  }
 //}
  return 0;
}
