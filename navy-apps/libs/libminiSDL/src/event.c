#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};
static char buf[64];
static int num;
int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  
  NDL_PollEvent(buf, 64);
  sscanf(buf,"%d",(int *)&num);
  //int f=*num;
  //printf("%d",f);
  if(buf[1]=='d'){
    event->type=SDL_KEYDOWN;
    event->key.keysym.sym=num;
    printf("%d\n",event->key.keysym.sym);
  }
  else if(buf[1]=='u'){
    event->type=SDL_KEYUP;
    
  }
  else {event->type=2;}
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
