#include <proc.h>

#define MAX_NR_PROC 4
extern void naive_uload(PCB *pcb, const char *filename);
extern uintptr_t loader(PCB *pcb, const char *filename);
extern void* new_page(size_t nr_page);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
extern Area heap;
void switch_boot_pcb() {
  current = &pcb_boot;
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
  printf("envp = %p\n",sp);
	sp-=(argc + 1)*sizeof(uint32_t);
	memcpy((void *)sp, (void *)uargv, sizeof(uint32_t) * (argc + 1));
  printf("argv = %p\n",sp);
	sp-=sizeof(uint32_t);
	memcpy((void *)sp, (void *)&argc, sizeof(uint32_t));
  printf("&argc = %p\n",sp);
  }
  
  Area ustack;
  //ustack.end=heap.end;
  //ustack.start=ustack.end-sizeof(pcb->stack);
  ustack.start= pcb->stack;
  ustack.end = ustack.start + sizeof(pcb->stack);
  assert(filename!=NULL);
  uintptr_t entry = loader(pcb, filename);
  pcb->cp = ucontext(NULL,ustack,(void *)entry);
  pcb->cp->gpr[10]=(uintptr_t)sp;
  printf("finish\n");
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
  //char* empty[] ={NULL};
  //char *environ[] ={NULL };
	//char *args[] = {"--skip", NULL};
  //context_uload(&pcb[0], "/bin/exec-test",empty,empty);
  context_uload(&pcb[0], "/bin/pal",NULL,NULL);
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
//current = &pcb[0];
current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
// then return the new context
return current->cp;
}
