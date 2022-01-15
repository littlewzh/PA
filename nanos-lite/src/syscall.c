#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <sys/time.h>
#include <time.h> 
#include <proc.h>
extern void naive_uload(PCB *pcb, const char *filename);
extern void context_uload(PCB *pcb,const char *filename, char *const argv[], char *const envp[]);
extern void switch_boot_pcb();
//extern int gettimeofday(struct timeval * tv, struct timezone * tz);
int sys_gettimeofday(struct timeval * tv, struct timezone * tz){
  //gettimeofday(tv, tz);
  uint64_t tim=io_read(AM_TIMER_UPTIME).us;
  tv->tv_sec=tim>>32;
  tv->tv_usec=tim;
  return 0;
}
int32_t syswrite(int fd, const void *buf, size_t len){
  /*if(fd==1){
    char *s=(char *)buf;
    for(int i=0;i<len;i++){
      putch(*(s+i));
    }
  }
  else if(fd==2){
    return -1;
  }*/
    return fs_write(fd,buf,len);
  //}
  //return -1;
}
uint32_t sysexecve(const char *filename, char *const argv[], char *const envp[]){
   context_uload(current,filename,argv,envp);
   switch_boot_pcb();
   yield();
   return -1;
}
//extern size_t fb_write(const void *buf, size_t offset, size_t len);
static uint32_t buf[300*400]={0};
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  //Log("system call ID=%d\n",a[0]);
  switch (a[0]) {
    //#ifdef CONFIG_STRACE
    
    //#endif
    case SYS_exit:
       //naive_uload(NULL,"/bin/menu");
       halt(a[1]);
       break;
    case SYS_yield:
       yield();
       //halt(0);
       c->GPRx=0;
       break;
    case SYS_open:
      c->GPRx=fs_open((char *)a[1],a[2],a[3]);
      break;
    case SYS_read:
      c->GPRx=fs_read(a[1],(void *)a[2],a[3]);
      break;
    case SYS_write:
       c->GPRx=syswrite(a[1], (void *)a[2], a[3]);
       break;
    case SYS_close:
       c->GPRx=fs_close(a[1]);
       break;
    case SYS_lseek:
       c->GPRx=fs_lseek(a[1],a[2],a[3]);
       break;
    case SYS_brk:
       c->GPRx=0;
       break;
    case SYS_gettimeofday:
       c->GPRx=sys_gettimeofday((struct timeval *) a[1], (struct timezone *) a[2]);
       break;
    case SYS_execve:
       io_write(AM_GPU_FBDRAW, 0, 0,(uint32_t *)buf, 400, 300, true);
       //naive_uload(NULL,(char *)a[1]);
       c->GPRx=sysexecve((char *)a[1],(char **)a[2],(char **)a[3]);
       break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}