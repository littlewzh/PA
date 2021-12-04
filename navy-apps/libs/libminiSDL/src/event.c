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
  
  if(NDL_PollEvent(buf, sizeof(buf))){
    //sscanf(buf,"%d",(int *)&num);
  //int f=*num;
  //printf("%d",f);
  if(buf[1]=='d'){
    event->type=SDL_KEYDOWN;
    event->key.keysym.sym=buf[3]-'0';
    printf("%d\n",event->key.keysym.sym);
    printf("%d\n",event->type);
  }
  else if(buf[1]=='u'){
    event->type=SDL_KEYUP;
    printf("%d\n",event->type);
  }
  else {event->type=2;}
  return 1;
  }
  return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
