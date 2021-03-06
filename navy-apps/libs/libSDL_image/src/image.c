#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc
#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}
static char buf[400*300*4]={0};

SDL_Surface* IMG_Load(const char *filename) {
  //printf("reach img_load\n");
  int fd=open(filename,0,0);
  //char *buf;
  uint32_t size=300*400*4;
  //buf= malloc(size);
  memset(buf,0,sizeof(buf));
  size=read(fd,(void *)buf,size);
  return STBIMG_LoadFromMemory(buf,size);
  //return NULL;
}
int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
