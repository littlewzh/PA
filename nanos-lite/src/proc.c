#include <proc.h>

#define MAX_NR_PROC 4
extern void naive_uload(PCB *pcb, const char *filename);
extern uintptr_t loader(PCB *pcb, const char *filename);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
extern Area heap;
void switch_boot_pcb() {
  current = &pcb_boot;
}
void context_uload(PCB *pcb,const char *filename, char *const argv[], char *const envp[]){
  Area ustack;
  //ustack.end=heap.end;
  //ustack.start=ustack.end-sizeof(pcb->stack);
  ustack.start= pcb->stack;
  ustack.end = ustack.start + sizeof(pcb->stack);
  uintptr_t entry = loader(pcb, filename);
  pcb->cp = ucontext(NULL,ustack,(void *)entry);

  pcb->cp->gpr[10]=(uintptr_t)heap.end-4;
  //printf("%x\n",&kstack.start);
  //printf("%x\n",&kstack.end);
  //printf("heap.start=%x\n",heap.start);
  //printf("heap.end=%x\n",heap.end);
}
void context_kload(PCB *pcb,void (*entry)(void *), void *arg){
  Area kstack;
  kstack.start= pcb->stack;
  kstack.end = kstack.start + sizeof(pcb->stack);
  //printf("kstack.start=%x\n",kstack.start);
  //printf("kstack.end=%x\n",kstack.end);
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
  context_kload(&pcb[0], hello_fun, (void *)8000);
  context_uload(&pcb[1], "/bin/pal",NULL,NULL);
  //context_kload(&pcb[1], hello_fun, (void *)1000);
  switch_boot_pcb();

  Log("Initializing processes...");
  //naive_uload(NULL,"/bin/menu");
  //load program here

}

Context* schedule(Context *prev) {
  // save the context pointer
current->cp = prev;

// always select pcb[0] as the new process
//current = &pcb[0];
current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
// then return the new context
return current->cp;
}
