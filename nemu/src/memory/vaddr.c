#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  if(isa_mmu_check(addr,len,1)==0){
    return paddr_read(addr, len);
  }
  else{
    return paddr_read(isa_mmu_translate(addr,len,1),len);
  }
  //return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
  if(isa_mmu_check(addr,len,1)==0){
    return paddr_read(addr, len);
  }
  else{
    return paddr_read(isa_mmu_translate(addr,len,1),len);
  }
  //return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {

  //paddr_write(addr, len, data);
  if(isa_mmu_check(addr,len,1)==0){
    return paddr_write(addr, len,data);
  }
  else{
    return paddr_write(isa_mmu_translate(addr,len,1),len,data);
  }
}
