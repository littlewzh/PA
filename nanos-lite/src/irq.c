#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
      case 1:panic("ev.event = EVENT_YIELD"); break;
      case 2:panic("ev.event = EVENT_SYSCALL");break;
      case 3:panic("ev.event = EVENT_PAGEFAULT"); break;
      case 4:panic("ev.event = EVENT_ERROR");break;
      case 5:panic("ev.event = EVENT_IRQ_TIMER");break;
      case 6:panic("ev.event = EVENT_IRQ_IODEV");break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
