#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  char *empty[] =  {NULL };
  printf("args = %p\n",args);
  int argc;
  argc=*args;
  printf("argc = %d\n",argc);
  /*char* argv[argc];
  for(int i=0;i<argc;i++){
     argv[i]=(char *)*(args+4+i*4);
  }
  printf("reach here\n");
  int num=0;
  while((*(args+4+4*argc+4*num))!=0) num++;
  char* envp[num];
  for(int i=0;i<num;i++){
     envp[i]=(char *)*(args+4+4*argc+i*4);
  }
  environ = envp;*/
  char **argv,**envp;
  argv=args+1;
  printf("argv = %p\n",argv);
  envp=args+(2+argc);
  printf("envp = %p\n",envp);
  exit(main(argc, argv, envp));
  assert(0);
}
