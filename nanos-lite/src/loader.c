#include <proc.h>
#include <elf.h>
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
#define addr 0x83000000
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
//extern uint8_t ramdisk_start;
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  //assert(*(uint32_t *)elf->e_ident == 0x7f454c46);
  ramdisk_read((void *)addr, 0, get_ramdisk_size());

  return addr+8;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

