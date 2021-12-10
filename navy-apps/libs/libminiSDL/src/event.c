#include <NDL.h>
#include <SDL.h>
#include <string.h>
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
  char key[32];
  if(NDL_PollEvent(buf, sizeof(buf))){
    //char *arg=strtok(NULL," ");
    //arg=strtok(NULL," ");
    printf("%s\n",buf);
    num=0;
    int i;
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
    for(int j=0;j<83;j++){
      if(strcmp(key,keyname[j])==0) {num=j;break;}
    }
  if(buf[1]=='d'){
    ev->type=SDL_KEYDOWN;
    ev->key.keysym.sym=num;
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
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  
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
  else {event->type=2;}
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
