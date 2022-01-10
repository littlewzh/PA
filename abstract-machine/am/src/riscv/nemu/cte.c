#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;         //定义了一个函数指针
Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 0xb:ev.event = 1; break;
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
  Context* text=(Context *)kstack.end;
  memset(text,0,sizeof(Context));
  text->mepc=(uintptr_t)entry;
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
