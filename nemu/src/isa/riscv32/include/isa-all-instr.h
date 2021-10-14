#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(auipc) f(jal)f(jalr)f(lb)f(lh)f(lw)f(lbu)f(lhu)f(sb)f(sh)f(sw) f(addi)f(sltiu) f(slti)f(ori)f(slli)f(srli) f(andi) f(ret)f(add)f(mul)f(mulh)f(rem)f(sub)f(sll)f(slt)f(sltu)f(xor)f(div)f(srl)f(sra)f(or)f(xori)f(and)f(srai)  f(beq)f(bne)f(blt) f(bge)f(bltu) f(bgeu)    f(inv) f(nemu_trap)

def_all_EXEC_ID();
