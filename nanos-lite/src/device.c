#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

#define KEYDOWN_MASK 0x8000

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
    char *s=(char *)buf;
    for(int i=0;i<len;i++){
      putch(*(s+i));
    }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
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
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
