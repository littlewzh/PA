/*#include <stdint.h>
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
    if (sec == 1) {
      printf("%d second.\n", sec);
    } else {
      printf("%d seconds.\n", sec);
    }
    sec ++;
  }
}*/
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <NDL.h>
int main() {
   //struct timeval t1;
   //struct timezone t2;
   //t1->tv_usec=; 
   int sec = 1;
   uint32_t time=0;
   while (1) {
  //printf("hello\n");
    while( time/ 1000 < sec){time=NDL_GetTicks();}
    //gettimeofday(&t1,NULL);
    if (sec == 1) {
      printf("%d second.\n", sec);
    } else {
      printf("%d seconds.\n", sec);
    }
    sec ++;
  }
}
