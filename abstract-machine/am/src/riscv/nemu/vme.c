#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};                       //
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);     //将mode设为1,表示此时进入分页模式
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {                //3
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);                               //调用map()来填写内核虚拟地址空间(kas)的页目录和页表, 最后设置一个叫satp(Supervisor Address Translation and Protection)的CSR寄存器来开启分页机制. 
    }
  }
  //printf("%d\n",i);
  set_satp(kas.ptr);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));     //uintptr_t,
  as->ptr = updir;                             //ptr是一个ISA相关的地址空间描述符指针, 用于指示具体的映射
  as->area = USER_SPACE;                      //area表示虚拟地址空间中用户态的范围
  as->pgsize = PGSIZE;               //4096
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {            //它用于将地址空间as中虚拟地址va所在的虚拟页, 以prot的权限映射到pa所在的物理页. 

  uint32_t off_first = (uint32_t)va >> 22;                   //一级页表偏移
  uint32_t off_second=((uint32_t)va&0x003ff000)>>12;         //二级页表偏移
  uint32_t *base = (uint32_t *)as->ptr;                       //base
  uint32_t *pte = &base[off_first];
  if((*pte & 1)==0){
  uint32_t* tem = (uint32_t*)pgalloc_usr(4096);
  *pte = ((uint32_t)tem &  0xfffff000) | 1;
  }
   *(uint32_t*)((int)(*pte & 0xfffff000)+ off_second*4) = ((uint32_t)pa & 0xfffff000) | 1;
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
   Context *text=kstack.end-sizeof(Context);
   memset(text,0,sizeof(Context));
   //text->gpr[10]=(uintptr_t)heap.end;
   text->mepc=(uintptr_t)entry;
   text->mstatus=0x80;
   //text->satp=(uintptr_t)as->ptr;
   return text;
  //return NULL;
}
