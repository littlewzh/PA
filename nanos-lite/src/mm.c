#include <memory.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *new = pf;
#ifdef HAS_VME
  memset(new,0,PGSIZE*nr_page);
#endif
  pf = (void *)((int)pf + nr_page*PGSIZE);
  return new;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
  int page_num = (n + PGSIZE - 1) / PGSIZE;
  return new_page(page_num);
  //return NULL;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);           //fenpei 32KB memory from heap.start
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
