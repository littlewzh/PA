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
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  //Elf_Ehdr * elf=(Elf_Ehdr *)ADDR;
  //assert(*(uint32_t *)elf->e_ident == 0x7f454c46);
  ramdisk_read((void *)ADDR, 0,get_ramdisk_size());
  
  return ADDR+0x3dc;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

