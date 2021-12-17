#include <NDL.h>
#include <SDL.h>
#include <string.h>
#define keyname(k) #k,
#include <assert.h>
static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};
//static char buf[64];
//static int num;
static uint8_t keystate[83]={0};
int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  //printf("sdl_poll\n");
  char buf[64];
  int len=NDL_PollEvent(buf, sizeof(buf));
  //printf("2\n");
  if(len!=0){
    char key[32];
    int num;
    printf("%s\n",buf);
    num=0;
    int i;
    i=3;
    int j=0;
    while(buf[i]!='\n'){
      key[j]=buf[i];
      i++;
      j++;
    }
    key[j]='\0';
    for(int j=0;j<83;j++){
      if(strcmp(key,keyname[j])==0) {num=j;break;}
    }
  if(buf[1]=='d'){
    ev->type=SDL_KEYDOWN;
    ev->key.keysym.sym=num;
    keystate[num]= (ev->type == SDL_KEYDOWN);
    printf("%d\n",ev->key.keysym.sym);
    printf("%d\n",ev->type);
  }
  else if(buf[1]=='u'){
    ev->type=SDL_KEYUP;
    printf("%d\n",ev->type);
  }
  else {ev->type=2;}
  return 1;
  }
  //printf("1\n");
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  //printf("sdl_wait\n");
  while(!SDL_PollEvent(event));
  
  /*char buf[64];
   int num;
   printf("sdl_wait\n");
  while(NDL_PollEvent(buf, sizeof(buf))==0);
    char key[32];
    
    printf("%s\n",buf);
    int i;
    num=0;
    //for(i=3;i<64&&buf[i]!=' ';i++){
    //  num=num*10+buf[i]-'0';
    //}
    i=3;
    int j=0;
    while(buf[i]!='\n'){
      key[j]=buf[i];
      i++;
      j++;
    }
    key[j]='\0';
    printf("%s\n",key);
    for(int j=0;j<83;j++){
      if(strcmp(key,keyname[j])==0) {num=j;break;}
    }
  if(buf[1]=='d'){
    event->type=SDL_KEYDOWN;
    event->key.keysym.sym=num;
    printf("%d\n",event->key.keysym.sym);
    //printf("%d\n",event->type);
  }
  else if(buf[1]=='u'){
    event->type=SDL_KEYUP;
    //printf("%d\n",event->type);
  }
  else {event->type=2;}*/
  //while(!SDL_PollEvent(event));
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  //assert(0);
  return keystate;
}
