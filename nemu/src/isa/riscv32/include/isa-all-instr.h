#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(auipc) f(lw) f(sw) f(addi) f(jal) f(ret)f(add) f(sub) f(sltiu)f(or) f(beq) f(srai)f(lbu)f(xor) f(sltu) f(bne) f(bge) f(inv) f(nemu_trap)

def_all_EXEC_ID();
