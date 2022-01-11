#include <common.h>
extern void do_syscall(Context *c);
extern Context* schedule(Context *prev);
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case 1:return schedule(c);break;
    case 2:do_syscall(c);break;
    //case 3:printf("ev.event = EVENT_PAGEFAULT\n"); break;
    //case 4:printf("ev.event = EVENT_ERROR\n");break;
    //case 5:printf("ev.event = EVENT_IRQ_TIMER\n");break;
    //case 6:printf("ev.event = EVENT_IRQ_IODEV\n");break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
