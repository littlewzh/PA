#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
static struct timeval boot_time;
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval t;
  gettimeofday(&t,NULL);
  uint32_t ticks = (t.tv_sec - boot_time.tv_sec)*1000 + (t.tv_usec - boot_time.tv_usec)/1000;
  //printf("%d\n",t.tv_usec/1000);
  return ticks;
}

int NDL_PollEvent(char *buf, int len) {
  int fd=open("/dev/events",0,0);
  //printf("reach poll\n");
  int res=read(fd,(void *)buf,len);
  //printf("%d\n",res);
  close(fd); 
  return res;
  //if(f==0) return 0;
  //return 1;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  if(*w==0||*h==0){
   *w=400;
   *h=300;
  }
  printf("%d %d\n",*w,*h);
  //TODO()
}
/*static uint32_t  canvas[300][400];
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
   int fd=open("/dev/fb",0,0);
   memset(canvas,0,sizeof(canvas));
   for (int i = 0; i < h; i ++) {
     for (int j = 0; j < w; j ++) {
        canvas[i+y][j+x]=pixels[i*w+j];
        //canvas[(i + y) * 400 + (j + x)] = pixels[i * w + j];
      }
    }
    //printf("reach here\n");
    write(fd,(void *)canvas,400*300*4);
    close(fd);
}*/
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h){
  int fd=open("/dev/fb",0,0);
  int offset=y*400+x;
  for(int i=0;i<h;i++){
    lseek(fd,(offset+i*400)*4,SEEK_SET);
    write(fd,pixels+i*w,w);
  }
  close(fd);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  gettimeofday(&boot_time, NULL);
  return 0;
}

void NDL_Quit() {
}
