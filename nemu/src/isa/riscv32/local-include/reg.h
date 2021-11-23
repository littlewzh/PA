#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include <common.h>
static inline int check_reg_idx(int idx) {
  IFDEF(CONFIG_RT_CHECK, assert(idx >= 0 && idx < 32));
  return idx;
}

#define gpr(idx) (cpu.gpr[check_reg_idx(idx)]._32)
/*#define mstatus (cpu.mstatus._32)
#define mcause  (cpu.mcause._32)
#define mtvec   (cpu.mtvec._32)
#define mepc    (cpu.mepc._32)*/
static inline const char* reg_name(int idx, int width) {
  extern const char* regs[];
  return regs[check_reg_idx(idx)];
}

#endif
