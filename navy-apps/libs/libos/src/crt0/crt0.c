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
  if(argc!=0){
    printf("argc = %d\n",argc);
    char **argv,**envp;
    argv=args+1;
    printf("argv = %p\n",argv);
    envp=args+(2+argc);
    printf("envp = %p\n",envp);
    exit(main(argc, argv, envp));
  }
  else {exit(main(0,empty,empty));}
  
  assert(0);
}
