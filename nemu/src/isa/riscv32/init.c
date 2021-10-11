#include <isa.h>
#include <memory/paddr.h>

// this is not consistent with uint8_t
// but it is ok since we do not access the array directly
static const uint32_t img [] = {
  0x00000413,  // lui t0,0x80000
  0x00009117,  // sw  zero,0(t0)
  0xffc10113,  // lw  a0,0(t0)
  0x00c000ef,  // nemu_trap
  0x00000513,
  0x00008067,
  0x80000537,
  0xff010113,
  0x03850513,
  0x00112623,
  0xfe9ff0ef,
  0x00050513,
  0x0000006b,
  0x0000006f,
};

static void restart() {
  /* Set the initial program counter. */
  cpu.pc = RESET_VECTOR;

  /* The zero register is always 0. */
  cpu.gpr[0]._32 = 0;
}

void init_isa() {
  /* Load built-in image. */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));

  /* Initialize this virtual computer system. */
  restart();
}
