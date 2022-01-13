#include <proc.h>

#define MAX_NR_PROC 4
extern void naive_uload(PCB *pcb, const char *filename);
extern void* new_page(size_t nr_page);
extern void context_uload(PCB *pcb,const char *filename, char *const argv[], char *const envp[]);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
extern Area heap;
void switch_boot_pcb() {
  current = &pcb_boot;
}

void context_kload(PCB *pcb,void (*entry)(void *), void *arg){
  Area kstack;
  kstack.start= pcb->stack;
  kstack.end = kstack.start + sizeof(pcb->stack);
  //printf("kstack.start=%x\n",kstack.start);
  //printf("kstack.end=%x\n",kstack.end);
  assert(entry!=NULL);
  pcb->cp = kcontext(kstack,entry,arg);
}
void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  //context_uload(&pcb[0], "/bin/hello");
  context_kload(&pcb[1], hello_fun, NULL);
  char* empty[] ={NULL};
  //char *environ[] ={NULL };
	char *args[] = {"--skip", NULL};
  //context_uload(&pcb[0], "/bin/exec-test",empty,empty);
  context_uload(&pcb[0], "/bin/pal",args,empty);
  //context_uload(&pcb[0], "/bin/menu",args,empty);
  //context_uload(&pcb[0], "/bin/nterm",args,empty);
  //context_kload(&pcb[1], hello_fun, (void *)1000);
  switch_boot_pcb();

  Log("Initializing processes...");
  //naive_uload(NULL,"/bin/exec-test");
  //load program here

}

Context* schedule(Context *prev) {
  // save the context pointer
current->cp = prev;

// always select pcb[0] as the new process
current = &pcb[1];
//current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
// then return the new context
return current->cp;
}
