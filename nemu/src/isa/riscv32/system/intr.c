#include <isa.h>

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
  return cpu.mtvec;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
