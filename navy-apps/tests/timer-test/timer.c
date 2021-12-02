#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
int main() {
   struct timeval * t;
   t->tv_usec; 
   int sec = 1;
   while (1) {
  //printf("hello\n");
    while( t->tv_usec/ 1000000 < sec) ;
    gettimeofday(t,NULL);
    if (sec == 1) {
      printf("%d second).\n", sec);
    } else {
      printf("%d seconds).\n", sec);
    }
    sec ++;
  }
}
