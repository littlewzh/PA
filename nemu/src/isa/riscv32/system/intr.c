#include <isa.h>
#define IRQ_TIMER 0x80000007  // for riscv32
word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  #ifdef CONFIG_ETRACE
    Log("error %d at 0x%08x",NO,epc);
  #endif
  cpu.mcause=NO;
  cpu.mepc=epc;
  //printf("%x\n",cpu.mtvec);
  cpu.mstatus=(cpu.mstatus<<4)&0x80;                                   //MIE(3) MPIE(7)  

  return cpu.mtvec;
}

word_t isa_query_intr() {
   if (cpu.INTR==1&&((cpu.mstatus&0x8)!=0)) {
    cpu.INTR = false;
    return IRQ_TIMER;
  }
  return INTR_EMPTY;
}
