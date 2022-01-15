#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  uint32_t offpage = vaddr & 0xfff;
  uint32_t offfirst = (uint32_t)vaddr >> 22;
  uint32_t offsecond  = ((uint32_t)vaddr & 0x003ff000 ) >> 12;
  uint32_t base=cpu.satp;
  paddr_t pte=paddr_read(base*4096+4*offfirst,4);
  if((pte&1) == 0) {assert(0);}
  else {
    paddr_t pagebase = paddr_read((pte & 0xfffff000) + offsecond*4 , 4);
    if ((pagebase & 1)==0) {assert(0);}
    else {return ((pagebase & 0xfffff000) + offpage);}
    //return ((pagebase & 0xfffff000) + offpage);
  }
  
  return MEM_RET_FAIL;
}
int isa_mmu_check(vaddr_t vaddr, int len, int type) {
  if((cpu.satp>>31) == 0){
    return MMU_DIRECT;
  }
  else {return MMU_TRANSLATE;}
}