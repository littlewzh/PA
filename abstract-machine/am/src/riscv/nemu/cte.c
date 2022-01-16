#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>




extern void __am_get_cur_as(Context *c);
extern void __am_switch(Context *c);
static Context* (*user_handler)(Event, Context*) = NULL;         //定义了一个函数指针


Context* __am_irq_handle(Context *c) {

  __am_get_cur_as(c);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 0xb:ev.event = 1; break;
      case 0x80000007: ev.event=3;break;
      case 0:case 2:case 1:case 3:case 4:case 7:case 8:case 9:case 13:case 19: ev.event = 2;break;
      //case 3:ev.event = EVENT_PAGEFAULT; break;
      //case 4:ev.event = EVENT_ERROR;break;
      //case 5:ev.event = EVENT_IRQ_TIMER;break;
      //case 6:ev.event = EVENT_IRQ_IODEV;break;
      default: ev.event = EVENT_ERROR;break;
    }
    c = user_handler(ev, c);
    assert(c != NULL);
  }
  if(c->pdir!=NULL) __am_switch(c);

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {            //其参数为一个函数类型，这样可以进行传递函数
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context* text=(Context *)(kstack.end-sizeof(Context)-4);
  //printf("text = %p\n",text);
  //printf("kstack.end = %p\n",kstack.end);
  memset(text,0,sizeof(Context));
  assert(entry!=NULL);
  text->mepc=(uintptr_t)entry;
  text->mstatus=0x8;
  text->gpr[10]=(uintptr_t)arg;   //a0 register is function arg
  text->pdir=NULL;
  return text;
}

void yield() {
  asm volatile("li a7, 0xb; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
