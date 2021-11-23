#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for(int i=1;i<32;i++){
    difftest_check_reg(reg_name(i,4),pc, ref_r->gpr[i]._32, cpu.gpr[i]._32);
    
  }
  //difftest_check_reg("pc", pc, ref_r->pc, cpu.pc);
  return true;
}

void isa_difftest_attach() {
}
