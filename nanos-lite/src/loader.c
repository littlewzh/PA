#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
#define ADDR 0x83000000
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Elf_Ehdr elf;
  ramdisk_read(&elf,0,sizeof(Elf_Ehdr));
  assert(*(uint32_t *)elf.e_ident == 0x7f454c46);
  Elf_Phdr phlf;
  ramdisk_read(&phlf,elf.e_phoff,elf.e_phentsize);

  //ramdisk_read((void *)elf, 0,sizeof(Elf_Ehdr));
  
  //ramdisk_read((void *)ADDR, 0,get_ramdisk_size());
  for (size_t i = 0; i < elf.e_phnum; ++i) {
    ramdisk_read((void *)phlf.p_vaddr,phlf.p_offset,phlf.p_memsz);
  }
  return elf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

