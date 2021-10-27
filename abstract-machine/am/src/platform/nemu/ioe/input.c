#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000
#define kaddr 0x60
void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {

  kbd->keydown = (inl(kaddr)& KEYDOWN_MASK ? false: true);
  kbd->keycode = inl(kaddr)&~KEYDOWN_MASK;
}
