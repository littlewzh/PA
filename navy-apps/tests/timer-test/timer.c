#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
int main() {
   struct timeval t1;
   //struct timezone t2;
   //t1->tv_usec=; 
   int sec = 1;
   while (1) {
  //printf("hello\n");
    while( t1.tv_usec/ 1000000 < sec){gettimeofday(&t1,NULL);}
    gettimeofday(&t1,NULL);
    if (sec == 1) {
      printf("%d second.\n", sec);
    } else {
      printf("%d seconds.\n", sec);
    }
    sec ++;
  }
}
