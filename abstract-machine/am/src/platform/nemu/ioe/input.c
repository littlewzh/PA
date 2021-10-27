#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000
#define kaddr 0xa0000060
static uint64_t k=0;
void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  k=inl(kaddr);
  //kbd->keycode=k;
  kbd->keydown = (k& KEYDOWN_MASK ? true: false);
  kbd->keycode = k&~KEYDOWN_MASK;
}
