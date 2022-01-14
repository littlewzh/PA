#include <proc.h>
#include <elf.h>
#include <fs.h>
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
extern void* new_page(size_t nr_page);
extern void map(AddrSpace *as, void *va, void *pa, int prot);
extern Area heap;
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  /*Elf_Ehdr elf;
  ramdisk_read(&elf,0,sizeof(Elf_Ehdr));
  assert(*(uint32_t *)elf.e_ident == 0x464c457f);
  
  
  //ramdisk_read((void *)elf, 0,sizeof(Elf_Ehdr));
  //ramdisk_read((void *)ADDR, 0,get_ramdisk_size());
  for (size_t i = 0; i < 2; ++i) {
    Elf_Phdr phlf;
    ramdisk_read(&phlf,elf.e_phoff+elf.e_phentsize * i,elf.e_phentsize);
    ramdisk_read((void *)phlf.p_vaddr,phlf.p_offset,phlf.p_memsz);
    memset((void *)(phlf.p_vaddr+phlf.p_filesz),0,phlf.p_memsz-phlf.p_filesz);
  }*/
  int fd=fs_open(filename,0,0);
  Elf_Ehdr elf;
  fs_read(fd,&elf,sizeof(Elf_Ehdr));
  assert(*(uint32_t *)elf.e_ident == 0x464c457f);
  for(int i=0;i<elf.e_phnum;i++){
    Elf_Phdr phlf;
    fs_lseek(fd,elf.e_phoff+elf.e_phentsize * i,SEEK_SET);
    fs_read(fd,&phlf,elf.e_phentsize);
    if (phlf.p_type == PT_LOAD){
       fs_lseek(fd,phlf.p_offset,SEEK_SET);
       #ifdef HAS_VME
       int pagenum=(((phlf.p_vaddr+phlf.p_memsz)&0xfffff000)-(phlf.p_vaddr &0xfffff000))/PGSIZE +1;
       uint32_t vaddr = phlf.p_vaddr;
       uint32_t limit = phlf.p_vaddr + phlf.p_filesz;
       for(int j=0;j<pagenum;j++){
          uint32_t paddr = (uint32_t)new_page(1);              //shenqing1 ye de wulineicun
          paddr = (paddr &0xfffff000) + (vaddr&0xfff);
          if(vaddr < limit){
            if((vaddr+PGSIZE)< limit){
              size_t len = (vaddr&0xfffff000) + PGSIZE - vaddr;
              fs_read(fd,(void*)paddr,len);
            }
            else{
              size_t len=limit-vaddr ;
              fs_read(fd,(void*)paddr,len);
              memset((void*)(paddr+len),0,PGSIZE-len);
            }
          }
          else{
          memset((void*)paddr,0,PGSIZE);
        }
        map(&pcb->as,(void*)(vaddr&0xfffff000),(void*)paddr,0);
        vaddr = (vaddr&0xfffff000) + PGSIZE;
        pcb->max_brk = vaddr;
      
      }

       #else
       fs_read(fd,(void *)phlf.p_vaddr,phlf.p_memsz);
       memset((void *)(phlf.p_vaddr+phlf.p_filesz),0,phlf.p_memsz-phlf.p_filesz);
       #endif
    }
  }
  return elf.e_entry;
  

}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

void context_uload(PCB *pcb,const char *filename, char *const argv[], char *const envp[]){
  uint32_t start,sp;
  start=(uint32_t)new_page(8);  //表示用户栈的开始
  sp=start+31*1024;               //栈指针，并将最高的1KB设置为unspecified区域
  if(argv!=NULL&&envp!=NULL){
  int num;
  char *uenvp[16];
  for(num=0;envp[num]!=0;num++) {
		sp-=strlen(envp[num])+1;
		memcpy((void *)sp, (void *)(envp[num]), strlen(envp[num]) + 1);
		uenvp[num]=(char *)sp;
	}
  uenvp[num]=0;                   //
  int argc;
  char *uargv[16];
	for(argc=0;argv[argc]!=0;argc++) {
		sp-=strlen(argv[argc])+1;
		memcpy((void *)sp, (void *)(argv[argc]), strlen(argv[argc]) + 1);
		uargv[argc]=(char *)sp;
	}
	uargv[argc]=0;                  //
	sp-=1024;                        //unspecified区域
	sp-=(num+1)*sizeof(uint32_t);
	sp-=sp % 4;                      //对齐
	memcpy((void *)sp, (void *)uenvp, sizeof(uint32_t) * (num + 1));
  //printf("envp = %p\n",sp);
	sp-=(argc + 1)*sizeof(uint32_t);
	memcpy((void *)sp, (void *)uargv, sizeof(uint32_t) * (argc + 1));
  //printf("argv = %p\n",sp);
	sp-=sizeof(uint32_t);
	memcpy((void *)sp, (void *)&argc, sizeof(uint32_t));
  //printf("&argc = %p\n",sp);
  }
  protect(&pcb->as);
  AddrSpace *AS=&pcb->as;
  Area ustack;
  //ustack.end=heap.end;
  //ustack.start=ustack.end-sizeof(pcb->stack);
  ustack.start= pcb->stack;
  ustack.end = ustack.start + sizeof(pcb->stack);
  assert(filename!=NULL);
  uintptr_t entry = loader(pcb, filename);
  pcb->cp = ucontext(AS,ustack,(void *)entry);      //初始化，使得当前的进程块的cp指向此程序保存context的地址
  pcb->cp->gpr[10]=(uintptr_t)sp;
  //printf("finish\n");
  //printf("%x\n",&kstack.start);
  //printf("%x\n",&kstack.end);
  //printf("heap.start=%x\n",heap.start);
  //printf("heap.end=%x\n",heap.end);
}