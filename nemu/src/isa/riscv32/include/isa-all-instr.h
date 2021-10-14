#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(auipc) f(lw) f(sw)f(sb) f(addi)f(sltiu) f(slti) f(andi)f(jal) f(ret)f(add) f(sub)f(sll)f(slt)f(sltu)f(xor)f(or)f(xori)f(and)f(srai) f(lbu) f(beq)f(bne)f(blt) f(bge)f(bltu) f(bgeu)    f(inv) f(nemu_trap)

def_all_EXEC_ID();
