#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif
extern uint8_t ramdisk_start;
#define NAME(key) \
  [AM_KEY_##key] = #key,

#define KEYDOWN_MASK 0x8000
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  #ifndef HAS_TIMER_IRQ
  yield();
  #endif
    char *s=(char *)buf;
    for(int i=0;i<len;i++){
      putch(*(s+i));
    }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  #ifndef HAS_TIMER_IRQ
  yield();
  #endif
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  //int keycode=io_read(AM_INPUT_KEYBRD).keycode;
  //int keydown=io_read(AM_INPUT_KEYBRD).keydown;
  if(ev.keycode==AM_KEY_NONE){
    return 0;
  }
  if(ev.keydown){
    sprintf(buf,"kd %s\n",keyname[ev.keycode]);
  }
  else {
    sprintf(buf,"ku %s\n",keyname[ev.keycode]);
  }
  return strlen(buf);
}
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //sprintf(buf,"%s",(char *)offset);
  //strncpy((char *)buf,(char *)(offset),len);
  ramdisk_read(buf,offset,len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  //AM_GPU_FBDRAW_T *ctl;
  #ifndef HAS_TIMER_IRQ
  yield();
  #endif
  int screen_w = 400;//io_read(AM_GPU_CONFIG).width;
  //int screen_h = io_read(AM_GPU_CONFIG).height;
  int x,y;
  x=(offset/4)%screen_w;
  y=(offset/4)/screen_w;
  //for(int )
  io_write(AM_GPU_FBDRAW, x, y,(uint32_t *)buf, len, 1, true);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
