#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  if(inl(KBD_ADDR)!=0){
    kbd->keydown = true;
    kbd->keycode =inl(KBD_ADDR);
  }
  else{
    kbd->keydown = 0;
    kbd->keycode =AM_KEY_NONE;
  }
}
