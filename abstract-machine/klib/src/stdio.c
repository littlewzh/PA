#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  /*char *str=out;
  int n;
  char *s;
  while(*fmt){
    if(*fmt!='%'){
      *str++=*fmt++;

    }
    else {
      fmt++;
      if(*fmt=='d'){
        n=va_arg(ap,int);
        //while(n){
        //}
        *str++=n-'0';
      }
      else if(*fmt=='s'){
        s=va_arg(ap,char *);
        strcat(str,s);
      }
    }
  }*/
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
    va_list args;
    int value;
    va_start(args,fmt);
    value=vsprintf(out,fmt, args);
    va_end(args);
    return value;
  //panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
