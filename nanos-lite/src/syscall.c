/*#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}*/
#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  Log("system call ID=%d\n",a[0]);
  switch (a[0]) {
    //#ifdef CONFIG_STRACE
    
    //#endif
    case SYS_exit:
       halt(a[1]);
       break;
    case SYS_yield:
       yield();
       //halt(0);
       c->GPRx=0;
       break;
    case SYS_write:
      
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}