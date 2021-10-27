#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000
#define kaddr 0xa0000060
uint64_t k;
void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  k=inl(kaddr);
  kbd->keycode=k;
  if(kbd->keycode!=AM_KEY_NONE){
      kbd->keydown=!(kbd->keydown);
  }
  //kbd->keydown = (inl(kaddr)& KEYDOWN_MASK ? false: true);
  //kbd->keycode = inl(kaddr)&~KEYDOWN_MASK;
}
